/*
Copyright (c) 2008, The Multicore Association
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

(1) Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
 
 (2) Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution. 

 (3) Neither the name of the Multicore Association nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <string.h>
#include <errno.h>
#include <assert.h>

#include <mcapi.h>
#include <transport_sm.h>
#include <mcapi_test.h>

#include <icc.h>
#include <stdio.h>

#define SEMKEYPATH "/dev/null"  /* Path used on ftok for semget key  */
#define SEMKEYID 1              /* Id used on ftok for semget key    */
#define SHMKEYPATH "/dev/null"  /* Path used on ftok for shmget key  */
#define SHMKEYID 2              /* Id used on ftok for shmget key    */
#define MAGIC_NUM 0xdeadcafe

/* the semaphore id */
uint32_t sem_id;
/* the shared memory address */
void* shm_addr;
/* the shared memory database */
mcapi_database* c_db = NULL;


/* debug printing */
/* Inline this (and define in header) so that it can be compiled out if WITH_DEBUG is 0 */
inline void mcapi_dprintf(int level,const char *format, ...) {
	if (WITH_DEBUG) {
		va_list ap;
		va_start(ap,format);
		if (level <= mcapi_debug){
			printf("MCAPI_DEBUG:");
			/* call variatic printf */
			vprintf(format,ap);
		}
		va_end(ap);
	}
}
/* the debug level */
int mcapi_debug = 7;

mcapi_endpoint_t mcapi_icc_index;

/* semaphore management */
uint32_t transport_sm_create_semaphore(uint32_t semkey) {
	int sem_id;
	union semun {
		int              val;    /* Value for SETVAL */
		struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
		unsigned short  *array;  /* Array for GETALL, SETALL */
		struct seminfo  *__buf;  /* Buffer for IPC_INFO
					    (Linux-specific) */
	} arg;

	arg.val = 1;
	if((sem_id = semget(semkey, 1, IPC_CREAT|IPC_EXCL|0666)) == -1)  {
		perror("semget\n");
		return -1;
	} else {
		if (semctl(sem_id, 0, SETVAL, arg) == -1) {
			perror("semctl\n");
			return -1;
		}
	}
	return sem_id;
}

mcapi_boolean_t transport_sm_lock_semaphore(uint32_t semid)
{
	struct sembuf sem_lock={ 0, -1, 0}; 
	if((semop(sem_id, &sem_lock, 1)) == -1) {
		return -1;
	}
	return 0;
}

mcapi_boolean_t transport_sm_unlock_semaphore(uint32_t semid)
{
	struct sembuf sem_unlock={ 0, 1, 0};
	/* Attempt to unlock the semaphore set */
	if((semop(sem_id, &sem_unlock, 1)) == -1) {
		return -1;
	}

	return 0;
}

void* sm_attach_shared_mem(uint32_t shmid){ 
	void *shm_addr;
	int rval;
	struct shmid_ds dsbuf;

	shm_addr = shmat(shmid, 0, 0);
	if ((long)shm_addr == (-1)) {
		return NULL;
	}
	rval = shmctl(shmid, IPC_STAT, &dsbuf);
	if (rval != 0) {
		shmdt(shm_addr);
		return NULL;
	}

	/* if we are the first to attach, then initialize the segment to 0 */
	if (dsbuf.shm_nattch == 1) {
		memset(shm_addr,0,dsbuf.shm_segsz);
	}
	return shm_addr;
}

/* shared memory management */
mcapi_boolean_t transport_sm_create_shared_mem(void** addr,uint32_t shmkey,uint32_t size) 
{
	uint32_t shmid = shmget(shmkey, size, 0666 | IPC_CREAT | IPC_EXCL); 
	if (errno == EEXIST) {
		shmid = shmget(shmkey, size, 0666 | IPC_CREAT); 
	}

	if (shmid == -1) {
		return MCAPI_FALSE;
	}  else {
		*addr = sm_attach_shared_mem(shmid);
		return MCAPI_TRUE;
	}
}

mcapi_boolean_t transport_sm_free_shared_mem(uint32_t shmid,void *shm_address)
{
	struct shmid_ds shmid_struct;

	/* detach the shared memory segment */
	int rc = shmdt(shm_address);
	if (rc==-1) {
		mcapi_dprintf(1,"Warning: mrapi: sysvr4_free_shared_mem shmdt() failed\n");
		return MCAPI_FALSE;
	}

	/* delete the shared memory id */
	rc = shmctl(shmid, IPC_RMID, &shmid_struct);
	if (rc==-1)  {
		mcapi_dprintf(1,"Warning: mrapi: sysvr4_free_shared_mem shmctl() failed\n");
		return MCAPI_FALSE;
	}
	return MCAPI_TRUE;

}

