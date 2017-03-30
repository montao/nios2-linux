/* Multicore communication on a BF561
 *
 * Copyright 2004-2009 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <asm-generic/errno-base.h>
#include <linux/bitmap.h>
#include <icc.h>
#include <protocol.h>
#include <blackfin.h>
#include <debug.h>

static inline void coreb_idle(void)
{
	__asm__ __volatile__( \
			".align 8;" \
			"nop;"  \
			"nop;"  \
			"idle;" \
			: \
			:  \
			);
}

struct coreb_icc_node coreb_info;
struct sm_msg scratch_msg;
struct sm_message scratch_message;
uint16_t iccq_should_stop;

#define SM_TASK_NONE 0
#define SM_TASK_RESET 1
#define SM_TASK_INIT 2
#define SM_TASK_RUNNING 3
struct sm_task sm_task1;
int sm_task1_status = 0;
int sm_task1_control_ep = 0;
uint32_t sm_task1_msg_buffer = 0;
int reinit = 0;
uint16_t pending = 0;

struct sm_proto *sm_protos[SP_MAX];

struct sm_message *get_message()
{
	return (struct sm_message *)gen_pool_alloc(coreb_info.msg_pool, 1 << 6);
}

void free_message(struct sm_message *message)
{
	gen_pool_free(coreb_info.msg_pool, (uint32_t)message, 1 << 6);
}

void *get_free_buffer(uint32_t size)
{
	return (void*)gen_pool_alloc(coreb_info.pool, size);
}

void free_buffer(uint32_t addr, uint32_t size)
{
	gen_pool_free(coreb_info.pool, addr, size);
}

int check_buffer_inpool(uint32_t addr, uint32_t size)
{
	return gen_pool_check(coreb_info.pool, addr, size);
}

int init_sm_session_table(void)
{
	coreb_info.sessions_table =
		(struct sm_session_table *)gen_pool_alloc(coreb_info.pool,
		sizeof(struct sm_session_table)); /* alloc session table*/
	if (!coreb_info.sessions_table) {
		coreb_msg("@@@ alloc session table failed\n");
		return -ENOMEM;
	}
	coreb_msg("session table %x\n", coreb_info.sessions_table);
	coreb_info.sessions_table->nfree = MAX_ENDPOINTS;
}

static int get_msg_src(struct sm_msg *msg)
{
	unsigned int n = 0;
	unsigned int offset;
	unsigned int align = 256;
	offset = (unsigned int)msg - MSGQ_START_ADDR;
	if (align < sizeof(struct sm_message_queue))
		align = (sizeof(struct sm_message_queue) + align - 1) / align;
	n = offset / align;
	if ((n % 2) == 0)
		return n + 1;
	else
		return 0;
}

static int sm_message_enqueue(struct sm_message_queue *outqueue, struct sm_msg *msg)
{
	unsigned int flags = bfin_cli();
	uint16_t sent = sm_atomic_read(&outqueue->sent);
	uint16_t received = sm_atomic_read(&outqueue->received);
	uint16_t pending = sent - received;

	if (pending < 0)
		pending += USHRT_MAX;

	if (pending >= (SM_MSGQ_LEN - 1)) {
		coreb_msg("over run\n");
		return -EAGAIN;
	}
	memcpy(&outqueue->messages[(sent%SM_MSGQ_LEN)], msg, sizeof(struct sm_msg));
	sent++;
	sm_atomic_write(&outqueue->sent, sent);
	bfin_sti(flags);
	return 0;
}

static int sm_message_dequeue(struct sm_message_queue *inqueue, struct sm_msg *msg)
{
	unsigned int flags = bfin_cli();
	uint16_t received = sm_atomic_read(&inqueue->received);
	received++;
	sm_atomic_write(&inqueue->received, received);
	bfin_sti(flags);
	return 0;
}

static struct sm_session_table* sm_get_session_table(void)
{
	struct sm_session_table *table = coreb_info.sessions_table;
	table->refcnt++;
	return table;
}

static int sm_put_session_table(void)
{
	struct sm_session_table *table = coreb_info.sessions_table;
	table->refcnt--;
	return 0;
}

struct sm_session* sm_index_to_session(uint32_t session_idx)
{
	struct sm_session *session;
	struct sm_session_table *table = coreb_info.sessions_table;
	if (session_idx < 0 && session_idx >= MAX_SESSIONS)
		return NULL;
	if (!test_bit(session_idx, table->bits))
		return NULL;
	session = &table->sessions[session_idx];
	return session;
}

uint32_t sm_session_to_index(struct sm_session *session)
{
	struct sm_session_table *table = coreb_info.sessions_table;
	uint32_t index;
	if ((session >= &table->sessions[0])
		&& (session < &table->sessions[MAX_SESSIONS])) {
		return ((session - &table->sessions[0])/sizeof(struct sm_session));
	}
	return -EINVAL;
}

