#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/syscall.h>

#ifndef __NR_gettid
#error "can't find gettid syscall number"
#endif



void *report(void *foo)
{
	pid_t tid;

	tid = (pid_t)syscall(__NR_gettid);
	printf("thread: id = %d\n", tid);
	sleep(5);
}

int main()
{
	pthread_t thread_id;
	int status;
	
	status = pthread_create(&thread_id, NULL, report, NULL);

	if (status != 0) {
		fprintf(stderr, "pthread_create failed: %s\n", 
			strerror(status));
		exit(-1);
	}

	printf("main: pid = %d\n", getpid());
	sleep(5);
}
		
