/* Test: msg1
   Description: Tests simple blocking msgsend and msgrecv calls to several endpoints
   on a single node 
*/

#include <mcapi.h>
#include <mca.h>
#include <mcapi_test.h>
#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h>
#include <mcapi_impl_spec.h>


#define NUM_SIZES 4
#define BUFF_SIZE 256
#define DOMAIN 0
#define NODE 0

#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
  fprintf(stderr,"WRONG: line=%u\n", line);
  fflush(stdout);
  _exit(1);
}
unsigned int cmd;
char dsp_data[128];

void recv (mcapi_endpoint_t recv,mcapi_status_t status,int exp_status) {
  size_t recv_size;
  char buffer[BUFF_SIZE];
  mcapi_request_t request;
  mcapi_msg_recv_i(recv,buffer,BUFF_SIZE,&request,&status);
  if (status != exp_status) { WRONG}
  if (status == MCAPI_SUCCESS) {
  int i;
  printf("DSP data after precessing\n");
  for (i = 0; i < 128; i++)
 	printf("%x", buffer[i]);
  }
}

int main () {
  mcapi_status_t status;
  mcapi_param_t parms;
  mcapi_info_t version;
  mcapi_endpoint_t ep1,ep2,ep3,ep4;
  int i,s = 0, rc = 0,pass_num=0;
  mcapi_uint_t avail;
  mcapi_request_t request;

  /* create a node */
  mcapi_initialize(DOMAIN,NODE,NULL,&parms,&version,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
    
  /* create endpoints */
  ep1 = mcapi_endpoint_create(MASTER_PORT_NUM1,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  printf("ep1 %x   \n", ep1);

  ep4 = mcapi_endpoint_create(2,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  printf("ep4 %x   \n", ep4);


  ep2 = mcapi_endpoint_get (DOMAIN,SLAVE_NODE_NUM,0,MCA_INFINITE,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  printf("ep2 %x   \n", ep2);

  ep3 = mcapi_endpoint_get (DOMAIN,SLAVE_NODE_NUM,1,MCA_INFINITE,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  printf("ep3 %x   \n", ep3);


  /* send and recv messages on the endpoints */
  /* regular endpoints */

  /* send DSP command */
  cmd = 1;

  mcapi_msg_send_i(ep1,ep2,&cmd,4,1,&request,&status);
  if (status != MCAPI_SUCCESS) { WRONG}
  if (status == MCAPI_SUCCESS) {
    printf("endpoint=%i has sent: [%x]\n",(int)ep1,cmd);
  }

  for (i = 0; i < 128; i++)
	  dsp_data[i] = i + 30;


  printf("DSP data before\n");
  for (i = 0; i < 128; i++)
 	printf("%x", dsp_data[i]);

  /* send DSP data */
  mcapi_msg_send_i(ep1,ep3,dsp_data,128,1,&request,&status);
  if (status != MCAPI_SUCCESS) { WRONG}
  if (status == MCAPI_SUCCESS) {
    printf("endpoint=%i has sent: [%x]\n",(int)ep1,cmd);
  }


  /* send DSP command process dataa*/
  cmd = 4;

  mcapi_msg_send_i(ep1,ep2,&cmd,4,1,&request,&status);
  if (status != MCAPI_SUCCESS) { WRONG}
  if (status == MCAPI_SUCCESS) {
    printf("endpoint=%i has sent: [%x]\n",(int)ep1,cmd);
  }


  sleep(1);

  /* recv DSP data */
 recv(ep4, &status, MCAPI_SUCCESS);

  mcapi_endpoint_delete(ep1,&status);

  mcapi_finalize(&status);

  printf("CoreA Test PASSED\n");

  return 0;
}
