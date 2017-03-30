#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include <icc.h>
#include <mempool.h>

#define MSGQ_START_ADDR L2_START

struct coreb_icc_node {
	struct sm_icc_desc icc_info;
	struct sm_session_table *sessions_table;
	struct gen_pool *pool;
	struct gen_pool *msg_pool;
};

extern struct coreb_icc_node coreb_info;

int init_sm_session_table(void);


int sm_send_packet(uint32_t session_idx, uint16_t dst_ep,
		uint16_t dst_cpu, void *buf, uint32_t len);

int icc_get_session_status(void *user_param, uint32_t session_idx);

#endif
