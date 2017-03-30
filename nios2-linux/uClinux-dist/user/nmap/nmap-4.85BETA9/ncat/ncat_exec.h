/* fork and exec a child process with netexec. Close the given file descriptor
   in the parent process. Return the child's PID or -1 on error. */
extern int netrun(int fd, char *cmdexec);

/* exec the given command line. Before the exec, redirect stdin, stdout, and
   stderr to the given file descriptor. Never returns. */
extern void netexec(int fd, char *cmdexec);
