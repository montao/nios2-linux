#include <icc.h>
#include <protocol.h>
#include <debug.h>

#define LOCAL_SESSION 5

uint32_t __icc_task_data session_index;
void  icc_task_init(int argc, char *argv[])
{
	struct sm_session *session;
	void *buf;
	int len;
	int ret;
	int src_ep, src_cpu;
	session_index = sm_create_session(LOCAL_SESSION, SP_PACKET);
	COREB_DEBUG(1, "%s() %s %s index %d\n", __func__, argv[0], argv[1], session_index);
	if (session_index >= 32)
		COREB_DEBUG(1, "create session failed\n");

	while (1) {
		COREB_DEBUG(1, "task loop\n");
		if (icc_wait()) {
			ret = sm_recv_packet(session_index, &src_ep, &src_cpu, &buf, len);
			if (ret <= 0) {
				COREB_DEBUG(1, "recv packet failed\n");
			}
			/* handle payload */
			COREB_DEBUG(1, "processing msg %s\n", buf);
			if (*(char *)buf  == '1') {
				int len = 64;
				int dst_ep = src_ep;
				int dst_cpu = src_cpu;
				void *send_buf = sm_send_request(len, session_index);
				COREB_DEBUG(1, "coreb send buf %x\n", send_buf);
				if (!send_buf)
					COREB_DEBUG(1, "NO MEM\n");
				memset(send_buf, 0, len);
				strcpy(send_buf, "finish");
				sm_send_packet(session_index, dst_ep, dst_cpu, send_buf, len);
			} else {
				COREB_DEBUG(1, "msg payload %s \n", buf);
			}

			sm_recv_release(buf, len, session_index);
		}

	}

	COREB_DEBUG(1, "%s() end\n", __func__);
}

void  icc_task_exit(void)
{
	sm_destroy_session(session_index);
}

