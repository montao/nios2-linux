#include <icc.h>
#include <protocol.h>
#include <debug.h>

#define LOCAL_SESSION 5

uint32_t __icc_task_data session_index;
void  __icc_task icc_task_init(int argc, char *argv[])
{
	struct sm_session *session;
	uint32_t scalar0 = 0, scalar1 = 0, size = 0;
	int ret;
	int src_ep, src_cpu;
	session_index = sm_create_session(LOCAL_SESSION, SP_SCALAR);
	COREB_DEBUG(1, "%s() %s %s index %d\n", __func__, argv[0], argv[1], session_index);
	if (session_index >= 32)
		COREB_DEBUG(1, "create session failed\n");

	while (1) {
		COREB_DEBUG(1, "task loop\n");
		if (icc_wait()) {
			ret = sm_recv_scalar(session_index, &src_ep, &src_cpu, &scalar0, &scalar1, &size);
			if (ret <= 0) {
				COREB_DEBUG(1, "recv packet failed\n");
			}
			/* handle payload */
			COREB_DEBUG(1, "processing msg %x %x\n", scalar0, scalar1);
			if (scalar0  == 0xBAD) {
				int dst_ep = src_ep;
				int dst_cpu = src_cpu;
				scalar0 = 0xBAD;
				scalar1 = bfin_read32(TIMER1_COUNTER);
				COREB_DEBUG(1, "get timer %x\n", scalar1);
				sm_send_scalar(session_index, dst_ep, dst_cpu, scalar0, scalar1, 8);
			}

		}

	}

	COREB_DEBUG(1, "%s() end\n", __func__);
}

void  __icc_task icc_task_exit(void)
{
	sm_destroy_session(session_index);
}

