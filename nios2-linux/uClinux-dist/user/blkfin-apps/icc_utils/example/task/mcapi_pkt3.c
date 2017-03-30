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

#define NUM_SIZES 2
#define RECV_BUFF_SIZE 128*1024
#define SEND_BUFF_SIZE 128

#define DOMAIN 0

struct message_hdr {
	uint32_t magic;
	uint32_t vaddr;
	uint32_t paddr;
	uint32_t size;
	uint32_t op;
};

char buffer[SEND_BUFF_SIZE] = "mcapi_pkt response";

#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
	COREB_DEBUG(1, "WRONG: line==%i \n",line);
}

void recv_pktchan(mcapi_endpoint_t recv,mcapi_status_t *status)
{
	size_t recv_size;
	mcapi_request_t request1;
	mcapi_request_t request2;
	mcapi_endpoint_t send_back;
	mcapi_status_t status1;
	mcapi_pktchan_recv_hndl_t r1;
	char *pbuffer = NULL;
	struct message_hdr *ph;
	uint32_t vaddr, paddr, size, op;
	char *p;
	int i;

	mcapi_pktchan_recv_open_i(&r1,recv, &request1, status);
	COREB_DEBUG(1, "open recv chan status %x   \n", *status);

	mcapi_pktchan_recv_i(r1,(void **)&pbuffer,&request1,status);
	if (*status == MCAPI_SUCCESS) {
		COREB_DEBUG(1, "endpoint=%i has received!! \n",(int)recv);

		ph = pbuffer;
		if (ph->magic != 0xAA55)
			COREB_DEBUG(1, "buffer magic is wrong\n");
		vaddr = ph->vaddr;
		paddr = ph->paddr;
		size = ph->size;
		op = ph->op;

		p = (char *)paddr;

		COREB_DEBUG(1, "##%08x %08x %08x %08x\n", vaddr, paddr, size, op);
		if (op == 1) {
			COREB_DEBUG(1, "processing buffer\n");
			for(i = 0; i < size; i++)
				p[i] = p[i] + 1;
		}

		if (pbuffer)
			mcapi_pktchan_release(pbuffer, &status1);
		mcapi_pktchan_recv_close_i(r1,&request1, &status1);
	}
}

void send_pktchan(mcapi_endpoint_t send,mcapi_status_t status,int exp_status)
{
	mcapi_request_t request1;
	mcapi_request_t request2;
	mcapi_endpoint_t send_back;
	mcapi_pktchan_send_hndl_t s1;
	size_t send_size;

	/*************************** open the channels *********************/

	COREB_DEBUG(1, "open pktchan send\n");
	mcapi_pktchan_send_open_i(&s1,send, &request1, &status);
	COREB_DEBUG(1, "open send chan status %x   \n", status);

	mcapi_pktchan_send_i(s1,buffer,SEND_BUFF_SIZE,&request1,&status);
	if (status != exp_status) { WRONG}
	if (status == MCAPI_SUCCESS) {
		COREB_DEBUG(1, "endpoint=%i has sent: [%s]\n",(int)send,buffer);
		mcapi_pktchan_send_close_i(s1,&request1, &status);
	}
}


void icc_task_init(int argc, char *argv[])
{
  	mcapi_status_t status,status1,status2;
  	mcapi_info_t version;
  	mcapi_param_t parms;
	mcapi_endpoint_t ep1,ep2,ep3,ep4;
	int i=0;
	int pass_num1=0,pass_num2=0;
	mcapi_uint_t avail;
	mcapi_request_t request;

	COREB_DEBUG(1, "[%s] %d\n", __func__, __LINE__);
	/* create a node */
  	mcapi_initialize(DOMAIN,SLAVE_NODE_NUM,NULL,&parms,&version,&status);
	if (status != MCAPI_SUCCESS) { WRONG }
	COREB_DEBUG(1, "[%s] %d\n", __func__, __LINE__);

	/* create endpoints */
  	ep1 = mcapi_endpoint_create(SLAVE_PORT_NUM1,&status);
	if (status != MCAPI_SUCCESS) { WRONG }
	COREB_DEBUG(1, "mcapi pktchan test ep1 %x\n", ep1);

	ep2 = mcapi_endpoint_create(SLAVE_PORT_NUM2,&status);
	if (status != MCAPI_SUCCESS) { WRONG }
	COREB_DEBUG(1, "mcapi pktchan test ep2 %x\n", ep2);

	ep3 = mcapi_endpoint_get (DOMAIN,MASTER_NODE_NUM,MASTER_PORT_NUM1,MCA_INFINITE,&status);
	if (status != MCAPI_SUCCESS) { WRONG }

	ep4 = mcapi_endpoint_get (DOMAIN,MASTER_NODE_NUM,MASTER_PORT_NUM2,MCA_INFINITE,&status);
	if (status != MCAPI_SUCCESS) { WRONG }


	COREB_DEBUG(1, "mcapi pktchan test ep3 %x\n", ep3);

	i = 0;
	while (1) {
		if (icc_wait()) {
			recv_pktchan(ep1,&status1);
                        if (status1 == MCAPI_SUCCESS)
        	                 pass_num1++;

			recv_pktchan(ep2,&status2);
                        if (status2 == MCAPI_SUCCESS)
        	                 pass_num2++;

			if ((status1 != MCAPI_SUCCESS) && (status2 != MCAPI_SUCCESS)) {
			       WRONG
			}

			COREB_DEBUG(1, "\nCoreB: mcapi pktchan test. The %i time send back,status1 %d, status2 %d . \n", i, status1,status2);

			if (i == 0) {
				mcapi_pktchan_connect_i(ep1,ep3,&request,&status);
				if (status != MCAPI_SUCCESS) { WRONG }
				mcapi_pktchan_connect_i(ep2,ep4,&request,&status);
				if (status != MCAPI_SUCCESS) { WRONG }
			}

	                i++;
      			if ( i == NUM_SIZES*2) {
				send_pktchan(ep1,status,MCAPI_SUCCESS);
				send_pktchan(ep2,status,MCAPI_SUCCESS);
			}

			if ((pass_num1 + pass_num2) == NUM_SIZES *2 - 1 )
			COREB_DEBUG(0, "CoreB Test PASSED\n");
		}
	}

	mcapi_endpoint_delete(ep1,&status);
	mcapi_endpoint_delete(ep2,&status);

	mcapi_finalize(&status);
	if ((pass_num1 + pass_num2) == NUM_SIZES *2 - 1 )
	COREB_DEBUG(0, "CoreB Test PASSED\n");
  	else
  	COREB_DEBUG(0, "CoreB Test FAILED\n");
	return;
}
