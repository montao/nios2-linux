/* Test: msg1
   Description: Tests simple blocking msgsend and msgrecv calls to several endpoints
   on a single node 
*/

#include <mcapi.h>
#include <mcapi_datatypes.h>
#include <mcapi_test.h>
#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h>

#define BUFF_SIZE 64

#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
  fprintf(stderr,"WRONG: line=%u\n", line);
  fflush(stdout);
  exit(1);
}

void send (mcapi_endpoint_t send, mcapi_endpoint_t recv,char* msg,mcapi_status_t status,int exp_status) {
  int size = strlen(msg);
  int priority = 1;
mcapi_request_t request;

  if (exp_status == MCAPI_EMESS_LIMIT) {
    size = MAX_MSG_SIZE+1;
  }

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

int main () {
  mcapi_status_t status;
  mcapi_version_t version;
  mcapi_endpoint_t ep1,ep2;
  int i;
  mcapi_uint_t avail;

  /* create a node */
  mcapi_initialize(MASTER_NODE_NUM,&version,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
    
  /* create endpoints */
  ep1 = mcapi_create_endpoint (MASTER_PORT_NUM1,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  printf("ep1 %x   \n", ep1);
 ep2 = mcapi_get_endpoint(SLAVE_NODE_NUM, SLAVE_PORT_NUM1, &status);
  if (status != MCAPI_SUCCESS) { WRONG }

  printf("ep2 %x   \n", ep2);
  /* send and recv messages on the endpoints */
  /* regular endpoints */
  send (ep1,ep2,"1Hello MCAPI",status,MCAPI_SUCCESS);
  
#if 1
  while (1) {
	avail = mcapi_msg_available(ep1, &status);
	if (avail > 0) {
		recv (ep1,status,MCAPI_SUCCESS);
		break;
	}
	sleep(2);
  }
#endif
  mcapi_finalize(&status);
  printf("   Test PASSED\n");
  return 0;
}
