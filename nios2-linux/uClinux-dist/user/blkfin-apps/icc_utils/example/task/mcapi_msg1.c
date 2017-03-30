/* Test: msg1
   Description: Tests simple blocking msgsend and msgrecv calls to several endpoints
   on a single node 
*/

#include <mcapi.h>
#include <mcapi_test.h>
#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h>
#include <debug.h>

#define BUFF_SIZE 64
#define NUM_SIZES 4
#define WRONG wrong(__LINE__);

#define DOMAIN 0

char buffer[BUFF_SIZE];

void wrong(unsigned line)
{
COREB_DEBUG(1, "WRONG: line==%i \n",line);
}

void send (mcapi_endpoint_t send, mcapi_endpoint_t recv,char* msg,mcapi_status_t status,int exp_status) {
  int size = strlen(msg);
  int priority = 1;
  mcapi_request_t request;

  mcapi_msg_send_i(send,recv,msg,size,priority,&request,&status);
  if (status != exp_status) { WRONG}
  if (status == MCAPI_SUCCESS) {
    COREB_DEBUG(1, "endpoint=%i has sent: [%s]\n",(int)send,msg);
  }
}

void recv_loopback (mcapi_endpoint_t recv,mcapi_status_t status,int exp_status) {
  size_t recv_size;
  mcapi_request_t request1;
  mcapi_request_t request2;
  mcapi_endpoint_t send_back;
  mcapi_msg_recv_i(recv,buffer,BUFF_SIZE,&request1,&status);
  if (status != exp_status) { WRONG}
  if (status == MCAPI_SUCCESS) {
    COREB_DEBUG(1, "endpoint=%i has received: [%s]\n",(int)recv,buffer);
  }

  send_back = mcapi_endpoint_get(DOMAIN,MASTER_NODE_NUM, MASTER_PORT_NUM1, MCA_INFINITE, &status);

  COREB_DEBUG(1, "endpoint=%i sendback: buf %x\n",(int)send_back, (unsigned int)buffer);
  mcapi_msg_send_i(recv,send_back,buffer,BUFF_SIZE,1,&request2,&status);
  if (status != exp_status) { WRONG}
  if (status == MCAPI_SUCCESS) {
    COREB_DEBUG(1, "endpoint=%i has sent: [%s]\n",(int)recv,buffer);
  }

}

void icc_task_init(int argc, char *argv[]) {
  mcapi_status_t status;
  mcapi_info_t version;
  mcapi_param_t parms;
  mcapi_endpoint_t ep1,ep2;
  int i ,pass_num=0 ;
  mcapi_uint_t avail;

  COREB_DEBUG(1, "[%s] %d\n", __func__, __LINE__);
  /* create a node */
  mcapi_initialize(DOMAIN,SLAVE_NODE_NUM,NULL,&parms,&version,&status);

  if (status != MCAPI_SUCCESS) { WRONG }
  COREB_DEBUG(1, "[%s] %d\n", __func__, __LINE__);

  /* create endpoints */
  ep1 = mcapi_endpoint_create(SLAVE_PORT_NUM1,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  COREB_DEBUG(1, "ep1 %x   \n", ep1);
  /* send and recv messages on the endpoints */
 
i = 0;  

while(1) {
	if (icc_wait()) {
		recv_loopback(ep1,status,MCAPI_SUCCESS);
		if (status != MCAPI_SUCCESS) { WRONG }
		else {pass_num++;}
		COREB_DEBUG(1, "\nCoreB: mcapi message loop test. The %i time send back, status %i . \n", i, status);
		i++;
		if (i == NUM_SIZES)
  		COREB_DEBUG(1, "CoreB finished data loop back! %d \n",pass_num);
  		if (pass_num == NUM_SIZES)
  		COREB_DEBUG(0, "CoreB Test PASSED\n");
	}
}

  mcapi_endpoint_delete(ep1,&status);

  mcapi_finalize(&status);
  if (pass_num == NUM_SIZES)
  COREB_DEBUG(0, "CoreB Test PASSED\n");
  else
  COREB_DEBUG(0, "CoreB Test FAILED\n");
  return; 
}