mcapi_boolean_t mcapi_icc_node_init()
{
	int node_index;
	mcapi_icc_index = sm_create_session(MCAPI_MANAGE_SESSION, SP_SCALAR);
	if (mcapi_icc_index < 0) {
		return MCAPI_FALSE;
	}
	node_index = 0;

	c_db->nodes[node_index].node_d.endpoints[mcapi_icc_index].valid = MCAPI_TRUE;
	c_db->nodes[node_index].node_d.endpoints[mcapi_icc_index].port_num = MCAPI_MANAGE_SESSION;
	c_db->nodes[node_index].node_d.endpoints[mcapi_icc_index].open = MCAPI_FALSE;

	c_db->nodes[node_index].node_d.num_endpoints++;


	return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_get_remote_endpoint(uint16_t node_index, mcapi_uint_t port_num, uint16_t *port_index)
{
	uint32_t scalar0, scalar1;
	uint32_t size;
	uint16_t src_ep, src_cpu;
	int ret;
	scalar0 = MK_MCAPI_CMD(MCAPI_CMD_GET_ID);
	scalar1 = port_num;
	ret = sm_send_scalar(mcapi_icc_index, MCAPI_MANAGE_SESSION, node_index, scalar0, scalar1, 8);
	if (ret)
		return MCAPI_FALSE;
	scalar0 = 0;
	scalar1 = 0;

	ret = sm_recv_scalar(mcapi_icc_index, &src_ep, &src_cpu, &scalar0, &scalar1, &size);
	if (ret)
		return MCAPI_FALSE;

	printf("recv scalar0 %x, scalar1 %x\n", scalar0, scalar1);
	if (MCAPI_HEAD(scalar0) == MCAPI_ACK_HEAD) {
		*port_index = scalar1;
		return MCAPI_TRUE;
	} else
		return MCAPI_FALSE;
}

mcapi_boolean_t mcapi_trans_set_node_num(mcapi_uint_t n)
{
	return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_get_node_num(mcapi_uint_t* node)
{
	*node = c_db->nodes[0].node_num;
	return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_get_port_num(mcapi_uint_t port_index, mcapi_uint_t *port_num)
{
	int rc = MCAPI_FALSE;
	int i;
	for (i = 0; i < c_db->num_nodes ; i++) {
		if (c_db->nodes[i].node_d.endpoints[port_index].valid) {
			*port_num = c_db->nodes[i].node_d.endpoints[port_index].port_num;
			rc = MCAPI_TRUE;
		}
	}
	return rc;
}

mcapi_boolean_t mcapi_trans_get_remote_port_num(mcapi_uint_t port_index, mcapi_uint_t *port_num)
{
	int rc = MCAPI_FALSE;
	int i;
	for (i = c_db->num_nodes; i < c_db->num_nodes*2 ; i++) {
		if (c_db->nodes[i].node_d.endpoints[port_index].valid) {
			*port_num = c_db->nodes[i].node_d.endpoints[port_index].port_num;
			rc = MCAPI_TRUE;
		}
	}
	return rc;
}

mcapi_uint16_t mcapi_trans_get_node_index(mcapi_uint_t node_num)
{
  /* look up the node */
  int i;
  uint32_t node_index = MAX_NODES;
  for (i = 0; i < c_db->num_nodes; i++) {
    if (c_db->nodes[i].node_num == node_num) {
      node_index = i;
      break;
    }
  }
  assert (node_index != MAX_NODES);
  return node_index;
}

mcapi_uint16_t mcapi_trans_get_port_index(mcapi_uint_t node_num, mcapi_uint_t port_num)
{
	/* look up the node port*/
	int i, j;
	uint32_t port_index = MAX_ENDPOINTS;
	for (i = 0; i < c_db->num_nodes; i++) {
		if (c_db->nodes[i].node_num == node_num) { 
			for (j = 0; j < c_db->nodes[i].node_d.num_endpoints; j++) {
				mcapi_dprintf(1,"index %d %d\n", c_db->nodes[i].node_d.endpoints[j].port_num, port_num);
				if ((c_db->nodes[i].node_d.endpoints[j].valid) && 
						(c_db->nodes[i].node_d.endpoints[j].port_num == port_num)) {
					/* return the handle */
					port_index = j;
					break;
				}
			}
		}
	}

	return port_index;
}




mcapi_boolean_t mcapi_trans_add_node (mcapi_uint_t node_num) 
{
	mcapi_boolean_t rc = MCAPI_TRUE;

	/* lock the database */

	/* mcapi should have checked that the node doesn't already exist */

	if (c_db->num_nodes == MAX_NODES) {
		rc = MCAPI_FALSE;
	}

	if (rc) {
		/* setup our local (private data) */
		/* we do this while we have the lock because we don't want an inconsistency/
		   race condition where the node exists in the database but not yet in
		   the transport layer's cached state */
		mcapi_trans_set_node_num(node_num);

		/* add the node */
		c_db->nodes[c_db->num_nodes].finalized = MCAPI_FALSE;  
		c_db->nodes[c_db->num_nodes].valid = MCAPI_TRUE;  
		c_db->nodes[c_db->num_nodes].node_num = node_num;
		c_db -> num_nodes++;
	}

	/* unlock the database */

	return rc;
}


/***************************************************************************
NAME:mcapi_trans_encode_handle_internal 
DESCRIPTION:
Our handles are very simple - a 32 bit integer is encoded with 
an index (16 bits gives us a range of 0:64K indices).
Currently, we only have 2 indices for each of: node array and
endpoint array.
PARAMETERS: 
node_index -
endpoint_index -
RETURN VALUE: the handle
 ***************************************************************************/
uint32_t mcapi_trans_encode_handle_internal (uint16_t node_index,uint16_t endpoint_index) 
{
	/* The database should already be locked */
	uint32_t handle = 0;
	uint8_t shift = 16;

	printf(" node_index=%d, port_index=%d\n", node_index, endpoint_index);

	handle = node_index;
	handle <<= shift;
	handle |= endpoint_index;

	return handle;
}

/***************************************************************************
NAME:mcapi_trans_decode_handle_internal
DESCRIPTION: Decodes the given handle into it's database indices
PARAMETERS: 
handle -
node_index -
endpoint_index -
RETURN VALUE: true/false indicating success or failure
 ***************************************************************************/
mcapi_boolean_t mcapi_trans_decode_handle_internal (uint32_t handle, uint16_t *node_index,
		uint16_t *endpoint_index) 
{
	int rc = MCAPI_FALSE;
	uint8_t shift = 16;

	/* The database should already be locked */
	*node_index              = (handle & 0xffff0000) >> shift;
	*endpoint_index          = (handle & 0x0000ffff);

	mcapi_dprintf(1, "node %d endpoint %d\n", *node_index, *endpoint_index);

	rc = MCAPI_TRUE;

	return rc;
}

void setup_request_internal (mcapi_endpoint_t send, mcapi_endpoint_t recv,
		mcapi_request_t* request, size_t size, mcapi_request_type type)
{
	if (!request)
		return;

	request->send_endpoint = send;
	request->recv_endpoint = recv;
	request->valid = MCAPI_TRUE;
	request->size = size;
	request->cancelled = MCAPI_FALSE;
	request->type = type;

}

/****************** error checking queries *************************/
/* checks if the given node is valid */
mcapi_boolean_t mcapi_trans_valid_node(mcapi_uint_t node_num)
{
  return MCAPI_TRUE;
}

/* checks to see if the port_num is a valid port_num for this system */
mcapi_boolean_t mcapi_trans_valid_port(mcapi_uint_t port_num)
{
  return MCAPI_TRUE;
}



/* checks if the endpoint handle refers to a valid endpoint */
mcapi_boolean_t mcapi_trans_valid_endpoint (mcapi_endpoint_t endpoint)
{
	uint16_t n,e;
	int index;
	int rc = MCAPI_FALSE;

	if (mcapi_trans_decode_handle_internal(endpoint,&n,&e)) {
		if (n != MASTER_NODE_NUM)
			return MCAPI_FALSE;
		index = mcapi_trans_get_port_index(n, e);
		if (index >= MAX_ENDPOINTS) {
			return MCAPI_FALSE;
		}

		rc = c_db->nodes[0].node_d.endpoints[index].valid;
	}

	mcapi_dprintf(3,"mcapi_trans_valid_endpoint endpoint=0x%llx (database indices: n=%d,e=%d) rc=%d\n",(unsigned long long)endpoint,n,e,rc);

	return rc;
}

mcapi_boolean_t mcapi_trans_valid_endpoints(mcapi_endpoint_t endpoint1,
						mcapi_endpoint_t endpoint2)
{
  return mcapi_trans_valid_endpoint(endpoint1);
}



/* checks if the channel is open for a given endpoint */
mcapi_boolean_t mcapi_trans_endpoint_channel_isopen (mcapi_endpoint_t endpoint)
{
  return MCAPI_TRUE;
}



/* checks if the channel is open for a given pktchan receive handle */
mcapi_boolean_t mcapi_trans_pktchan_recv_isopen (mcapi_pktchan_recv_hndl_t receive_handle) 
{
  return MCAPI_FALSE;
}



/* checks if the channel is open for a given pktchan send handle */
mcapi_boolean_t mcapi_trans_pktchan_send_isopen (mcapi_pktchan_send_hndl_t send_handle) 
{
  return MCAPI_FALSE;
}



/* checks if the channel is open for a given sclchan receive handle */
mcapi_boolean_t mcapi_trans_sclchan_recv_isopen (mcapi_sclchan_recv_hndl_t receive_handle) 
{
  return MCAPI_FALSE;
}



/* checks if the channel is open for a given sclchan send handle */
mcapi_boolean_t mcapi_trans_sclchan_send_isopen (mcapi_sclchan_send_hndl_t send_handle) 
{
  return MCAPI_FALSE;
}



/* checks if the given endpoint is owned by the given node */
mcapi_boolean_t mcapi_trans_endpoint_isowner (mcapi_endpoint_t endpoint)
{
  return MCAPI_FALSE;
}



channel_type mcapi_trans_channel_type (mcapi_endpoint_t endpoint)
{
  return 0;
}



mcapi_boolean_t mcapi_trans_channel_connected  (mcapi_endpoint_t endpoint)
{
	mcapi_boolean_t rc = MCAPI_FALSE;
	uint16_t n,e;
	int index;
	int ret;
	mcapi_uint_t status = 0;


	assert(mcapi_trans_decode_handle_internal(endpoint,&n,&e));

	index = mcapi_trans_get_port_index(n, e);
	if (index >= MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	rc = c_db->nodes[0].node_d.endpoints[index].connected;

	if (rc)
		return rc;
	else {
		ret = sm_get_session_status(index, NULL, NULL, &status);
		if (ret)
			return MCAPI_FALSE;
		mcapi_dprintf(1, "%s status = %d\n", __func__, status);
		if (status == 1)
			return MCAPI_TRUE;
		else
			return MCAPI_FALSE;
	}

}



mcapi_boolean_t mcapi_trans_recv_endpoint (mcapi_endpoint_t endpoint)
{
  return MCAPI_TRUE;
}



mcapi_boolean_t mcapi_trans_send_endpoint (mcapi_endpoint_t endpoint)
{
  return MCAPI_TRUE;
}



/* checks if the given endpoints have compatible attributes */
mcapi_boolean_t mcapi_trans_compatible_endpoint_attributes  (mcapi_endpoint_t send_endpoint, mcapi_endpoint_t recv_endpoint)
{
  return MCAPI_FALSE;
}



/* checks if the given channel handle is valid */
mcapi_boolean_t mcapi_trans_valid_pktchan_send_handle( mcapi_pktchan_send_hndl_t handle)
{
  return MCAPI_TRUE;
}


mcapi_boolean_t mcapi_trans_valid_pktchan_recv_handle( mcapi_pktchan_recv_hndl_t handle)
{
  return MCAPI_TRUE;
}


mcapi_boolean_t mcapi_trans_valid_sclchan_send_handle( mcapi_sclchan_send_hndl_t handle)
{
  return MCAPI_TRUE;
}


mcapi_boolean_t mcapi_trans_valid_sclchan_recv_handle( mcapi_sclchan_recv_hndl_t handle)
{
  return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_initialized (mcapi_node_t node_id)
{
  int i;
  mcapi_boolean_t rc = MCAPI_FALSE;

  if (c_db == NULL)
  	return rc;
    
    for (i = 0; i < MAX_NODES; i++) {
      if ((c_db->nodes[i].valid) && (c_db->nodes[i].node_num == node_id)) {
        rc = MCAPI_TRUE;
        break;
     }
    return rc;
  }

  printf("%s\n", __func__); 
  return MCAPI_FALSE;
}

mcapi_uint32_t mcapi_trans_num_endpoints()
{
  return 0;
}

mcapi_boolean_t mcapi_trans_valid_priority(mcapi_priority_t priority)
{
  return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_connected(mcapi_endpoint_t endpoint)
{
	uint16_t n,e;
	int index;
	int rc;
	int ret;
	mcapi_uint_t status = 0;

	assert(mcapi_trans_decode_handle_internal(endpoint,&n,&e));

	index = mcapi_trans_get_port_index(n, e);
	if (index >= MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	rc = c_db->nodes[0].node_d.endpoints[index].connected;

	if (rc)
		return rc;
	else {
		ret = sm_get_session_status(index, NULL, NULL, &status);
		if (ret)
			return MCAPI_FALSE;
		mcapi_dprintf(1, "%s status = %d\n", __func__, status);
		if (status == 1)
			return MCAPI_TRUE;
		else
			return MCAPI_FALSE;
	}
}

mcapi_boolean_t valid_status_param (mcapi_status_t* mcapi_status){
  return MCAPI_TRUE;
}

mcapi_boolean_t valid_version_param (mcapi_version_t* mcapi_version)
{
  return MCAPI_TRUE;
}

mcapi_boolean_t valid_buffer_param (void* buffer)
{
  return MCAPI_TRUE;
}

mcapi_boolean_t valid_request_param (mcapi_request_t* request)
{
  return MCAPI_TRUE;
}

mcapi_boolean_t valid_size_param (size_t* size)
{
  return MCAPI_TRUE;
}

/****************** initialization *************************/
mcapi_boolean_t mcapi_trans_initialize_() 
{
	int i;
	char *p;
	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
	int semkey = ftok(SEMKEYPATH,SEMKEYID);
	int shmkey = 0;
	mcapi_boolean_t rc = MCAPI_TRUE;

	if (!sem_id) {
		/* create the semaphore (it may already exist) */
		sem_id =  transport_sm_create_semaphore(semkey);
		if (sem_id == -1) {
			mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
			return MCAPI_FALSE;
		}
	}  

	/* lock the database */
	transport_sm_lock_semaphore(sem_id);

	if (c_db == NULL) {
		/* create the shared memory (it may already exist) */
		shmkey = ftok(SEMKEYPATH,SEMKEYID);
		transport_sm_create_shared_mem(&shm_addr,shmkey,sizeof(mcapi_database));  

		if (!shm_addr) {
			mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
			rc = MCAPI_FALSE;
		}

		c_db = shm_addr; 
		memset(c_db, 0, sizeof(mcapi_database));
		mcapi_dprintf(1, "%s %d db size %x\n", __func__, __LINE__, sizeof(mcapi_database));
		p = c_db;
		for (i = 0; i < sizeof(mcapi_database); i++) {
			assert(p[i] == 0);
		}

		mcapi_icc_node_init();
	}
	transport_sm_unlock_semaphore(sem_id);
	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
	return MCAPI_TRUE;
}

/* initialize the transport layer */
mcapi_boolean_t mcapi_trans_initialize(mcapi_uint_t node_num)
{
	sm_dev_initialize();
	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
	if (mcapi_trans_initialize_()) {
		mcapi_trans_add_node(node_num);
		return MCAPI_TRUE;
	}
	return MCAPI_FALSE;
}



/****************** tear down ******************************/
void mcapi_trans_finalize()
{
	sm_dev_finalize();
	void *shm_addr = c_db;
	uint32_t shmkey = ftok(SEMKEYPATH,SEMKEYID);
	uint32_t shmid = shmget(shmkey, sizeof(mcapi_database), 0666); 
	transport_sm_free_shared_mem(shmid,shm_addr);
}


/****************** endpoints ******************************/
/* create endpoint <node_num,port_num> and return it's handle */
mcapi_boolean_t mcapi_trans_create_endpoint(mcapi_endpoint_t *endpoint,  mcapi_uint_t port_num,mcapi_boolean_t anonymous)
{
	int node_num;
	int node_index;
	int i;
	int endpoint_index = sm_create_session(port_num, SP_PACKET);
	assert(endpoint_index >= 0);
	if (endpoint_index < 0) {
		return MCAPI_FALSE;
	}
	assert (mcapi_trans_get_node_num(&node_num));
	node_index = mcapi_trans_get_node_index(node_num);

	mcapi_dprintf(1," node index %d ep index %d\n", node_index, endpoint_index);

	assert(c_db->nodes[node_index].node_d.endpoints[endpoint_index].valid == MCAPI_FALSE);

	c_db->nodes[node_index].node_d.endpoints[endpoint_index].valid = MCAPI_TRUE;
	c_db->nodes[node_index].node_d.endpoints[endpoint_index].port_num = port_num;
	c_db->nodes[node_index].node_d.endpoints[endpoint_index].open = MCAPI_FALSE;
	c_db->nodes[node_index].node_d.endpoints[endpoint_index].anonymous = anonymous;

	c_db->nodes[node_index].node_d.num_endpoints++;


	*endpoint = mcapi_trans_encode_handle_internal(node_num, port_num);
	return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_get_endpoint_internal (mcapi_endpoint_t *e, mcapi_uint_t node_num, 
		mcapi_uint_t port_num)
{
	int i,j;
	int ret;
	int rc = MCAPI_FALSE;

	/* the database should already be locked */

	mcapi_dprintf(2," mcapi_trans_get_endpoint_internal node_num=%d, port_num=%d\n",
			node_num,port_num);

	*e = mcapi_trans_encode_handle_internal (node_num, port_num);

	rc = MCAPI_TRUE;
	return rc;
}


/* non-blocking get endpoint for the given <node_num,port_num> and set endpoint parameter to it's handle */
void mcapi_trans_get_endpoint_i(  mcapi_endpoint_t* endpoint, mcapi_uint_t node_num, mcapi_uint_t port_num,mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	if (mcapi_trans_get_endpoint_internal(endpoint, node_num, port_num))
		*mcapi_status = MCAPI_SUCCESS;
	else
		*mcapi_status = MCAPI_EPARAM;
}

/* blocking get endpoint for the given <node_num,port_num> and return it's handle */
mcapi_boolean_t mcapi_trans_get_endpoint(mcapi_endpoint_t *endpoint,mcapi_uint_t node_num, mcapi_uint_t port_num)
{
	mcapi_status_t* mcapi_status;
	mcapi_request_t request;
	mcapi_trans_get_endpoint_i(endpoint, node_num, port_num, &request, &mcapi_status);
	if (mcapi_status == MCAPI_SUCCESS)
		return MCAPI_TRUE;
	else
		return MCAPI_FALSE;
}

mcapi_boolean_t mcapi_trans_endpoint_exists(mcapi_uint_t port_num)
{
	return MCAPI_FALSE;
}

/* delete the given endpoint */
void mcapi_trans_delete_endpoint( mcapi_endpoint_t endpoint)
{
	uint16_t node_index,endpoint_index;
	assert(mcapi_trans_decode_handle_internal(endpoint,&node_index,&endpoint_index));

	c_db->nodes[node_index].node_d.endpoints[endpoint_index].valid = MCAPI_FALSE;
	c_db->nodes[node_index].node_d.endpoints[endpoint_index].port_num = 0;
	c_db->nodes[node_index].node_d.endpoints[endpoint_index].open = MCAPI_FALSE;
	c_db->nodes[node_index].node_d.endpoints[endpoint_index].anonymous = 0;

	c_db->nodes[node_index].node_d.num_endpoints--; 

	sm_destroy_session(endpoint_index);
}



/* get the attribute for the given endpoint and attribute_num */
void mcapi_trans_get_endpoint_attribute( mcapi_endpoint_t endpoint, mcapi_uint_t attribute_num, void* attribute, size_t attribute_size)
{
}



/* set the given attribute on the given endpoint */
void mcapi_trans_set_endpoint_attribute( mcapi_endpoint_t endpoint, mcapi_uint_t attribute_num, const void* attribute, size_t attribute_size)
{
}




/****************** msgs **********************************/

void mcapi_trans_msg_send_i( mcapi_endpoint_t  send_endpoint, mcapi_endpoint_t  receive_endpoint, char* buffer, size_t buffer_size, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se;
	uint16_t rn,re;
	int ret;
	int index;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	assert(mcapi_trans_decode_handle_internal(send_endpoint,&sn,&se));
	assert(mcapi_trans_decode_handle_internal(receive_endpoint,&rn,&re));

	index = mcapi_trans_get_port_index(sn, se);

	mcapi_dprintf(1,"index %d, re %d, rn %d\n", index, se, sn);

	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	mcapi_dprintf(1,"index %d, re %d, rn %d\n", index, re, rn);
	ret = sm_send_packet(index, re, rn, buffer, buffer_size);
	if (ret) {
		mcapi_dprintf(1,"send failed\n");
		*mcapi_status = ret;
	} else
		*mcapi_status = MCAPI_SUCCESS;

	setup_request_internal(send_endpoint, receive_endpoint, request, buffer_size, SEND);
}



mcapi_boolean_t mcapi_trans_msg_send( mcapi_endpoint_t  send_endpoint, mcapi_endpoint_t  receive_endpoint, char* buffer, size_t buffer_size)
{
  return MCAPI_FALSE;
}



void mcapi_trans_msg_recv_i( mcapi_endpoint_t  receive_endpoint,  char* buffer, size_t buffer_size, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se;
	uint16_t rn,re;
	int ret;
	int index;
	uint32_t len;
	mcapi_endpoint_t  send_endpoint;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	assert(mcapi_trans_decode_handle_internal(receive_endpoint,&rn,&re));

	index = mcapi_trans_get_port_index(rn, re);

	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	ret = sm_recv_packet(index, &se, &sn, buffer, &len);
	if (ret) {
		mcapi_dprintf(1,"recv failed\n");
		*mcapi_status = ret;
	} else {
	mcapi_dprintf(1,"index %d, se %d, sn %d\n", index, se, sn);
	mcapi_dprintf(1,"buffer %s\n", buffer);
		*mcapi_status = MCAPI_SUCCESS;
	}

	send_endpoint = mcapi_trans_encode_handle_internal(sn,se);

	setup_request_internal(send_endpoint, receive_endpoint, request, len, RECV);
}



mcapi_boolean_t mcapi_trans_msg_recv( mcapi_endpoint_t  receive_endpoint,  char* buffer, size_t buffer_size, size_t* received_size)
{
  return MCAPI_FALSE;
}


mcapi_uint_t mcapi_trans_msg_available( mcapi_endpoint_t receive_endpoint)
{
	uint16_t rn,re;
	int index;
	int ret;
	mcapi_uint_t avail = 0;
	assert(mcapi_trans_decode_handle_internal(receive_endpoint,&rn,&re));
	assert(rn == 0);

	index = mcapi_trans_get_port_index(rn, re);

	if (index >= MAX_ENDPOINTS) {
		return -EINVAL;
	}

	ret = sm_get_session_status(index, &avail, NULL, NULL);
	if (ret)
		return ret;
	mcapi_dprintf(1, "%s avail = %d\n", __func__, avail);
	return avail;
}


/****************** channels general ****************************/

/****************** pkt channels ****************************/
void mcapi_trans_connect_pktchan_i( mcapi_endpoint_t  send_endpoint, mcapi_endpoint_t  receive_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se,rn,re;
	int ret;
	mcapi_uint_t status = 0;
	assert(mcapi_trans_decode_handle_internal(send_endpoint,&sn,&se));
	assert(mcapi_trans_decode_handle_internal(receive_endpoint,&rn,&re));
	mcapi_dprintf(1, "%s se %d re %d rn= %d\n", __func__, se, re, rn);

	mcapi_trans_connect_channel_internal (send_endpoint,receive_endpoint,MCAPI_PKT_CHAN);
	*mcapi_status = MCAPI_TRUE;
}

void mcapi_trans_open_pktchan_recv_i( mcapi_pktchan_recv_hndl_t* recv_handle, mcapi_endpoint_t receive_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t rn,re;
	int index;
	assert(mcapi_trans_decode_handle_internal(receive_endpoint,&rn,&re));
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	if (!completed) {

		c_db->nodes[0].node_d.endpoints[index].open = MCAPI_TRUE;

		/* fill in the channel handle */
		*recv_handle = mcapi_trans_encode_handle_internal(rn,re);


		/* has the channel been connected yet? */
		if ( c_db->nodes[0].node_d.endpoints[index].type == MCAPI_PKT_CHAN) {
			completed = MCAPI_TRUE;
		}

		mcapi_dprintf(2," mcapi_trans_open_pktchan_recv_i (node_num=%d,port_num=%d) handle=%x\n",
				c_db->nodes[0].node_num,c_db->nodes[0].node_d.endpoints[index].port_num,*recv_handle); 
	}

}

void mcapi_trans_open_pktchan_send_i( mcapi_pktchan_send_hndl_t* send_handle, mcapi_endpoint_t  send_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;
	int index;
	mcapi_dprintf(1,"%s send_handle %d\n", __func__,send_endpoint);
	assert(mcapi_trans_decode_handle_internal(send_endpoint,&sn,&se));
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	if (!completed) {

		/* mark the endpoint as open */
		c_db->nodes[0].node_d.endpoints[index].open = MCAPI_TRUE;

		/* fill in the channel handle */
		*send_handle = mcapi_trans_encode_handle_internal(sn,se);

		/* has the channel been connected yet? */
		if ( c_db->nodes[0].node_d.endpoints[index].type == MCAPI_PKT_CHAN) {
			completed = MCAPI_TRUE;
		}

		mcapi_dprintf(2," mcapi_trans_open_pktchan_send_i (node_num=%d,port_num=%d) handle=%x\n",
				c_db->nodes[0].node_num,c_db->nodes[0].node_d.endpoints[index].port_num,*send_handle);
	}

}

void  mcapi_trans_pktchan_send_i( mcapi_pktchan_send_hndl_t send_handle, void* buffer, size_t size, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se;
	uint16_t rn,re;
	int ret;
	int index;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	mcapi_dprintf(1,"%s send_handle %d\n", __func__,send_handle);
	assert(mcapi_trans_decode_handle_internal(send_handle,&sn,&se));

	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	assert(mcapi_trans_decode_handle_internal(c_db->nodes[0].node_d.endpoints[index].recv_endpt,&rn,&re));
	mcapi_dprintf(1,"index %d, re %d, rn %d\n", index, re, rn);
	ret = sm_send_packet(index, re, rn, buffer, size);
	if (ret) {
		mcapi_dprintf(1,"send failed\n");
		*mcapi_status = ret;
	} else
		*mcapi_status = MCAPI_SUCCESS;

	setup_request_internal(send_handle, c_db->nodes[0].node_d.endpoints[index].recv_endpt, request, size, SEND);
}


mcapi_boolean_t  mcapi_trans_pktchan_send( mcapi_pktchan_send_hndl_t send_handle, void* buffer, size_t size)
{
  return MCAPI_FALSE;
}



void mcapi_trans_pktchan_recv_i( mcapi_pktchan_recv_hndl_t receive_handle,  void** buffer, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se;
	uint16_t rn,re;
	int ret;
	int index;
	uint32_t len;
	int i;
	mcapi_endpoint_t send_endpoint;
	buffer_entry* db_buff = NULL;

	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	assert(mcapi_trans_decode_handle_internal(receive_handle,&rn,&re));
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* find a free mcapi buffer (we only have to worry about this on the sending side) */
	for (i = 0; i < MAX_BUFFERS; i++) {
		if (!c_db->buffers[i].in_use) {
			c_db->buffers[i].in_use = MCAPI_TRUE;
			c_db->buffers[i].magic_num = MAGIC_NUM;
			db_buff = &c_db->buffers[i];
			break;
		}
	}
	if (i == MAX_BUFFERS) {
		/* we couldn't get a free buffer */
		mcapi_dprintf(2," ERROR mcapi_trans_send_internal: No more buffers available - try freeing some buffers. \n");
		return MCAPI_FALSE;
	}

	index = re;

	ret = sm_recv_packet(index,&se, &sn, db_buff->buff, &len);
	if (ret) {
		mcapi_dprintf(1,"recv failed\n");
		*mcapi_status = ret;
	}
	mcapi_dprintf(1,"index %d, se %d, sn %d\n", index, se, sn);
	mcapi_dprintf(1,"buffer %s\n", buffer);

	*buffer = db_buff->buff;
	*mcapi_status = MCAPI_SUCCESS;

	send_endpoint = mcapi_trans_encode_handle_internal(sn,se);
	setup_request_internal(send_endpoint, receive_handle, request, len, RECV);
}


mcapi_boolean_t mcapi_trans_pktchan_recv( mcapi_pktchan_recv_hndl_t receive_handle, void** buffer, size_t* received_size)
{
  return MCAPI_FALSE;
}

mcapi_uint_t mcapi_trans_pktchan_available( mcapi_pktchan_recv_hndl_t   receive_handle)
{
	mcapi_uint_t avail;
	avail = mcapi_trans_msg_available(receive_handle);
	return avail;
}

mcapi_boolean_t mcapi_trans_pktchan_free( void* buffer)
{
	int rc = MCAPI_TRUE;
	buffer_entry* b_e;

	/* optimization - just do pointer arithmetic on the buffer pointer to get
	   the base address of the buffer_entry structure. */
	b_e = buffer-9;
	if (b_e->magic_num == MAGIC_NUM) {
		memset(b_e,0,sizeof(buffer_entry));
	} else {
		/* didn't find the buffer */
		rc = MCAPI_FALSE;
	}
	return rc;
}



void mcapi_trans_pktchan_recv_close_i( mcapi_pktchan_recv_hndl_t  receive_handle,mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t rn,re;
	int index;

	assert(mcapi_trans_decode_handle_internal(receive_handle,&rn,&re));
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE; 
	if (!completed) {    

		c_db->nodes[0].node_d.endpoints[index].open = MCAPI_FALSE;
		completed = MCAPI_TRUE;    
	}  

}


void mcapi_trans_pktchan_send_close_i( mcapi_pktchan_send_hndl_t  send_handle,mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se;
	int index;

	assert(mcapi_trans_decode_handle_internal(send_handle,&sn,&se));
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	if (!completed) {
		c_db->nodes[0].node_d.endpoints[index].open = MCAPI_FALSE;
	}

}

/****************** scalar channels ****************************/
void mcapi_trans_connect_sclchan_i( mcapi_endpoint_t  send_endpoint, mcapi_endpoint_t  receive_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se,rn,re;
	int ret;
	mcapi_uint_t status = 0;
	assert(mcapi_trans_decode_handle_internal(send_endpoint,&sn,&se));
	assert(mcapi_trans_decode_handle_internal(receive_endpoint,&rn,&re));
	mcapi_dprintf(1, "%s se %d re %d rn= %d\n", __func__, se, re, rn);

	mcapi_trans_connect_channel_internal (send_endpoint,receive_endpoint,MCAPI_SCL_CHAN);
	*mcapi_status = MCAPI_TRUE;

}



void mcapi_trans_open_sclchan_recv_i( mcapi_sclchan_recv_hndl_t* recv_handle, mcapi_endpoint_t receive_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t rn,re;
	int index;
	assert(mcapi_trans_decode_handle_internal(receive_endpoint,&rn,&re));
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	if (!completed) {

		c_db->nodes[0].node_d.endpoints[index].open = MCAPI_TRUE;

		/* fill in the channel handle */
		*recv_handle = mcapi_trans_encode_handle_internal(rn,re);


		/* has the channel been connected yet? */
		if ( c_db->nodes[0].node_d.endpoints[index].type == MCAPI_SCL_CHAN) {
			completed = MCAPI_TRUE;
		}

		mcapi_dprintf(2," mcapi_trans_open_pktchan_recv_i (node_num=%d,port_num=%d) handle=%x\n",
				c_db->nodes[0].node_num,c_db->nodes[0].node_d.endpoints[index].port_num,*recv_handle); 
	}


}


void mcapi_trans_open_sclchan_send_i( mcapi_sclchan_send_hndl_t* send_handle, mcapi_endpoint_t  send_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;
	int index;
	assert(mcapi_trans_decode_handle_internal(send_endpoint,&sn,&se));
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	if (!completed) {

		/* mark the endpoint as open */
		c_db->nodes[0].node_d.endpoints[index].open = MCAPI_TRUE;

		/* fill in the channel handle */
		*send_handle = mcapi_trans_encode_handle_internal(sn,se);

		/* has the channel been connected yet? */
		if ( c_db->nodes[0].node_d.endpoints[index].type == MCAPI_SCL_CHAN) {
			completed = MCAPI_TRUE;
		}

		mcapi_dprintf(2," mcapi_trans_open_sclchan_send_i (node_num=%d,port_num=%d) handle=%x completed %d\n",
				c_db->nodes[0].node_num,c_db->nodes[0].node_d.endpoints[index].port_num,*send_handle, completed);
	}

}



mcapi_uint_t mcapi_trans_sclchan_available_i( mcapi_sclchan_recv_hndl_t receive_handle)
{
	mcapi_uint_t avail;
	avail = mcapi_trans_msg_available(receive_handle);
	return avail;
}

void mcapi_trans_sclchan_recv_close_i( mcapi_sclchan_recv_hndl_t  recv_handle,mcapi_request_t* mcapi_request,mcapi_status_t* mcapi_status)
{
	uint16_t rn,re;
	int index;

	assert(mcapi_trans_decode_handle_internal(recv_handle,&rn,&re));
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE; 
	if (!completed) {

		c_db->nodes[0].node_d.endpoints[index].open = MCAPI_FALSE;
		completed = MCAPI_TRUE;
	}

}

void mcapi_trans_sclchan_send_close_i( mcapi_sclchan_send_hndl_t send_handle,mcapi_request_t* mcapi_request,mcapi_status_t* mcapi_status)
{
	uint16_t sn,se;
	int index;

	assert(mcapi_trans_decode_handle_internal(send_handle,&sn,&se));
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	if (!completed) {
		c_db->nodes[0].node_d.endpoints[index].open = MCAPI_FALSE;
	}

}

mcapi_boolean_t mcapi_trans_sclchan_send( mcapi_sclchan_send_hndl_t send_handle,
		uint64_t dataword, uint32_t size)
{
	uint16_t sn,se,rn,re;
	uint32_t scalar0 = 0, scalar1 = 0;
	int ret;
	int index;
	int rc = MCAPI_FALSE;

	mcapi_dprintf(2,"  mcapi_trans_sclchan_send send_handle=%x\n",send_handle);

	assert(mcapi_trans_decode_handle_internal(send_handle,&sn,&se));

	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MAX_ENDPOINTS) {
		return MCAPI_FALSE;;
	}

	assert(mcapi_trans_decode_handle_internal(c_db->nodes[sn].node_d.endpoints[index].recv_endpt,&rn,&re));
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	switch (size) {
	case 1:
		scalar0 = (uint32_t)(dataword & 0xff);
		break;
	case 2:
		scalar0 = (uint32_t)(dataword & 0xffff);
		break;
	case 4:
		scalar0 = (uint32_t)(dataword & 0xffffffff);
		break;
	case 8:
		scalar1 = (uint32_t)(dataword & 0xffffffff);
		scalar0 = (uint32_t)((dataword >> 32) & 0xffffffff);
		break;
	}

	ret = sm_send_scalar(index, re, rn, scalar0, scalar1, size);
	if (ret)
		mcapi_dprintf(1,"send failed\n");
	return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_sclchan_recv( mcapi_sclchan_recv_hndl_t receive_handle,
		uint64_t *data,uint32_t size)
{
	uint16_t rn,re;
	uint16_t sn,se;
	size_t received_size;
	uint32_t scalar0 = 0, scalar1 = 0;
	uint32_t type;
	int ret;
	int index;
	int rc = MCAPI_FALSE;

	assert(mcapi_trans_decode_handle_internal(receive_handle,&rn,&re));
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	ret = sm_recv_scalar(index, &se, &sn, &scalar0, &scalar1, &type);
	if (ret) {
		mcapi_dprintf(1,"send failed\n");
	} 
	received_size = type;
	switch (received_size) {
	case 1:
	case 2:
	case 4:
		*data = scalar0;
	case 8:
		*data = ((uint64_t )scalar0 << 32) | scalar1;
	}

	if (size != received_size) {}
		return MCAPI_FALSE;

	return MCAPI_TRUE;
}


/****************** test,wait & cancel ****************************/
mcapi_boolean_t mcapi_trans_test_i( mcapi_request_t* request, size_t* size,mcapi_status_t* mcapi_status)
{
	mcapi_boolean_t rc;
	rc = MCAPI_FALSE;
	uint16_t rn,re;
	int index;
	int ret;
	mcapi_uint_t avail = 0;
	mcapi_uint_t uncomplete = 0;
	mcapi_uint_t status = 0;

	if (request->valid == MCAPI_FALSE) {
		*mcapi_status = MCAPI_ENOTREQ_HANDLE;
		rc = MCAPI_FALSE;
	} else if (request->cancelled) {
		*mcapi_status = MCAPI_EREQ_CANCELED;
		rc = MCAPI_FALSE;
	} else if (!(request->completed)) {
		/* try to complete the request */
		/*  receives to an empty channel or get_endpt for an endpt that
		    doesn't yet exist are the only two types of non-blocking functions
		    that don't complete immediately for this implementation */
		assert(mcapi_trans_decode_handle_internal(request->handle,&rn,&re));
		index = mcapi_trans_get_port_index(rn, re);
		if (index >= MAX_ENDPOINTS) {
			*mcapi_status = MCAPI_FALSE;
			return MCAPI_FALSE;
		}

		ret = sm_get_session_status(index, &avail, &uncomplete, &status);
		if (ret)
			return ret;

		switch (request->type) {
			case (RECV) :
				if (avail)
					rc = MCAPI_TRUE;
			case (SEND) :
				if (!uncomplete)
					rc = MCAPI_TRUE;
			case (GET_ENDPT) :
			default:
				assert(0);
				break;
		};
	}
	return rc;
}



mcapi_boolean_t mcapi_trans_wait( mcapi_request_t* request, size_t* size,
			mcapi_status_t* mcapi_status,  mcapi_timeout_t timeout)
{
	mcapi_timeout_t time = 0;
	mcapi_boolean_t rc;
	while(1) {
		time++;
		rc = mcapi_trans_test_i(request,size,mcapi_status);
		if (request->completed) {
			return rc;
		}
		/* yield */
		mcapi_dprintf(5," mcapi_trans_wait - attempting to yield\n");
		/* we don't have the lock, it's safe to just yield */
		sched_yield();
		if ((timeout !=  MCAPI_INFINITE) && (time >= timeout)) {
			*mcapi_status = MCAPI_EREQ_TIMEOUT;
			return MCAPI_FALSE;
		}
	}
}

mcapi_boolean_t mcapi_trans_wait_any( mcapi_request_t* request, size_t* size,mcapi_status_t* mcapi_status)
{
  return MCAPI_FALSE;
}



int mcapi_trans_wait_first( size_t number, mcapi_request_t** requests, size_t* size)
{
  return 0;
}



void mcapi_trans_cancel( mcapi_request_t* request)
{
}


