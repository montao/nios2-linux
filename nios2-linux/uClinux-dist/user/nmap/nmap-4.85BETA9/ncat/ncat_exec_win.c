#include <assert.h>

#include "ncat.h"

/* A list of subprocesses, so we can kill them when the program exits. */
static HANDLE subprocesses[DEFAULT_MAX_CONNS];
static int subprocess_max_index = 0;
/* Prevent concurrent access to the subprocesses table by the main process and
   a thread. Protects subprocesses and subprocesses_max_index. */
static HANDLE subprocesses_mutex = NULL;

static int start_subprocess(char *cmdexec, struct subprocess_info *info);
static DWORD WINAPI subprocess_thread_func(void *data);

static int register_subprocess(HANDLE proc);
static int unregister_subprocess(HANDLE proc);
static int get_subprocess_slot(void);

/* Have we registered the termination handler yet? */
static int atexit_registered = 0;
static void terminate_subprocesses(void);
static void sigint_handler(int s);

/* This structure holds information about a subprocess with redirected input
   and output handles. */
struct subprocess_info {
    HANDLE proc;
    SOCKET fd;
    HANDLE child_in_r;
    HANDLE child_in_w;
    HANDLE child_out_r;
    HANDLE child_out_w;
};

/* Run a child process, redirecting its standard file handles to a socket
   descriptor. Return the child's PID or -1 on error. */
int netrun(int fd, char *cmdexec)
{
    struct subprocess_info *info;
    HANDLE thread;
    int pid;

    info = (struct subprocess_info *) safe_malloc(sizeof(*info));
    info->fd = fd;

    pid = start_subprocess(cmdexec, info);
    if (pid == -1) {
        free(info);
        return -1;
    }

    /* Start up the thread to handle process I/O. */
    thread = CreateThread(NULL, 0, subprocess_thread_func, info, 0, NULL);
    if (thread == NULL) {
        if (o.verbose)
            logdebug("Error in CreateThread: %d\n", GetLastError());
        free(info);
        return -1;
    }
    CloseHandle(thread);

    return pid;
}

/* Run the given command line as if by exec. Doesn't return. */
void netexec(int fd, char *cmdexec)
{
    struct subprocess_info *info;
    int pid;
    DWORD ret;

    info = (struct subprocess_info *) safe_malloc(sizeof(*info));
    info->fd = fd;

    pid = start_subprocess(cmdexec, info);
    if (pid == -1)
        ExitProcess(2);

    /* Run the subprocess thread function, but don't put it in a thread. Just
       run it and exit with its return value because we're simulating exec. */
    ExitProcess(subprocess_thread_func(info));
}

/* Run a command and redirect its input and output handles to a pair of
   anonymous pipes.  The process handle and pipe handles are returned in the
   info struct. Returns the PID of the new process, or -1 on error. */
