/* Test: pkt4
   Description: Tests simple out-of-order non-blocking pkt_channel_send and 
   pkt_channel_recv calls to two endpoints on asingle node.  This is a 
   directed test for a bug found using a pthreaded test.  Specifically this 
   test makes sure that pktchan calls in the order: recv send send recv obey FIFO.
*/

#include <mcapi.h>
#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h>
#include <assert.h>
#include <mcapi_test.h>

#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
  fprintf(stderr,"WRONG: line=%u\n", line);
  fflush(stdout);
  exit(1);
}

char send_buf[32] = "mcapi_pkt1";
const int fail = 1;
const int ok = 0;

int main () {
  size_t size;
  mcapi_endpoint_t ep1,ep2,ep3,ep4;
  mcapi_pktchan_send_hndl_t s1; /* s1 = ep1->ep3 */
  mcapi_pktchan_recv_hndl_t r1; /* r1 = ep4->ep2 */
  int i = 0;
  int rc = 1;
  mcapi_status_t status;
  mcapi_request_t request;
  mcapi_version_t version;

  /* create a node */
  mcapi_initialize(MASTER_NODE_NUM,&version,&status);
  if (status != MCAPI_SUCCESS) { WRONG }

  /* create endpoints */
  ep1 = mcapi_create_endpoint (MASTER_PORT_NUM1,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  printf("ep1 %x   \n", ep1);
  ep2 = mcapi_create_endpoint (MASTER_PORT_NUM2,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  printf("ep2 %x   \n", ep1);

  ep3 = mcapi_get_endpoint(SLAVE_NODE_NUM, SLAVE_PORT_NUM1, &status);
  if (status != MCAPI_SUCCESS) { WRONG }
  ep4 = mcapi_get_endpoint(SLAVE_NODE_NUM, SLAVE_PORT_NUM2, &status);
  if (status != MCAPI_SUCCESS) { WRONG }



  /*************************** connect the channels *********************/
  mcapi_connect_pktchan_i(ep1,ep3,&request,&status);

  /*************************** open the channels *********************/
  
  printf("open pktchan send\n");
  mcapi_open_pktchan_send_i(&s1 /*send_handle*/,ep1, &request, &status);
  printf("status %d\n", status);

  printf("pktchan send i\n");
  mcapi_pktchan_send_i(s1,send_buf,32,&request,&status);
  printf("status %d\n", status);

  printf("close pktchan send\n");
  /* close the channels */
  mcapi_pktchan_send_close_i(s1,&request,&status); 
  printf("status %d\n", status);

  mcapi_finalize(&status);
  if (rc == 0) {
    printf("   Test PASSED\n");
  } else {
    printf("   Test FAILED\n");
  }
  return rc;
}