static int sm_send_message_internal(struct sm_msg *msg, int dstcpu)
{
	struct sm_message_queue *outqueue = coreb_info.icc_info.icc_queue;
	int ret = 0;
	coreb_msg("%s() dst %d src %d %x\n", __func__, dstcpu, blackfin_core_id(), msg->type);
	while (sm_message_enqueue(outqueue, msg))
		continue;
	platform_send_ipi_cpu(dstcpu, COREB_ICC_LOW_SEND);
	return ret;
}

static uint32_t sm_alloc_session(struct sm_session_table *table)
{
	uint32_t index;
	index = find_next_zero_bit(table->bits, BITS_PER_LONG, 0);
	if (index >= BITS_PER_LONG)
		return -EAGAIN;
	bitmap_set(table->bits, index, 1);

	table->nfree--;
	return index;
}

static int sm_free_session(uint32_t slot, struct sm_session_table *table)
{
	memset(&table->sessions[slot], 0, sizeof(struct sm_session));
	__clear_bit(slot, table->bits);
	table->nfree++;
	return 0;
}

int sm_find_session(uint32_t local_ep, uint32_t remote_ep,
			struct sm_session_table *table)
{
	uint32_t index;
	struct sm_session *session;
//	coreb_msg("%s bits %08x\n", __func__, table->bits[0]);
	for_each_set_bit(index, table->bits, BITS_PER_LONG) {
		session = &table->sessions[index];
		coreb_msg("index %d ,local ep %d\n", index, session->local_ep);
		if(session->local_ep == local_ep) {
			if (remote_ep && session->remote_ep != remote_ep)
				return -EINVAL;
			goto found_slot;
		}
	}
	return -EINVAL;
found_slot:
	return index;
}

int sm_create_session(uint32_t src_ep, uint32_t type)
{
	coreb_msg("create ep \n");
	struct sm_session_table *table = sm_get_session_table();
	uint32_t index = sm_find_session(src_ep, 0, table);
	if (index >= 0 && index < 32) {
		coreb_msg("already bound index %d srcep %d\n", index, src_ep);
		return -EEXIST;
	}
	if (type >= SP_MAX) {
		coreb_msg("bad type %x\n", type);
		return -EINVAL;
	}
	index = sm_alloc_session(table);
	if (index >=0 && index <32) {
		table->sessions[index].local_ep = src_ep;
		table->sessions[index].remote_ep = 0;
		table->sessions[index].pid = 0;
		table->sessions[index].flags = 0;
		table->sessions[index].n_uncompleted = 0;
		table->sessions[index].n_avail = 0;
		table->sessions[index].type = type;
		table->sessions[index].proto_ops = sm_protos[type];
		INIT_LIST_HEAD(&table->sessions[index].tx_messages);
		INIT_LIST_HEAD(&table->sessions[index].rx_messages);
		coreb_msg("create ep index %d srcep %d\n", index, src_ep);
		sm_put_session_table();
		return index;
	}
	sm_put_session_table();
	return -EAGAIN;
}

int sm_register_session_handler(uint32_t session_idx,
			int (*handle)(struct sm_message *message, struct sm_session *session))
{
	struct sm_session *session = sm_index_to_session(session_idx);
	if (!session)
		return -EINVAL;

	if (handle)
		session->handle = handle;

	coreb_msg("%s handle %x\n", __func__, session->handle);
	return 0;
}

static int
sm_wait_for_connect_ack(struct sm_session *session)
{
	return 0;
}

int sm_destroy_session(uint32_t session_idx)
{
	struct sm_message *message;
	struct sm_msg *msg;
	struct sm_session *session;
	struct sm_session_table *table;
	session = sm_index_to_session(session_idx);
	if (!session)
		return -EINVAL;
	while (!list_empty(&session->rx_messages)) {
		message = list_first_entry(&session->rx_messages,
				struct sm_message, next);
		msg = &message->msg;

		if (session->flags == SM_CONNECT)
			sm_send_session_packet_ack(session, msg->src_ep,
					message->src, msg->payload, msg->length);
		else
			sm_send_packet_ack(session, msg->src_ep,
					message->src, msg->payload, msg->length);
		list_del(&message->next);
		free_message(message);
	}

	if (session->flags == SM_CONNECT)
		sm_send_close(session, msg->src_ep, message->src);

	table = sm_get_session_table();
	sm_free_session(session_idx, table);
	sm_put_session_table();
	return 0;
}

