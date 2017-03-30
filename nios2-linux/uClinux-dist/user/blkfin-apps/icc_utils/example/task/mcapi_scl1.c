/* Test: scl1
   Description: Tests scl_channel_send and scl_channel_recv calls on single node.  
   Note, that for scalar channels we only have blocking versions of send/recv.
   This test tests send/recv to several endpoints on the same node.  It tests
   all error conditions.  
*/

#include <mcapi.h>
#include <mcapi_test.h>
#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h>
#include <debug.h>

#define NUM_SIZES 4

#define DOMAIN 0

#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
COREB_DEBUG(0, "CoreB Test FAILED at line %i .\n", line);
}

mcapi_boolean_t send (mcapi_sclchan_send_hndl_t send_handle, mcapi_endpoint_t recv,unsigned long long data,uint32_t size,mcapi_status_t status,int exp_status) {
  mcapi_boolean_t rc = MCAPI_FALSE;
  switch (size) {
  case (8): mcapi_sclchan_send_uint8(send_handle,data,&status); break;
  case (16): mcapi_sclchan_send_uint16(send_handle,data,&status); break;
  case (32): mcapi_sclchan_send_uint32(send_handle,data,&status); break;
  case (64): mcapi_sclchan_send_uint64(send_handle,data,&status); break;
  default: COREB_DEBUG(1, stderr,"ERROR: bad data size in call to send\n");
  };
  if (status == MCAPI_SUCCESS) {
    COREB_DEBUG(1, "CoreB endpoint=%i has sent %i byte(s): [%llx]\n",(int)send_handle,(int)size/8,data);
  }
  if (status == exp_status) {
    rc = MCAPI_TRUE;
  }
  return rc;
}

mcapi_boolean_t recv (mcapi_sclchan_recv_hndl_t recv_handle,uint32_t size,mcapi_status_t status,uint32_t exp_status,unsigned long long exp_data) {
  unsigned long long data = 0;
  mcapi_boolean_t rc = MCAPI_FALSE;
  uint64_t size_mask; 
  switch (size) {
  case (8): size_mask = 0xff;data=mcapi_sclchan_recv_uint8(recv_handle,&status); break;
  case (16): size_mask = 0xffff;data=mcapi_sclchan_recv_uint16(recv_handle,&status); break;
  case (32): size_mask = 0xffffffff;data=mcapi_sclchan_recv_uint32(recv_handle,&status); break;
  case (64): size_mask = 0xffffffffffffffffULL;data=mcapi_sclchan_recv_uint64(recv_handle,&status); break;
  default: COREB_DEBUG(1, "ERROR: bad data size in call to send\n");
  };
 
    COREB_DEBUG(1, "CoreB has recv scalar[%llx] exp[%llx] %d %d\n",data, exp_data, status, exp_status);
  exp_data = exp_data & size_mask;
   
  if (status == exp_status) {
    rc = MCAPI_TRUE;
  }
  if (status == MCAPI_SUCCESS) {
    COREB_DEBUG(1, "endpoint=%i has received %i byte(s): [%llx]\n",(int)recv_handle,(int)size/8,data);
    if (data != exp_data) { 
       rc = MCAPI_FALSE; 
     }
  }
 
  return rc;
}

void icc_task_init(int argc, char *argv[])
{
	mcapi_status_t status;
	mcapi_request_t request;
	mcapi_param_t parms;
	mcapi_endpoint_t ep1,ep2,ep3;

	/* cases:
1: both named endpoints (1,2)
*/
	mcapi_sclchan_send_hndl_t s1;
	mcapi_sclchan_recv_hndl_t r1;
	mcapi_uint_t avail;
	int s;
	int i = 0;
	int pass_num = 0;
	int sizes[NUM_SIZES] = {8,16,32,64};
	size_t size;
	mcapi_info_t version;
	mcapi_boolean_t rc = MCAPI_FALSE;
	uint64_t test_pattern = 0x1122334455667788ULL;
	unsigned long long exp_data = 0x1122334455667788ULL;

	COREB_DEBUG(1, "[%s] %d\n", __func__, __LINE__);

	/* create a node */
	mcapi_initialize(DOMAIN,SLAVE_NODE_NUM,NULL,&parms,&version,&status);
	if (status != MCAPI_SUCCESS) { WRONG }
	COREB_DEBUG(1, "[%s] %d\n", __func__, __LINE__);

	/* create endpoints */
	ep1 = mcapi_endpoint_create(SLAVE_PORT_NUM1,&status);
	if (status != MCAPI_SUCCESS) { WRONG }
	COREB_DEBUG(1, "mcapi sclchan test ep1 %x\n", ep1);

	ep2 = mcapi_endpoint_create(SLAVE_PORT_NUM2,&status);
	if (status != MCAPI_SUCCESS) { WRONG }
	COREB_DEBUG(1, "mcapi sclchan test ep2 %x\n", ep2);

	ep3 = mcapi_endpoint_get (DOMAIN,MASTER_NODE_NUM,MASTER_PORT_NUM2,MCA_INFINITE,&status);
	if (status != MCAPI_SUCCESS) { WRONG }

	COREB_DEBUG(1, "mcapi sclchan test ep3 %x\n", ep3);

	/****CoreB to receive open scalar chan ***********/

	mcapi_sclchan_recv_open_i(&r1 /*recv_handle*/,ep1, &request, &status);

	while (1) {
		if (icc_wait()) {
			rc = recv(r1, sizes[i++], status, MCAPI_SUCCESS, exp_data);
			if (rc == MCAPI_FALSE)
			{	COREB_DEBUG(1, "scl recv wrong data\n");
				wrong;
			}else if (rc == MCAPI_TRUE)
			{pass_num++;}		
			if ( i == NUM_SIZES )
			break;
		}
	}

	mcapi_sclchan_recv_close_i(r1,&request,&status);

	/****CoreB to receive scalar ended. ***********/


	/****CoreB start to send scalar data back to CoreA. ***********/
	i = 0;
        pass_num = 0;
	mcapi_sclchan_connect_i(ep2,ep3,&request,&status);
	COREB_DEBUG(1, "CoreB connect %x\n", ep2);
	mcapi_sclchan_send_open_i(&s1,ep2, &request, &status);
	COREB_DEBUG(1, "CoreB scalar send open %x\n", ep2);

	while (1) {
			rc = send(s1,ep3,test_pattern,sizes[i++],status,MCAPI_SUCCESS);
			if (rc == MCAPI_FALSE)
			{	COREB_DEBUG(1, "scl send data fail\n");
				wrong;
			}else {pass_num++;}		

			if ( i == NUM_SIZES ){
			break;}
	}

	mcapi_sclchan_send_close_i(s1,&request,&status);
	COREB_DEBUG(1, "CoreB Send Close PASSED\n");

	if (pass_num == NUM_SIZES)
	COREB_DEBUG(0, "CoreB Test PASSED\n");

	/****CoreB to send scalar ended. ***********/
	while(1)
		icc_wait();

	mcapi_endpoint_delete(ep1,&status);
	mcapi_endpoint_delete(ep2,&status);

	mcapi_finalize(&status);

	if (pass_num == NUM_SIZES)
	COREB_DEBUG(0, "CoreB Test PASSED\n");
  	else
  	COREB_DEBUG(0, "CoreB Test FAILED\n");
	return;
}
