/* Test: msg2
   Description: Tests msg send/recv under two processes situation..  
   It tests all error conditions.  
*/

#include <mcapi.h>
#include <mca.h>
#include <mcapi_test.h>
#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h>
#include <mcapi_impl_spec.h>

#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_SIZES 4
#define BUFF_SIZE 64
#define DOMAIN 0
#define NODE 0


#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
  fprintf(stderr,"WRONG: line=%u\n", line);
  fflush(stdout);
  _exit(1);
}

void send (mcapi_endpoint_t send, mcapi_endpoint_t recv,char* msg,mcapi_status_t status,int exp_status) {
  int size = strlen(msg);
  int priority = 1;
  mcapi_request_t request;

  mcapi_msg_send_i(send,recv,msg,size,priority,&request,&status);
  if (status != exp_status) { WRONG}
  if (status == MCAPI_SUCCESS) {
    printf("endpoint=%i has sent: [%s]\n",(int)send,msg);
  }
}

void recv (mcapi_endpoint_t recv,mcapi_status_t status,int exp_status) {
  size_t recv_size;
  char buffer[BUFF_SIZE];
  mcapi_request_t request;
  mcapi_msg_recv_i(recv,buffer,BUFF_SIZE,&request,&status);
  if (status != exp_status) { WRONG}
  if (status == MCAPI_SUCCESS) {
    printf("endpoint=%i has received: [%s]\n",(int)recv,buffer);
  }
}

void do_child()
{
	mcapi_status_t status;
	mcapi_param_t parms;
	mcapi_info_t version;
	mcapi_request_t request;
	mcapi_endpoint_t ep1,ep2,ep3,ep4;
	char send_string[] = "HELLO MCAPI";

	mcapi_uint_t avail;
	int s;
	int i,rc;
	int fd;
	char *send_buf = malloc(64);
	if (!send_buf)
		WRONG;
	

	fd = open("/tmp/child.log", O_CREAT | O_RDWR, 0666);
	if (fd < 0)
		{ WRONG };

	if (dup2(fd, STDOUT_FILENO) == -1) {
		WRONG;
	}
	close(fd);

	printf("test log\n");

      	mcapi_initialize(DOMAIN,NODE,NULL,&parms,&version,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
          
        /* create endpoints */
        ep2 = mcapi_endpoint_create(MASTER_PORT_NUM2,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("ep2 %x   \n", ep1);
     
        ep4 = mcapi_endpoint_get (DOMAIN,SLAVE_NODE_NUM,SLAVE_PORT_NUM2,MCA_INFINITE,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("ep4 %x   \n", ep2);
     
        /* send and recv messages on the endpoints */
        /* regular endpoints */
     
        for (s = 0; s < NUM_SIZES; s++) {
	memset(send_buf, 0, 64);
        sprintf(send_buf, "CHILD %s %d", send_string, s);
        send (ep2,ep4,send_buf,status,MCAPI_SUCCESS);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("coreA: The %d time sending, status %d\n",s, status);
     
        }

#if 0
  rc = 0;
  while (1) {
	avail = mcapi_msg_available(ep2, &status);
	if (avail > 0) {
		recv (ep2,status,MCAPI_SUCCESS);
  		if (status != MCAPI_SUCCESS) { WRONG }
                printf("CoreA child : message recv. The %d time receiving ok, status %i\n", rc, status);

		if (rc == (NUM_SIZES - 1))
                	break;
                rc++;
	}
	sleep(2);
  }
#endif
	sleep(8);
        mcapi_endpoint_delete(ep2,&status);
        if (status != MCAPI_SUCCESS) { WRONG }

	mcapi_finalize(&status);

	free(send_buf);
	printf("Child Test PASSED\n");

        fflush(stdout);
	close(STDOUT_FILENO);
        _exit(0);
}

void do_parent(int pid)
{
	mcapi_status_t status;
	mcapi_param_t parms;
	mcapi_info_t version;
	mcapi_request_t request;
	mcapi_endpoint_t ep1,ep2,ep3,ep4;
	char send_string[] = "HELLO MCAPI";
        int stat_val;
	mcapi_uint_t avail;
	int s;
	int i,pass_num=0;
	int fd;
	char *send_buf0 = malloc(64);
	if (!send_buf0)
		WRONG;


	fd = open("/tmp/parent.log", O_CREAT | O_RDWR, 0666);
	if (fd < 0)
		{ WRONG };

	if (dup2(fd, STDOUT_FILENO) == -1) {
		WRONG;
	}
	close(fd);

      	mcapi_initialize(DOMAIN,NODE,NULL,&parms,&version,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
          
        /* create endpoints */
        ep1 = mcapi_endpoint_create(MASTER_PORT_NUM1,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("ep1 %x   \n", ep1);
     
        ep3 = mcapi_endpoint_get (DOMAIN,SLAVE_NODE_NUM,SLAVE_PORT_NUM1,MCA_INFINITE,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("ep3 %x   \n", ep2);
     
        /* send and recv messages on the endpoints */
        /* regular endpoints */
     
        for (s = 0; s < NUM_SIZES; s++) {
	memset(send_buf0, 0, 64);
        sprintf(send_buf0, "PARENT %s %d", send_string, s);
        send (ep1,ep3,send_buf0,status,MCAPI_SUCCESS);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("coreA: The %d time sending, status %d\n",s, status);
     
        }

#if 1
  i = 0;
  while (1) {
	avail = mcapi_msg_available(ep1, &status);
	if (avail > 0) {
		recv (ep1,status,MCAPI_SUCCESS);
  		if (status != MCAPI_SUCCESS) { WRONG }
		else {pass_num++;}
                printf("CoreA parent: message recv. The %d time receiving, status %i\n", i, status);

		if (i == (NUM_SIZES*2 - 1))
                	break;
                i++;
	}
	sleep(2);
  }
#endif

        waitpid(pid, &stat_val, 0);
	if (WIFEXITED(stat_val))
		 printf("Child exited with code %d\n",WEXITSTATUS(stat_val));
	else if (WIFSIGNALED(stat_val))
		printf("Child terminated  abnormally, signal %d\n", WTERMSIG(stat_val));

	sleep(5);
        mcapi_endpoint_delete(ep1,&status);
        if (status != MCAPI_SUCCESS) { WRONG }


	mcapi_finalize(&status);

	free(send_buf0);

	if (pass_num == NUM_SIZES*2) {
    	printf("Parent Test PASSED\n");
  	} else {
    	printf("Parent Test FAILED\n");
	_exit(1);
  	}
}

int main (int ac, char **av) {
	mcapi_status_t status;
	mcapi_param_t parms;
	mcapi_info_t version;
	mcapi_request_t request;
	mcapi_endpoint_t ep1,ep2,ep3,ep4;

	/* cases:
	1: both named endpoints (1,2)
	*/
	mcapi_sclchan_send_hndl_t s1;
	mcapi_sclchan_recv_hndl_t r1;
	mcapi_uint_t avail;
	int s;
	int i;

	int parentpid, childpid;
	char opt;

	while ( (opt = getopt(ac, av, "C")) > 0) {

 	switch (opt) {
                case 'C': /* Run child */
			do_child();
                        break;
                default:
			break;

 	}
	}

	childpid = vfork();
	if (childpid < 0) {
		WRONG
	}
	if (childpid == 0) {/* child */

        return	execlp(av[0], av[0], "-C", (char *) NULL);

	} else {/* parent */
 
	do_parent(childpid);	

	}		

    	printf("CoreA Test PASSED\n");
        fflush(stdout);
	close(STDOUT_FILENO);
	return 0;
}