int sm_connect_session(uint32_t dst_ep, uint32_t dst_cpu,
			uint32_t src_ep, uint32_t type)
{
	struct sm_session_table *table;
	struct sm_session *session;
	uint32_t msg_type;
	table = sm_get_session_table();
	uint32_t index = sm_find_session(src_ep, 0, table);
	sm_put_session_table();
	session = sm_index_to_session(index);
	if (!session)
		return -EINVAL;

	if (type == SP_SESSION_SCALAR) {
		session->type = SP_SESSION_SCALAR;
		msg_type = SM_SESSION_SCALAR_CONNECT;
	} else if (type == SP_SESSION_PACKET) {
		session->type = SP_SESSION_PACKET;
		msg_type = SM_SESSION_PACKET_CONNECT;
	} else
		return -EINVAL;

	sm_send_connect(session, dst_ep, dst_cpu, msg_type);
	if (sm_wait_for_connect_ack(session))
		return -EAGAIN;
	table->sessions[index].remote_ep = dst_ep;
	table->sessions[index].flags = SM_CONNECT;
	sm_send_connect_done(session, dst_ep, dst_cpu);
	return 0;
}

int sm_disconnect_session(uint32_t dst_ep, uint32_t src_ep)
{
	struct sm_session_table *table;
	table = sm_get_session_table();
	uint32_t index = sm_find_session(src_ep, 0, table);
	sm_put_session_table();
	if (index >= MAX_SESSIONS)
		return -EINVAL;

	table->sessions[index].remote_ep = 0;
	table->sessions[index].flags = 0;
	return 0;
}

int sm_open_session(uint32_t index)
{
	struct sm_session *session;
	session = sm_index_to_session(index);
	if (!session)
		return -EINVAL;
	if (session->flags == SM_CONNECT) {
		session->flags |= SM_OPEN;
		return 0;
	}
	return -EINVAL;
}

int sm_close_session(uint32_t index)
{
	struct sm_session *session;
	session = sm_index_to_session(index);
	if (!session)
		return -EINVAL;
	if (session->flags & SM_OPEN) {
		session->flags &= ~SM_OPEN;
		return 0;
	}
	return -EINVAL;
}

#define SM_MAX_TASKARGS 3
void sm_handle_control_message()
{
	struct sm_message_queue *inqueue = coreb_info.icc_info.icc_queue + 1;
	uint16_t sent = sm_atomic_read(&inqueue->sent);
	uint16_t received = sm_atomic_read(&inqueue->received);
	struct sm_msg *msg;
	msg = &inqueue->messages[(received % SM_MSGQ_LEN)];

	coreb_msg("%s type %x\n", __func__, msg->type);

	if ((SM_MSG_PROTOCOL(msg->type) == SP_CORE_CONTROL) || (SM_MSG_PROTOCOL(msg->type) == SP_TASK_MANAGER)) {

		coreb_msg("%s %x %x\n", __func__, msg->type, SM_TASK_RUN);
		switch (msg->type) {
		case SM_CORE_START:
			iccq_should_stop = 0;
			break;
		case SM_CORE_STOP:
			iccq_should_stop = 1;
			break;
		case SM_TASK_RUN:
			if (sm_task1.task_init) {
				coreb_msg("task exit %x\n", sm_task1.task_exit);
				gen_pool_destroy(coreb_info.pool);
				gen_pool_destroy(coreb_info.msg_pool);
				reinit = 1;
			}
			memset(&sm_task1, 0, sizeof(struct sm_task));
			memcpy(&sm_task1, msg->payload,
				sizeof(struct sm_task));
			coreb_msg("task init %x exit %x\n", sm_task1.task_init, sm_task1.task_exit);
			sm_task1_control_ep = msg->src_ep;
			sm_task1_msg_buffer = msg->payload;

			if (reinit) {
				icc_init();
			}

			sm_task1_status = SM_TASK_RESET;
			sm_send_task_run_ack(sm_task1_control_ep, coreb_info.icc_info.peer_cpu);

			delay(1);

			coreb_msg("finish %s task status %d\n", __func__, sm_task1_status);
			break;
		case SM_TASK_KILL:
			if (sm_task1.task_init)
				sm_task1.task_exit();
			memset(&sm_task1, 0, sizeof(struct sm_task));
			sm_task1_status = SM_TASK_NONE;
			break;
		}
		sm_message_dequeue(inqueue, msg);
		if (reinit)
			memset(inqueue, 0, sizeof(struct sm_message_queue));
		coreb_msg("finish1 %s task status %d\n", __func__, sm_task1_status);
		return;
	}
}

int
sm_send_control_msg(struct sm_session *session, uint32_t remote_ep,
			uint32_t dst_cpu, uint32_t payload,
			uint32_t len, uint32_t type)
{
	int ret;
	struct sm_msg *m = &scratch_msg;
	memset(m, 0, sizeof(struct sm_msg));

	m->type = type;
	if (session)
		m->src_ep = session->local_ep;
	m->dst_ep = remote_ep;
	m->length = len;
	m->payload = payload;

	ret = sm_send_message_internal(m, dst_cpu);
	if (ret)
		return -EAGAIN;
	return ret;
}

