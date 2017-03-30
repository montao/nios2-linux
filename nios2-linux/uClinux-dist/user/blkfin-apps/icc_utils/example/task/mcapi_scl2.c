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
}

mcapi_boolean_t send (mcapi_sclchan_send_hndl_t send_handle, mcapi_endpoint_t recv,unsigned long long data,uint32_t size,mcapi_status_t status,int exp_status) {
  mcapi_boolean_t rc = MCAPI_FALSE;
  switch (size) {
  case (8): mcapi_sclchan_send_uint8(send_handle,data,&status); break;
  case (16): mcapi_sclchan_send_uint16(send_handle,data,&status); break;
  case (32): mcapi_sclchan_send_uint32(send_handle,data,&status); break;
  case (64): mcapi_sclchan_send_uint64(send_handle,data,&status); break;
  default: COREB_DEBUG(1, stderr,"ERROR: bad data size in call to send. size is %i\n", size);
  };
  if (status == MCAPI_SUCCESS) {
    COREB_DEBUG(1, "endpoint=%i has sent %i byte(s): [%llx]!!\n",(int)send_handle,(int)size/8,data);
  } else
  	COREB_DEBUG(1, "send failed size %d\n", size);
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
  default: COREB_DEBUG(1, "ERROR: bad data size in call to send. size is %i\n", size);
  };
 
    COREB_DEBUG(1, "eeeeeee recv scalar[%llx] exp[%llx] %d %d\n",data, exp_data, status, exp_status);
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

mcapi_boolean_t recv_sclchan1(mcapi_endpoint_t re,int size,mcapi_status_t status,int exp_status)
{
	mcapi_sclchan_recv_hndl_t r1;
	mcapi_request_t request;
	int rc;
	unsigned long long exp_data = 0x9383736353433323ULL;

	mcapi_sclchan_recv_open_i(&r1 /*recv_handle*/,re, &request, &status);
	rc = recv(r1, size, status, exp_status, exp_data);
	if (rc == MCAPI_FALSE)
		COREB_DEBUG(1, "pppppppscl recv wrong data\n");
	mcapi_sclchan_recv_close_i(r1,&request,&status);

  return rc;	

}

mcapi_boolean_t recv_sclchan2(mcapi_endpoint_t re,int size,mcapi_status_t status,int exp_status)
{
	mcapi_sclchan_recv_hndl_t r1;
	mcapi_request_t request;
	int rc;
	unsigned long long exp_data =  0x9282726252423222ULL;

	mcapi_sclchan_recv_open_i(&r1 /*recv_handle*/,re, &request, &status);
	rc = recv(r1, size, status, exp_status, exp_data);
	if (rc == MCAPI_FALSE)
		COREB_DEBUG(1, "pccccccccscl recv wrong data\n");
	mcapi_sclchan_recv_close_i(r1,&request,&status);

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
	int pass_num1=0,pass_num2=0;
	int sizes[NUM_SIZES] = {8,16,32,64};
	size_t size;
	mcapi_info_t version;
	mcapi_boolean_t rc = MCAPI_FALSE;
	mcapi_boolean_t rc1 = MCAPI_FALSE;
	mcapi_boolean_t rc2 = MCAPI_FALSE;
	uint64_t test_pattern = 0x1122334455667788ULL;

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

	ep3 = mcapi_endpoint_get (DOMAIN,MASTER_NODE_NUM,MASTER_PORT_NUM1,MCA_INFINITE,&status);
	if (status != MCAPI_SUCCESS) { WRONG }

	COREB_DEBUG(1, "mcapi sclchan test ep3 %x\n", ep3);


	while (1) {
		if (icc_wait()) {
			rc1 = recv_sclchan1(ep1, sizes[i%NUM_SIZES],status,MCAPI_SUCCESS);
			if (rc1 == MCAPI_TRUE)
				pass_num1++;

			rc2 = recv_sclchan2(ep2, sizes[i%NUM_SIZES],status,MCAPI_SUCCESS);
			if (rc2 == MCAPI_TRUE)
				pass_num2++;
			
			i++;
			if ((rc1 == MCAPI_FALSE) && (rc2 == MCAPI_FALSE)) {
                        WRONG 
			}

			if ((pass_num1 + pass_num2) == NUM_SIZES *2)
			COREB_DEBUG(0, "CoreB Test PASSED\n");

			if (i == NUM_SIZES*2 ) {
				mcapi_sclchan_connect_i(ep1,ep3,&request,&status);

				mcapi_sclchan_send_open_i(&s1,ep1, &request, &status);

				send(s1,ep3,test_pattern,64,status,MCAPI_SUCCESS);

				break;
			}

		}
	}

	mcapi_finalize(&status);

	if ((pass_num1 + pass_num2) == NUM_SIZES *2)
	COREB_DEBUG(0, "CoreB Test PASSED\n");
  	else
  	COREB_DEBUG(0, "CoreB Test FAILED\n");
	return;
}
