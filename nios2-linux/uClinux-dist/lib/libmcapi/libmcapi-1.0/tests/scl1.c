/* Test: scl1
   Description: Tests scl_channel_send and scl_channel_recv calls on single node.  
   Note, that for scalar channels we only have blocking versions of send/recv.
   This test tests send/recv to several endpoints on the same node.  It tests
   all error conditions.  
*/

#include <mcapi.h>
#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h>
#include <mcapi_test.h>

#define NUM_SIZES 4 

#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
  fprintf(stderr,"WRONG: line=%u\n", line);
  fflush(stdout);
  exit(1);
}

mcapi_boolean_t send (mcapi_sclchan_send_hndl_t send_handle, mcapi_endpoint_t recv,unsigned long long data,uint32_t size,mcapi_status_t status,int exp_status) {
  mcapi_boolean_t rc = MCAPI_FALSE;
  switch (size) {
  case (8): mcapi_sclchan_send_uint8(send_handle,data,&status); break;
  case (16): mcapi_sclchan_send_uint16(send_handle,data,&status); break;
  case (32): mcapi_sclchan_send_uint32(send_handle,data,&status); break;
  case (64): mcapi_sclchan_send_uint64(send_handle,data,&status); break;
  default: fprintf (stderr,"ERROR: bad data size in call to send\n");
  };
  if (status == MCAPI_SUCCESS) {
    fprintf(stderr,"endpoint=%i has sent %i byte(s): [%llu]\n",(int)send_handle,(int)size/8,data);
  }
  if (status == exp_status) {
    rc = MCAPI_TRUE;
  }
  return rc;
}

mcapi_boolean_t recv (mcapi_sclchan_recv_hndl_t recv_handle,uint32_t size,mcapi_status_t status,uint64_t exp_status,unsigned long long exp_data) {
  unsigned long long data = 0;
  mcapi_boolean_t rc = MCAPI_FALSE;
  uint64_t size_mask; 
  switch (size) {
  case (8): size_mask = 0xff;data=mcapi_sclchan_recv_uint8(recv_handle,&status); break;
  case (16): size_mask = 0xffff;data=mcapi_sclchan_recv_uint16(recv_handle,&status); break;
  case (32): size_mask = 0xffffffff;data=mcapi_sclchan_recv_uint32(recv_handle,&status); break;
  case (64): size_mask = 0xffffffffffffffffULL;data=mcapi_sclchan_recv_uint64(recv_handle,&status); break;
  default: fprintf (stderr,"ERROR: bad data size in call to send\n");
  };
 
  exp_data = exp_data & size_mask;
   
  if (status == exp_status) {
    rc = MCAPI_TRUE;
  }
  if (status == MCAPI_SUCCESS) {
	  fprintf(stderr,"endpoint=%i has received %i byte(s): [%llu]\n",(int)recv_handle,(int)size/8,data);
	  if (data != exp_data) { 
		  fprintf(stderr, "expected %lld, received %lld\n",exp_data,data); 
		  rc = MCAPI_FALSE; 
	  }
  }

  return rc;
}

int main () {
	mcapi_status_t status;
	mcapi_request_t request;
	mcapi_endpoint_t ep1,ep2;

	/* cases:
	1: both named endpoints (1,2)
	*/
	mcapi_sclchan_send_hndl_t s1;
	mcapi_sclchan_recv_hndl_t r1;
	mcapi_uint_t avail;
	int s;
	int i;
	int sizes[NUM_SIZES] = {8,16,32,64};
	uint64_t test_pattern = 0x1122334455667788ULL;
	size_t size;
	mcapi_version_t version;
	mcapi_boolean_t rc = MCAPI_FALSE;

	/* create a node */
	mcapi_initialize(MASTER_NODE_NUM,&version,&status);
	if (status != MCAPI_SUCCESS) { WRONG }

	/* create endpoints */
	ep1 = mcapi_create_endpoint (MASTER_PORT_NUM1,&status);
	if (status != MCAPI_SUCCESS) { WRONG }
	printf("ep1 %x   \n", ep1);

	ep2 = mcapi_get_endpoint(SLAVE_NODE_NUM, SLAVE_PORT_NUM1, &status);
	if (status != MCAPI_SUCCESS) { WRONG }


	/*************************** connect the channels *********************/
	mcapi_connect_sclchan_i(ep1,ep2,&request, &status);
	if (status != MCAPI_SUCCESS) { WRONG }


	/*************************** open the channels *********************/
	mcapi_open_sclchan_send_i(&s1 /*send_handle*/,ep1, &request, &status);
	if (status != MCAPI_SUCCESS) { WRONG }

	sleep(1);

	/* test send/recv of different sizes */
	for (s = 0; s < NUM_SIZES; s++) {
		size = sizes[s];
		/* send and recv messages on the channels */
		/* regular endpoints */
		rc = send (s1,ep2,test_pattern,64,status,MCAPI_SUCCESS);
		if (!rc) {WRONG}
		break;
	}

	mcapi_sclchan_send_close_i(s1,&request,&status); 
	mcapi_finalize(&status);

	printf("   Test PASSED\n");
	return 0;
}