int sm_send_task_run_ack(uint32_t remote_ep,
		uint32_t dst_cpu)
{
	int ret;
	struct sm_msg *m = &scratch_msg;
	memset(m, 0, sizeof(struct sm_msg));

	m->type = SM_TASK_RUN_ACK;
	m->src_ep = 0;
	m->dst_ep = remote_ep;
	m->length = 0;
	if (sm_task1_msg_buffer)
		m->payload = sm_task1_msg_buffer;

	ret = sm_send_message_internal(m, dst_cpu);
	if (ret)
		return -EAGAIN;
	return ret;
}

int sm_send_task_kill_ack(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu)
{
	int ret;
	struct sm_msg *m = &scratch_msg;
	memset(m, 0, sizeof(struct sm_msg));

	m->type = SM_TASK_KILL_ACK;
	m->src_ep = 0;
	m->dst_ep = remote_ep;
	m->length = 0;
	m->payload = 0;

	ret = sm_send_message_internal(m, dst_cpu);
	if (ret)
		return -EAGAIN;
	return ret;
}

int
sm_send_packet_ack(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu, uint32_t payload, uint32_t len)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, payload,
					len, SM_PACKET_CONSUMED);
}

int
sm_send_session_packet_ack(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu, uint32_t payload, uint32_t len)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, payload,
					len, SM_SESSION_PACKET_CONSUMED);
}

int sm_send_scalar_cmd(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu, uint32_t payload, uint32_t len)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, payload,
			len, SM_SCALAR_READY_64);
}

int sm_send_scalar_ack(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu, uint32_t payload, uint32_t len)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, payload,
			len, SM_SCALAR_CONSUMED);
}

	int
sm_send_session_scalar_ack(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu, uint32_t payload, uint32_t len)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, payload,
			len, SM_SESSION_SCALAR_CONSUMED);
}

int sm_send_connect(struct sm_session *session, uint32_t remote_ep,
			uint32_t dst_cpu, uint32_t type)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
			0, type);
}

int sm_send_connect_ack(struct sm_session *session, uint32_t remote_ep,
			uint32_t dst_cpu)
{
	if (session->type == SP_SESSION_PACKET)
		return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
				0, SM_SESSION_PACKET_CONNECT_ACK);
	else if (session->type == SP_SESSION_SCALAR)
		return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
				0, SM_SESSION_SCALAR_CONNECT_ACK);
	else
		return -EINVAL;
}

int sm_send_connect_done(struct sm_session *session, uint32_t remote_ep,
			uint32_t dst_cpu)
{
	if (session->type == SP_SESSION_PACKET)
		return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
				0, SM_SESSION_PACKET_CONNECT_DONE);
	else if (session->type == SP_SESSION_SCALAR)
		return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
				0, SM_SESSION_SCALAR_CONNECT_DONE);
	else
		return -EINVAL;
}

int sm_send_session_active(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
			0, SM_SESSION_PACKET_ACTIVE);
}

int sm_send_session_active_ack(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, SM_OPEN,
			0, SM_SESSION_PACKET_ACTIVE_ACK);
}

int sm_send_session_active_noack(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
			0, SM_SESSION_PACKET_ACTIVE_ACK);
}

int sm_send_close(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu)
{
	if (session->type == SP_SESSION_PACKET)
		return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
				0, SM_SESSION_PACKET_CLOSE);
	else if (session->type == SP_SESSION_SCALAR)
		return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
				0, SM_SESSION_SCALAR_CLOSE);
	else
		return -EINVAL;
}

int sm_send_close_ack(struct sm_session *session, uint32_t remote_ep,
		uint32_t dst_cpu)
{
	if (session->type == SP_SESSION_PACKET)
		return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
				0, SM_SESSION_PACKET_CLOSE_ACK);
	else if (session->type == SP_SESSION_SCALAR)
		return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
				0, SM_SESSION_SCALAR_CLOSE_ACK);
	else
		return -EINVAL;
}

int sm_send_error(struct sm_session *session, uint32_t remote_ep,
			uint32_t dst_cpu)
{
	return sm_send_control_msg(session, remote_ep, dst_cpu, 0,
			0, SM_PACKET_ERROR);
}

void *sm_send_request(uint32_t size, uint32_t session_index)
{
	void *buf = get_free_buffer(size);
	return buf;
}

int sm_recv_release(void *addr, uint32_t size, uint32_t session_idx)
{
	struct sm_message *message = NULL;
	struct sm_msg *msg = NULL;
	struct sm_session *session = sm_index_to_session(session_idx);
	if (!session)
		return -EINVAL;
	if (!list_empty(&session->rx_messages)) {
		message = list_first_entry(&session->rx_messages,
					struct sm_message, next);
		msg = &message->msg;

	}

	if (msg && msg->payload != (unsigned long)addr)
		return -EINVAL;

	if (SM_MSG_PROTOCOL(msg->type) == SP_PACKET)
		sm_send_packet_ack(session, msg->src_ep, message->src, msg->payload, msg->length);
	else
		sm_send_session_packet_ack(session, msg->src_ep, message->src, msg->payload, msg->length);

	list_del(&message->next);
	session->n_avail--;
	coreb_msg("free message %x\n", (unsigned long)message);
	free_message(message);
	return 0;
}

