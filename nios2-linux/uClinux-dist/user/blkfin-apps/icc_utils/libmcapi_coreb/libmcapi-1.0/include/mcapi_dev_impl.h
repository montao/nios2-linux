
int sm_dev_initialize(void);

void sm_dev_finalize(void);

int sm_create_session(uint32_t src_ep, uint32_t type);
int sm_destroy_session(uint32_t src_ep);
int sm_connect_session(uint32_t dst_ep, uint32_t dst_cpu, uint32_t src_ep);
int sm_disconnect_session(uint32_t dst_ep, uint32_t src_ep);
int sm_send_packet(uint32_t session_idx, uint32_t dst_ep,
		uint32_t dst_cpu, void *buf, uint32_t len);
int sm_recv_packet(uint32_t session_idx, void **buf,
		uint32_t len);
