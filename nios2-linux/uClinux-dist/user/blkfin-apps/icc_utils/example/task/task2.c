#include <icc.h>
#include <protocol.h>
#include <debug.h>

#define LOCAL_SESSION 6

uint32_t __icc_task_data session_index;
void  __icc_task icc_task_init(int argc, char *argv[])
{
	struct sm_session *session;
	void *buf;
	int len;
	int ret;
	int src_ep, src_cpu;
	struct l3_proto_head *p;
	session_index = sm_create_session(LOCAL_SESSION, SP_SESSION_PACKET);
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
			p = (struct l3_proto_head *)buf;
			if (p->type == L3_TYPE_AUDIO) {
				int len = sizeof(struct l3_proto_head);
				unsigned int audio_buf = p->chunk_addr;
				unsigned int size = p->chunk_size;
				int dst_ep = src_ep;
				int dst_cpu = src_cpu;
				void *send_buf = sm_send_request(len, session);
				COREB_DEBUG(1, "coreb send buf %x\n", send_buf);
				if (!send_buf)
					COREB_DEBUG(1, "NO MEM\n");
				memset(send_buf, 0, len);
				p = send_buf;
				p->type = L3_TYPE_AUDIO;
				p->chunk_addr = audio_buf;
				p->chunk_size = size;
				p->status = 1;
				sm_send_packet(session_index, dst_ep, dst_cpu, send_buf, len);
			} else {
				COREB_DEBUG(1, "msg payload %s \n", buf);
			}

			sm_recv_release(buf, len, session_index);
		}

	}

	COREB_DEBUG(1, "%s() end\n", __func__);
}

void  __icc_task icc_task_exit(void)
{
	sm_destroy_session(session_index);
}