int
sm_send_scalar(uint32_t session_idx, uint16_t dst_ep,
		uint16_t dst_cpu, uint32_t scalar0, uint32_t scalar1, uint32_t size)
{
	struct sm_session *session;
	int ret = -EAGAIN;
	struct sm_message *message = get_message();

	session = sm_index_to_session(session_idx);

	if (!session)
		return -EINVAL;

	message->msg.src_ep = session->local_ep;
	message->msg.dst_ep = dst_ep;
	message->msg.payload = scalar0;
	message->msg.length = scalar1;

	switch (size) {
	case 1:
		message->msg.type = SM_MSG_TYPE(session->type, SM_SCALAR_8BIT);
		break;
	case 2:
		message->msg.type = SM_MSG_TYPE(session->type, SM_SCALAR_16BIT);
		break;
	case 4:
		message->msg.type = SM_MSG_TYPE(session->type, SM_SCALAR_32BIT);
		break;
	case 8:
		message->msg.type = SM_MSG_TYPE(session->type, SM_SCALAR_64BIT);
		break;
	}

	ret = session->proto_ops->sendmsg(message, session);
	if (ret)
		goto fail;

	ret = sm_send_message_internal(&message->msg, dst_cpu);
	if (!ret)
		goto out;
fail:
	free_message(message);
out:
	return ret;
}

int
sm_send_packet(uint32_t session_idx, uint16_t dst_ep,
		uint16_t dst_cpu, void *buf, uint32_t len)
{
	struct sm_session *session;
	void *payload_buf = NULL;
	int ret = -EAGAIN;
	struct sm_message *message = get_message();

	session = sm_index_to_session(session_idx);

	if (!session)
		return -EINVAL;

	message->msg.src_ep = session->local_ep;
	message->msg.dst_ep = dst_ep;
	message->msg.length = len;
	message->msg.type = SM_MSG_TYPE(session->type, 0);

	if (message->msg.length) {
		if (!check_buffer_inpool(buf, len)) {
			payload_buf = get_free_buffer(message->msg.length);
			if (!payload_buf) {
				ret = -ENOMEM;
				goto out;
			}
			message->msg.payload = payload_buf;
			memcpy(message->msg.payload, buf, message->msg.length);
		} else {
			message->msg.payload = buf;
			coreb_msg("%s() in pool %x %s \n", __func__, (unsigned int)buf, message->msg.payload);
		}
	} else {
		ret = -EINVAL;
		goto out;
	}
	ret = session->proto_ops->sendmsg(message, session);
	if (ret)
		goto fail;

	ret = sm_send_message_internal(&message->msg, dst_cpu);
	if (!ret)
		goto out;

fail:
	free_message(message);
	if (payload_buf)
		free_buffer(payload_buf, message->msg.length);
out:
	return ret;
}

int sm_recv_scalar(uint32_t session_idx, uint16_t *src_ep, uint16_t *src_cpu, uint32_t *scalar0,
				uint32_t *scalar1, uint32_t *size)
{
	struct sm_message *message;
	struct sm_msg *msg;
	struct sm_session *session;
	int ret = 0;
	uint32_t len = 0;

	session = sm_index_to_session(session_idx);
	if (!session)
		return 0;

	coreb_msg(" %s session type %x localep%d\n",__func__, session->type, session->local_ep);
	if (!list_empty(&session->rx_messages)) {
		message = list_first_entry(&session->rx_messages,
					struct sm_message, next);

		msg = &message->msg;

		coreb_msg("msg type%x src ep %x scalar0 %x scalar1 %x\n", msg->type, msg->src_ep, msg->payload, msg->length);

		if (src_ep)
			*src_ep = msg->src_ep;
		if (src_cpu)
			*src_cpu = message->src;
		if (scalar0)
			*scalar0 = msg->payload;
		if (scalar1)
			*scalar1 = msg->length;
		switch (msg->type) {
		case SM_SCALAR_READY_8:
		case SM_SESSION_SCALAR_READY_8:
			len = 1;
			break;
		case SM_SCALAR_READY_16:
		case SM_SESSION_SCALAR_READY_16:
			len = 2;
			break;
		case SM_SCALAR_READY_32:
		case SM_SESSION_SCALAR_READY_32:
			len = 4;
			break;
		case SM_SCALAR_READY_64:
		case SM_SESSION_SCALAR_READY_64:
			len = 8;
			break;
		}

		if (size)
			*size = len;

		list_del(&message->next);
		free_message(message);
		session->n_avail--;
		coreb_msg("%s() s0%x s1%x avail %d\n", __func__, *scalar0, *scalar1, session->n_avail);
		ret = 1;


	} else {
		coreb_msg("no message\n");
		ret = 0;
	}
	coreb_msg(" %s msg\n",__func__);
	return ret;
}

