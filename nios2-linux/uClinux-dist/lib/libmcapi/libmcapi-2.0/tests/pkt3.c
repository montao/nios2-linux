/* Test: pkt2
   Description: Tests pkt transition with two processes on at the same time.  
   This test tests send/recv to several endpoints on the same node.  It tests
   all error conditions.  
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

#define NUM_SIZES 2
#define BUFF_SIZE 128*1024
#define DOMAIN 0
#define NODE 0

#define INDIRECT_BUF_MAGIC 0xaa55

#define ADD_OP 1

#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
  fprintf(stderr,"WRONG: line=%u\n", line);
  fflush(stdout);
  _exit(1);
}

struct message_hdr {
	uint32_t magic;
	uint32_t vaddr;
	uint32_t paddr;
	uint32_t size;
	uint32_t op;
};

void do_child()
{
	mcapi_status_t status;
	mcapi_param_t parms;
	mcapi_info_t version;
	mcapi_request_t request;
	mcapi_endpoint_t ep1,ep2,ep3,ep4;
	mcapi_sclchan_send_hndl_t s1;
	mcapi_boolean_t rc = MCAPI_FALSE;
        mcapi_sclchan_recv_hndl_t r1;
	char send_string[] = "mcapi_pkt_3_process";
        void *pbuffer = NULL;
	struct message_hdr send_msg;

	mcapi_uint_t avail;
	int s;
	int i,pass_num=0;
	int fd;
	char *send_buf[NUM_SIZES];
	char *save_buf[NUM_SIZES];
	uint32_t paddr;

	fd = open("/tmp/child.log", O_CREAT | O_RDWR, 0666);
	if (fd < 0)
		{ WRONG };

	if (dup2(fd, STDOUT_FILENO) == -1) {
		WRONG;
	}
	close(fd);

	printf("test log\n");


	/* create a node */
        mcapi_initialize(DOMAIN,NODE,NULL,&parms,&version,&status);
        if (status != MCAPI_SUCCESS) { WRONG }

        /* create endpoints */
        ep2 = mcapi_endpoint_create(MASTER_PORT_NUM2,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("ep1 %x   \n", ep1);

        ep4 = mcapi_endpoint_get(DOMAIN,SLAVE_NODE_NUM, SLAVE_PORT_NUM2,MCA_INFINITE, &status);
        if (status != MCAPI_SUCCESS) { WRONG }


        /*************************** connect the channels *********************/
        mcapi_pktchan_connect_i(ep2,ep4,&request,&status);


        /*************************** open the channels *********************/
        printf("open pktchan send\n");
        mcapi_pktchan_send_open_i(&s1 /*send_handle*/,ep2, &request, &status);
        printf("status %d\n", status);

        for (s = 0; s < NUM_SIZES; s++) {
	send_buf[s] = sm_request_uncached_buf(BUFF_SIZE, &paddr);
	if (!send_buf[s])
		WRONG;
	send_msg.magic = INDIRECT_BUF_MAGIC;
	send_msg.vaddr = send_buf[s];
	send_msg.paddr = paddr;
	send_msg.size = BUFF_SIZE;
	send_msg.op = ADD_OP;

	save_buf[s] = malloc(BUFF_SIZE);
	if (!save_buf[s])
		WRONG;
  	memset(send_buf[s], 0x20 + s, BUFF_SIZE);
  	memset(save_buf[s], 0x20 + s + 1, BUFF_SIZE);
  	mcapi_pktchan_send_i(s1,&send_msg, sizeof(send_msg),&request,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("coreA child: The %d time sending, status %d\n",s, status);
        }

        printf("close pktchan send\n");
        /* close the channels */
        mcapi_pktchan_send_close_i(s1,&request,&status); 
        printf("status %d\n", status);
	
# if 1 

  mcapi_pktchan_recv_open_i(&r1 /*recv_handle*/,ep2, &request, &status);
  printf("status %d\n", status);

  while (1) {
	  avail = mcapi_pktchan_available(r1, &status);
	  if (avail > 0) {
		  mcapi_pktchan_recv_i(r1,(void **)&pbuffer,&request,&status);
  		  if (status != MCAPI_SUCCESS) { WRONG }
		  printf("Child: CoreA pktchan recv buffer %s, The status %i\n",pbuffer, status);

                  for (s = 0; s < NUM_SIZES; s++) {
			  rc=memcmp(send_buf[s],save_buf[s],BUFF_SIZE);
			  if (rc) { 
				printf("buffer addr %08x\n", send_buf[s]);
			  WRONG }
			  else {
				printf("Child: the %d time data sent back from coreB are same with that from CoreA! \n",s);
		  		pass_num++;}
	         }

		  mcapi_pktchan_release(pbuffer, &status);
		  break;
	  }
	  sleep(2);
  }
  mcapi_pktchan_recv_close_i(r1,&request,&status); 


#endif
        mcapi_endpoint_delete(ep2,&status);
        if (status != MCAPI_SUCCESS) { WRONG }

        for (s = 0; s < NUM_SIZES; s++) {
		sm_release_uncached_buf(send_buf[s], BUFF_SIZE, 0);
		free(save_buf[s]);
	}

	mcapi_finalize(&status);
	if (pass_num == NUM_SIZES) {
    	printf("Child Test PASSED\n");
  	} else {
    	printf("Child Test FAILED\n");
	_exit(1);
	}
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
	mcapi_sclchan_send_hndl_t s0;
        mcapi_sclchan_recv_hndl_t r0;
	mcapi_boolean_t rc = MCAPI_FALSE;
	char send_string[] = "mcapi_pkt_3_process";
	int stat_val;
        void *pbuffer = NULL;
	mcapi_uint_t avail;
	int s;
	int i,pass_num=0;
	int fd;
	char *send_buf[NUM_SIZES];
	char *save_buf[NUM_SIZES];
	uint32_t paddr;
	struct message_hdr send_msg;

	fd = open("/tmp/parent.log", O_CREAT | O_RDWR, 0666);
	if (fd < 0)
		{ WRONG };

	if (dup2(fd, STDOUT_FILENO) == -1) {
		WRONG;
	}
	close(fd);

	/* create a node */
        mcapi_initialize(DOMAIN,NODE,NULL,&parms,&version,&status);
        if (status != MCAPI_SUCCESS) { WRONG }

        /* create endpoints */
        ep1 = mcapi_endpoint_create(MASTER_PORT_NUM1,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("ep1 %x   \n", ep1);

        ep3 = mcapi_endpoint_get(DOMAIN,SLAVE_NODE_NUM, SLAVE_PORT_NUM1,MCA_INFINITE, &status);
        if (status != MCAPI_SUCCESS) { WRONG }


        /*************************** connect the channels *********************/
        mcapi_pktchan_connect_i(ep1,ep3,&request,&status);


        /*************************** open the channels *********************/
        printf("open pktchan send\n");
        mcapi_pktchan_send_open_i(&s0 /*send_handle*/,ep1, &request, &status);
        printf("status %d\n", status);

        for (s = 0; s < NUM_SIZES; s++) {
	send_buf[s] = sm_request_uncached_buf(BUFF_SIZE, &paddr);
	if (!send_buf[s])
		WRONG;

	send_msg.magic = INDIRECT_BUF_MAGIC;
	send_msg.vaddr = send_buf[s];
	send_msg.paddr = paddr;
	send_msg.size = BUFF_SIZE;
	send_msg.op = ADD_OP;
	save_buf[s] = malloc(BUFF_SIZE);
	if (!save_buf[s])
		WRONG;
  	memset(send_buf[s], 0x10 + s, BUFF_SIZE);
  	memset(save_buf[s], 0x10 + s + 1, BUFF_SIZE);
  	mcapi_pktchan_send_i(s0, &send_msg, sizeof(send_msg),&request,&status);
        if (status != MCAPI_SUCCESS) { WRONG }
        printf("coreA parent: The %d time sending, status %d\n",s, status);

        }

        printf("close pktchan send\n");
        /* close the channels */
        mcapi_pktchan_send_close_i(s0,&request,&status); 
        printf("status %d\n", status);
	
# if 1 

  mcapi_pktchan_recv_open_i(&r0 /*recv_handle*/,ep1, &request, &status);
  printf("status %d\n", status);

  while (1) {
	  avail = mcapi_pktchan_available(r0, &status);
	  if (avail > 0) {
		  mcapi_pktchan_recv_i(r0,(void **)&pbuffer,&request,&status);
  		  if (status != MCAPI_SUCCESS) { WRONG }
		  printf("Parent: CoreA pktchan recv buffer %s, The status %i\n",pbuffer, status);

                  for (s = 0; s < NUM_SIZES; s++) {
			  rc=memcmp(send_buf[s],save_buf[s],BUFF_SIZE);
			  if (rc) {
				printf("buffer addr %08x\n", send_buf[s]);
			  WRONG }
			  else {
			  	printf("Parent: the %d time data sent back from coreB are same with that from CoreA! ",s);
		  		pass_num++;}
	       	}

		  mcapi_pktchan_release(pbuffer, &status);
		  break;
	  }
	  sleep(2);
  }
  mcapi_pktchan_recv_close_i(r0,&request,&status); 


#endif

	 waitpid(pid, &stat_val, 0);
	if (WIFEXITED(stat_val))
		 printf("Child exited with code %d\n",WEXITSTATUS(stat_val));
	else if (WIFSIGNALED(stat_val))
		printf("Child terminated  abnormally, signal %d\n", WTERMSIG(stat_val));

        mcapi_endpoint_delete(ep1,&status);
        if (status != MCAPI_SUCCESS) { WRONG }

        for (s = 0; s < NUM_SIZES; s++) {
    		printf("* Parent Test release buf\n");
		sm_release_uncached_buf(send_buf[s], BUFF_SIZE, 0);
		free(save_buf[s]);
	}

	mcapi_finalize(&status);
	if (pass_num == NUM_SIZES) {
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

	/* cases:
	1: both named endpoints (1,2)
	*/
	mcapi_uint_t avail;
	int s;
	int i;
	size_t size;
	mcapi_boolean_t rc = MCAPI_FALSE;

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