static int run_command_redirected(char *cmdexec, struct subprocess_info *info) {
    /* Each named pipe we create has to have a unique name. */
    static int pipe_serial_no = 0;
    char pipe_name[32];
    SECURITY_ATTRIBUTES sa;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    /* Make the pipe handles inheritable. */
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    /* The child's input pipe is an ordinary blocking pipe. */
    if (CreatePipe(&info->child_in_r, &info->child_in_w, &sa, 0) == 0) {
        if (o.verbose)
            logdebug("Error in CreatePipe: %d\n", GetLastError());
        return -1;
    }

    /* Pipe names must have this special form. */
    Snprintf(pipe_name, sizeof(pipe_name), "\\\\.\\pipe\\ncat-%d", pipe_serial_no);
    if (o.debug > 1)
        logdebug("Creating named pipe \"%s\"\n", pipe_name);

    /* The output pipe has to be nonblocking, which requires this complicated
       setup. */
    info->child_out_r = CreateNamedPipe(pipe_name,
        PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE, 1, 4096, 4096, 1000, &sa);
    if (info->child_out_r == 0) {
        if (o.verbose)
            logdebug("Error in CreateNamedPipe: %d\n", GetLastError());
        CloseHandle(info->child_in_r);
        CloseHandle(info->child_in_w);
        return -1;
    }
    info->child_out_w = CreateFile(pipe_name,
        GENERIC_WRITE, 0, &sa, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    if (info->child_out_w == 0) {
        CloseHandle(info->child_in_r);
        CloseHandle(info->child_in_w);
        CloseHandle(info->child_out_r);
        return -1;
    }
    pipe_serial_no++;

    /* Don't inherit our end of the pipes. */
    SetHandleInformation(info->child_in_w, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(info->child_out_r, HANDLE_FLAG_INHERIT, 0);

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = info->child_in_r;
    si.hStdOutput = info->child_out_w;
    si.hStdError = info->child_out_w;
    si.dwFlags |= STARTF_USESTDHANDLES;

    memset(&pi, 0, sizeof(pi));

    if (CreateProcess(NULL, cmdexec, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == 0) {
        if (o.verbose)
            logdebug("Error in CreateProcess: %d\n", GetLastError());
        CloseHandle(info->child_in_r);
        CloseHandle(info->child_in_w);
        CloseHandle(info->child_out_r);
        CloseHandle(info->child_out_w);
        return -1;
    }

    info->proc = pi.hProcess;

    return pi.dwProcessId;
}

static const char *get_shell(void)
{
    const char *comspec;

    comspec = getenv("COMSPEC");
    if (comspec == NULL)
        comspec = "cmd.exe";

    return comspec;
}

static void subprocess_info_close(struct subprocess_info *info)
{
    closesocket(info->fd);
    CloseHandle(info->proc);
    CloseHandle(info->child_in_r);
    CloseHandle(info->child_in_w);
    CloseHandle(info->child_out_r);
    CloseHandle(info->child_out_w);
}

/* Start a subprocess with run_command_redirected and register it with the
   termination handler. Takes care of o.shellexec. Returns the PID of the
   subprocess or -1 on error. */
static int start_subprocess(char *cmdexec, struct subprocess_info *info)
{
    char *cmdbuf;
    int pid;

    if (o.shellexec) {
        /* Run with cmd.exe. */
        const char *shell;
        size_t cmdlen;

        shell = get_shell();
        cmdlen = strlen(shell) + strlen(cmdexec) + 32;
        cmdbuf = (char *) safe_malloc(cmdlen);
        Snprintf(cmdbuf, cmdlen, "%s /C %s", shell, cmdexec);
    } else {
        cmdbuf = cmdexec;
    }

    if (o.debug)
        logdebug("Executing: %s\n", cmdbuf);

    pid = run_command_redirected(cmdbuf, info);

    if (cmdbuf != cmdexec)
        free(cmdbuf);

    if (register_subprocess(info->proc) == -1) {
        if (o.verbose)
            logdebug("Couldn't register subprocess with termination handler; not executing.\n");
        TerminateProcess(info->proc, 2);
        subprocess_info_close(info);
        return -1;
    }

    return pid;
}

/* Relay data between a socket and a process until the process dies or stops
   sending or receiving data. The socket descriptor and process pipe handles
   are in the data argument, which must be a pointer to struct subprocess_info.

   This function is a workaround for the fact that we can't just run a process
   after redirecting its input handles to a socket. If the process, for
   example, redirects its own stdin, it somehow confuses the socket and stdout
   stops working. This is exactly what ncat does (as part of the Windows stdin
   workaround), so it can't be ignored.

   This function can be invoked through CreateThread to simulate fork+exec, or
   called directly to simulate exec. It frees the subprocess_info struct and
   closes the socket and pipe handles before returning. Returns the exit code
   of the subprocess. */
static DWORD WINAPI subprocess_thread_func(void *data) {
    struct subprocess_info *info;
    char pipe_buffer[BUFSIZ];
    OVERLAPPED overlap = { 0 };
    HANDLE events[3];
    DWORD ret;

    info = (struct subprocess_info *) data;

    /* Three events we watch for: socket read, pipe read, and process end. */
    events[0] = (HANDLE) WSACreateEvent();
    WSAEventSelect(info->fd, events[0], FD_READ | FD_CLOSE);
    events[1] = info->child_out_r;
    events[2] = info->proc;

    /* To avoid blocking or polling, we use asynchronous I/O, or what Microsoft
       calls "overlapped" I/O, on the process pipe. WaitForMultipleObjects
       reports when the read operation is complete. */
    ReadFile(info->child_out_r, pipe_buffer, sizeof(pipe_buffer), NULL, &overlap);

    /* Loop until EOF or error. */
    for (;;) {
        DWORD n, nwritten;
        int i;

        i = WaitForMultipleObjects(3, events, FALSE, INFINITE);
        if (i == WAIT_OBJECT_0) {
            /* Read from socket, write to process. */
            char buffer[BUFSIZ];

            if (ReadFile((HANDLE) info->fd, buffer, sizeof(buffer), &n, NULL) == 0)
                break;
            if (n == -1 || n == 0)
                break;
            if (WriteFile(info->child_in_w, buffer, n, &nwritten, NULL) == 0)
                break;
            if (nwritten != n)
                break;
            ResetEvent(events[0]);
        } else if (i == WAIT_OBJECT_0 + 1) {
            /* Read from process, write to socket. */
            if (GetOverlappedResult(info->child_out_r, &overlap, &n, FALSE)) {
                if (WriteFile((HANDLE) info->fd, pipe_buffer, n, &nwritten, NULL) == 0)
                    break;
                if (nwritten != n)
                    break;
                /* Queue another ansychronous read. */
                ReadFile(info->child_out_r, pipe_buffer, sizeof(pipe_buffer), NULL, &overlap);
            } else {
                if (GetLastError() != ERROR_IO_PENDING)
                    /* Error or end of file. */
                    break;
            }
        } else if (i == WAIT_OBJECT_0 + 2) {
            /* The child died. There are no more writes left in the pipe
               because WaitForMultipleObjects guarantees events with lower
               indexes are handled first. */
            break;
        } else {
            break;
        }
    }

    WSACloseEvent(events[0]);

    GetExitCodeProcess(info->proc, &ret);
    if (o.debug > 1)
        logdebug("Subprocess ended with exit code %d.\n", ret);

    assert(unregister_subprocess(info->proc) != -1);
    shutdown(info->fd, 2);
    subprocess_info_close(info);
    free(info);

    return ret;
}

/* Find a free slot in the subprocesses table. Update subprocesses_max_index to
   be one greater than the maximum index containing a non-NULL handle. (It is
   assumed that the index returned by this function will be filled by a
   handle.) */
static int get_subprocess_slot(void)
{
    int i, free_index, max_index;

    assert(WaitForSingleObject(subprocesses_mutex, INFINITE) == WAIT_OBJECT_0);

    free_index = -1;
    max_index = 0;
    for (i = 0; i < subprocess_max_index; i++) {
        HANDLE proc = subprocesses[i];
        DWORD ret;

        if (proc == NULL) {
            if (free_index == -1)
                free_index = i;
        } else {
            max_index = i + 1;
        }
    }
    if ((free_index == -1 || free_index == max_index)
        && max_index < sizeof(subprocesses) / sizeof(subprocesses[0]))
        free_index = max_index++;
    subprocess_max_index = max_index;

    assert(ReleaseMutex(subprocesses_mutex) != 0);

    return free_index;
}

/* Add a process to the list of processes to kill at program exit. Once you
   call this function, the process handle "belongs" to it and you shouldn't
   modify the handle until you call unregister_subprocess. Returns -1 on
   error. */
static int register_subprocess(HANDLE proc)
{
    int i, rc;

    if (subprocesses_mutex == NULL) {
        subprocesses_mutex = CreateMutex(NULL, FALSE, NULL);
        assert(subprocesses_mutex != NULL);
    }

    assert(WaitForSingleObject(subprocesses_mutex, INFINITE) == WAIT_OBJECT_0);

    i = get_subprocess_slot();
    if (i == -1) {
        if (o.verbose)
            logdebug("No free process slots for termination handler.\n");
    } else {
        subprocesses[i] = proc;

        if (o.debug > 1)
            logdebug("Register subprocess %p at index %d.\n", proc, i);

        if (!atexit_registered) {
            /* We register both an atexit and a SIGINT handler because ^C
               doesn't seem to cause atexit handlers to be called. */
            atexit(terminate_subprocesses);
            signal(SIGINT, sigint_handler);
            atexit_registered = 1;
        }
    }

    assert(ReleaseMutex(subprocesses_mutex) != 0);

    return i;
}

/* Remove a process handle from the termination handler list. Returns -1 if the
   process was not already registered. */
static int unregister_subprocess(HANDLE proc)
{
    int i;

    assert(WaitForSingleObject(subprocesses_mutex, INFINITE) == WAIT_OBJECT_0);

    for (i = 0; i < subprocess_max_index; i++) {
        if (proc == subprocesses[i])
            break;
    }
    if (i < subprocess_max_index) {
        subprocesses[i] = NULL;
        if (o.debug > 1)
            logdebug("Unregister subprocess %p from index %d.\n", proc, i);
    } else {
        i = -1;
    }

    assert(ReleaseMutex(subprocesses_mutex) != 0);

    return i;
}

static void terminate_subprocesses(void)
{
    int i;

    if (o.debug)
        logdebug("Terminating subprocesses\n");

    assert(WaitForSingleObject(subprocesses_mutex, INFINITE) == WAIT_OBJECT_0);

    if (o.debug > 1)
        logdebug("max_index %d\n", subprocess_max_index);
    for (i = 0; i < subprocess_max_index; i++) {
        HANDLE proc = subprocesses[i];
        DWORD ret;

        if (proc == NULL)
            continue;
        GetExitCodeProcess(proc, &ret);
        if (ret == STILL_ACTIVE) {
            if (o.debug > 1)
                logdebug("kill index %d\n", i);
            TerminateProcess(proc, 0);
        }
        subprocesses[i] = NULL;
    }

    assert(ReleaseMutex(subprocesses_mutex) != 0);
}

static void sigint_handler(int s)
{
    terminate_subprocesses();
    ExitProcess(0);
}