int sm_recv_packet(uint32_t session_idx, uint16_t *src_ep, uint16_t *src_cpu, void **buf,
				uint32_t *len)
{
	struct sm_message *message;
	struct sm_msg *msg;
	struct sm_session *session;
	int ret = 0;

	session = sm_index_to_session(session_idx);

	if (!session)
		return -EINVAL;

	coreb_msg(" %s session type %x localep%d\n",__func__, session->type, session->local_ep);
	if (!list_empty(&session->rx_messages)) {
		message = list_first_entry(&session->rx_messages,
					struct sm_message, next);
		msg = &message->msg;
		if (src_ep)
			*src_ep = message->msg.src_ep;
		if (src_cpu)
			*src_cpu = message->src;
		if (len)
			*len = message->msg.length;
		*buf = msg->payload;
		ret = msg->length;
	} else {
		ret = -EINVAL;
	}
	return ret;
}

inline uint16_t iccqueue_getpending()
{
	struct sm_message_queue *inqueue = coreb_info.icc_info.icc_queue + 1;
	uint16_t sent = sm_atomic_read(&inqueue->sent);
	uint16_t received = sm_atomic_read(&inqueue->received);
	uint16_t pending;
	pending = sent - received;
	if(pending < 0)
		pending += USHRT_MAX;
	return (pending % SM_MSGQ_LEN);
}

static int msg_recv_internal(struct sm_msg *msg, struct sm_session *session)
{
	int ret = 0;
	int cpu = blackfin_core_id();
	struct sm_message *message = get_message();
	coreb_msg("%s msg type %x alloc %x\n", __func__, msg->type, (unsigned long)message);
	if (!message) {
		coreb_msg("no mem in pool discard message\n");
		return -1;
	}
	memcpy(&message->msg, msg, sizeof(struct sm_msg));
	message->dst = cpu;
	message->src = coreb_info.icc_info.peer_cpu;

	if ((SM_MSG_PROTOCOL(msg->type) == SP_SCALAR))
		sm_send_scalar_ack(session, msg->src_ep, message->src,
				msg->payload, msg->length);
	else if ((SM_MSG_PROTOCOL(msg->type) == SP_SESSION_SCALAR))
		sm_send_session_scalar_ack(session, msg->src_ep, message->src,
				msg->payload, msg->length);

	if (session->handle) {
		coreb_msg("default handler\n");
		session->handle(message, session);
		free_message(message);
	} else {
		list_add_tail(&message->next, &session->rx_messages);
		session->n_avail++;
		coreb_msg("avail %d \n", session->n_avail);
	}
	return ret;
}

static int sm_default_sendmsg(struct sm_message *message, struct sm_session *session)
{
	struct sm_msg *msg = &message->msg;
	coreb_msg("%s msg type %x\n", __func__, msg->type);
	switch (SM_MSG_PROTOCOL(msg->type)) {
	case SP_PACKET:
	case SP_SESSION_PACKET:
	case SP_SCALAR:
	case SP_SESSION_SCALAR:
		list_add_tail(&message->next, &session->tx_messages);
		session->n_uncompleted++;
		break;
	case SM_PACKET_ERROR:
		coreb_msg("SM ERROR %08x\n", msg->payload);
		break;
	default:
		break;
	};
	return 0;
}

static int
sm_default_recvmsg(struct sm_msg *msg, struct sm_session *session)
{
	int ret = 0;
	struct sm_message *uncompleted;
	struct sm_message_queue *inqueue = coreb_info.icc_info.icc_queue + 1;

	coreb_msg("type %x, dstep %d, srcep %d \n", msg->type, msg->dst_ep, msg->src_ep);
	switch (msg->type) {
	case SM_PACKET_CONSUMED:
	case SM_SESSION_PACKET_CONSUMED:
		list_for_each_entry(uncompleted, &session->tx_messages, next) {
			if (uncompleted->msg.payload == msg->payload) {
				coreb_msg("ack matched free buf %x\n", msg->payload);
				goto matched;
			}
			coreb_msg("unmatched ack %08x %x uncomplete tx %08x\n", msg->payload, msg->length, uncompleted->msg.payload);
		}
		coreb_msg("unmatched ack\n");
		break;
matched:
		list_del(&uncompleted->next);
		session->n_uncompleted--;
		coreb_msg("free buffer %x\n", msg->payload);
		free_buffer(msg->payload, uncompleted->msg.length);
		free_message(uncompleted);
		break;
	case SM_SCALAR_CONSUMED:
	case SM_SESSION_SCALAR_CONSUMED:
		list_for_each_entry(uncompleted, &session->tx_messages, next) {
			if (uncompleted->msg.payload == msg->payload) {
				coreb_msg("ack matched free buf %x\n", msg->payload);
				goto matched1;
			}
			coreb_msg("unmatched ack %08x %x uncomplete tx %08x\n", msg->payload, msg->length, uncompleted->msg.payload);
		}
		coreb_msg("unmatched ack\n");
		break;
matched1:
		list_del(&uncompleted->next);
		session->n_uncompleted--;
		coreb_msg("free message %x\n", uncompleted);
		free_message(uncompleted);
		break;
	case SM_SESSION_PACKET_CONNECT_ACK:
	case SM_SESSION_SCALAR_CONNECT_ACK:
		session->remote_ep = msg->src_ep;
		session->flags = SM_CONNECT;
		break;
	case SM_SESSION_PACKET_CONNECT:
	case SM_SESSION_SCALAR_CONNECT:
		session->remote_ep = msg->src_ep;
		session->flags = SM_CONNECTING;
		session->type = SM_MSG_PROTOCOL(msg->type);
		coreb_msg("session type %x\n", session->type);
		sm_send_connect_ack(session, msg->src_ep, coreb_info.icc_info.peer_cpu);
		break;
	case SM_SESSION_PACKET_CONNECT_DONE:
	case SM_SESSION_SCALAR_CONNECT_DONE:
		session->flags = SM_CONNECT;
		coreb_msg("connected %x %d\n", session->flags, session->remote_ep);
		break;
	case SM_SESSION_PACKET_ACTIVE:
		if (session->flags & SM_OPEN)
			sm_send_session_active_ack(session, msg->src_ep, coreb_info.icc_info.peer_cpu);
		else
			sm_send_session_active_noack(session, msg->src_ep, coreb_info.icc_info.peer_cpu);
		break;
	case SM_SESSION_PACKET_ACTIVE_ACK:
		if (session->flags & SM_OPEN) {
			if (msg->payload == SM_OPEN) {
				session->flags |= SM_ACTIVE;
			}
		}
		break;
	case SM_SESSION_PACKET_CLOSE:
	case SM_SESSION_SCALAR_CLOSE:
		session->remote_ep = 0;
		session->flags = 0;
		sm_send_close_ack(session, msg->src_ep, coreb_info.icc_info.peer_cpu);
		break;
	case SM_SESSION_PACKET_CLOSE_ACK:
	case SM_SESSION_SCALAR_CLOSE_ACK:
		session->remote_ep = 0;
		session->flags = 0;
		break;
	case SM_PACKET_READY:
	case SM_SESSION_PACKET_READY:
		if (SM_MSG_PROTOCOL(msg->type) != session->type) {
			coreb_msg("msg type %08x unmatch session type %08x\n", msg->type, session->type);
			break;
		}
		coreb_msg("recved packet msg handle%x\n", (unsigned int)session->handle);
		msg_recv_internal(msg, session);
		break;
	case SM_SCALAR_READY_8:
	case SM_SCALAR_READY_16:
	case SM_SCALAR_READY_32:
	case SM_SCALAR_READY_64:
	case SM_SESSION_SCALAR_READY_8:
	case SM_SESSION_SCALAR_READY_16:
	case SM_SESSION_SCALAR_READY_32:
	case SM_SESSION_SCALAR_READY_64:
		msg_recv_internal(msg, session);
		break;
	case SM_PACKET_ERROR:
		coreb_msg("SM ERROR %08x\n", msg->payload);
		break;
	default:
		ret = -EINVAL;
	};

	sm_message_dequeue(inqueue, msg);
	return ret;
}

static int sm_default_shutdown(struct sm_session *session)
{
	return 0;
}

static int sm_default_error(struct sm_msg *msg, struct sm_session *session)
{
	return 0;
}

struct sm_proto packet_proto = {
	.sendmsg = sm_default_sendmsg,
	.recvmsg = sm_default_recvmsg,
	.shutdown = sm_default_shutdown,
	.error = sm_default_error,
};

struct sm_proto session_packet_proto = {
	.sendmsg = sm_default_sendmsg,
	.recvmsg = sm_default_recvmsg,
	.shutdown = sm_default_shutdown,
	.error = sm_default_error,
};

struct sm_proto scalar_proto = {
	.sendmsg = sm_default_sendmsg,
	.recvmsg = sm_default_recvmsg,
	.shutdown = sm_default_shutdown,
	.error = sm_default_error,
};

struct sm_proto session_scalar_proto = {
	.sendmsg = sm_default_sendmsg,
	.recvmsg = sm_default_recvmsg,
	.shutdown = sm_default_shutdown,
	.error = sm_default_error,
};

void register_sm_proto(void)
{
	sm_protos[SP_PACKET] = &packet_proto;
	sm_protos[SP_SESSION_PACKET] = &session_packet_proto;
	sm_protos[SP_SCALAR] = &scalar_proto;
	sm_protos[SP_SESSION_SCALAR] = &session_scalar_proto;
}

void icc_run_task(void)
{
	char *task_argv[SM_MAX_TASKARGS];
	struct sm_task *task;
	int i;
	if (sm_task1_status == SM_TASK_INIT) {
		task = &sm_task1;
		for (i = 0; i < SM_MAX_TASKARGS; i++) {
			task_argv[i] = task->task_argv[i];
		}

		coreb_msg("before run task %x\n", sm_task1.task_init);
		sm_task1_status = SM_TASK_RUNNING;
		sm_task1.task_init(sm_task1.task_argc, task_argv);
		sm_task1_status = SM_TASK_NONE;
	}
}

int sm_get_session_status(uint32_t session_idx, struct sm_session_status *status)
{
	struct sm_session *session = sm_index_to_session(session_idx);
	if (!session)
		return -EINVAL;
	if (!status)
		return -EINVAL;
	memset(status, 0, sizeof(*status));
	status->n_avail = session->n_avail;
	status->n_uncompleted = session->n_uncompleted;
	status->local_ep = session->local_ep;
	status->remote_ep = session->remote_ep;
	status->type = session->type;
	status->pid = session->pid;
	status->flags = session->flags;
	return 0;
}


int icc_handle_scalar_cmd(struct sm_msg *msg)
{
	int ret;
	uint32_t scalar0, scalar1;
	uint16_t src_cpu;
	struct sm_session *session;
	int index;

	if (msg->type != SM_SCALAR_READY_64)
		return 0;

	scalar0 = msg->payload;
	scalar1 = msg->length;

	src_cpu = get_msg_src(msg);

	coreb_msg("scalar cmd %x %d\n", scalar0, src_cpu);

	if (SM_SCALAR_CMD(scalar0) != SM_SCALAR_CMD_HEAD)
		return 0;

	coreb_msg("scalar cmd %x %x\n", SM_SCALAR_CMD(scalar0), SM_SCALAR_CMD_HEAD);
	switch (SM_SCALAR_CMDARG(scalar0)) {
	case SM_SCALAR_CMD_GET_SESSION_ID:
		index = sm_find_session(scalar1, 0, coreb_info.sessions_table);
		session = sm_index_to_session(index);
		if (session) {
			scalar0 = MK_SM_SCALAR_CMD_ACK(SM_SCALAR_CMD_GET_SESSION_ID);
			scalar1 = index;
			sm_send_scalar_cmd(NULL, msg->src_ep, src_cpu, scalar0,
					scalar1);
		}
		break;
	case SM_SCALAR_CMD_GET_SESSION_TYPE:
		break;
	default:
		return 0;
	}

	return 1;
}

uint32_t msg_handle(void);
int icc_wait(int session_mask)
{
	uint32_t avail = 0;
	unsigned long flags;
	icc_run_task();
	pending = iccqueue_getpending();
	if (!pending) {
		bfin_coretmr_set_next_event(4000000000);
		coreb_idle();
		return 0;
	}
	avail = msg_handle();
	return avail;
}

uint32_t msg_handle(void)
{
	struct sm_message_queue *inqueue = coreb_info.icc_info.icc_queue + 1;
	uint16_t sent = sm_atomic_read(&inqueue->sent);
	uint16_t received = sm_atomic_read(&inqueue->received);
	struct sm_msg *msg;
	struct sm_session *session;
	int pending;
	uint32_t index;
	struct sm_session_status status;

	sm_handle_control_message();
	msg = &inqueue->messages[(received % SM_MSGQ_LEN)];

	index = sm_find_session(msg->dst_ep, 0, coreb_info.sessions_table);

	session = sm_index_to_session(index);
	if (!session) {
		coreb_msg("error messag type %08x\n", msg->type);
		sm_message_dequeue(inqueue, msg);
		return;
	}

	pending = iccqueue_getpending();
	if (!pending) {
		coreb_msg("BUG\n");
		return 0;
	}

	coreb_msg("msg type %x index %d session type %x\n", msg->type, index, session->type);
	if (session) {
		if (session->proto_ops->recvmsg) {
			session->proto_ops->recvmsg(msg, session);
			memset(&status, 0, sizeof(status));
			sm_get_session_status(index, &status);
			if (status.n_avail)
				coreb_msg("index %d avail %d\n", index, status.n_avail);
		} else
			coreb_msg("unsupported protocol\n");

	} else {
		coreb_msg("discard msg type\n", msg->type);
		sm_message_dequeue(inqueue, msg);
	}
	return status.n_avail;
}

