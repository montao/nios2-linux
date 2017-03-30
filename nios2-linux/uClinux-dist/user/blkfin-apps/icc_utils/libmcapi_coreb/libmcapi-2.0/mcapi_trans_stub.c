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

#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <mcapi.h>
#include <transport_sm.h>
#include <icc.h>
#define COREB_MEMPOOL_START 0x3D00000
#define MASTER_NODE_NUM 0
#define SLAVE_NODE_NUM 1

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

unsigned mcapi_nindex = 0;
unsigned mcapi_dindex = 0;
mcapi_node_t mcapi_node_num = 0;
mcapi_domain_t mcapi_domain_id = 0;

/* the debug level */
int mcapi_debug = 7;

mcapi_endpoint_t mcapi_icc_index;

/* semaphore management */
uint32_t transport_sm_create_semaphore(uint32_t semkey) {
	return 0;
}

mcapi_boolean_t transport_sm_lock_semaphore(uint32_t semid)
{
	return 0;
}

mcapi_boolean_t transport_sm_unlock_semaphore(uint32_t semid)
{
	return 0;
}

/* shared memory management */
mcapi_boolean_t transport_sm_create_shared_mem(void** addr,uint32_t shmkey,uint32_t size) 
{
	void *buf = get_free_buffer(sizeof(mcapi_database));
	if (!buf) {
		coreb_msg("no mem\n");
		return MCAPI_FALSE;
	}
	*addr = buf;
	return MCAPI_TRUE;
}

mcapi_boolean_t transport_sm_free_shared_mem(uint32_t shmid,void *shm_address)
{
	return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_icc_node_init()
{
#if 0
	int node_index;
	mcapi_icc_index = sm_create_session(MCAPI_MANAGE_SESSION, SP_SCALAR);
	if (mcapi_icc_index < 0) {
		return  MCAPI_FALSE;
	}
	node_index = 0;

	c_db->domains[0].nodes[node_index].node_d.endpoints[mcapi_icc_index].valid = MCAPI_TRUE;
	c_db->domains[0].nodes[node_index].node_d.endpoints[mcapi_icc_index].port_num = MCAPI_MANAGE_SESSION;
	c_db->domains[0].nodes[node_index].node_d.endpoints[mcapi_icc_index].open = MCAPI_FALSE;

	c_db->domains[0].nodes[node_index].node_d.num_endpoints++;
#endif
	return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_get_domain_num(mcapi_domain_t* domain)
{
#if 0
	uint32_t d,n;
	mcapi_node_t node;

	return mcapi_trans_whoami(&node,&n,domain,&d);
#endif
	COREB_DEBUG(2, "domain name %x  domain id %x\n", c_db->domains[0].valid, c_db->domains[0].domain_id);
	if (c_db->domains[0].valid) {
		*domain = c_db->domains[0].domain_id;
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
	*node = c_db->domains[0].nodes[0].node_num;
	return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_get_port_num(mcapi_uint_t port_index, mcapi_uint_t *port_num)
{
	int rc = MCAPI_FALSE;
	int i;
	for (i = 0; i < c_db->domains[0].num_nodes ; i++) {
		if (c_db->domains[0].nodes[i].node_d.endpoints[port_index].valid) {
			*port_num = c_db->domains[0].nodes[i].node_d.endpoints[port_index].port_num;
			rc = MCAPI_TRUE;
		}
	}
	return rc;
}

mcapi_uint16_t mcapi_trans_get_node_index(mcapi_uint_t node_num)
{
  /* look up the node */
  uint32_t domain_index = 0;
  int i;
  uint32_t node_index = MCA_MAX_NODES;
  for (i = 0; i < c_db->domains[domain_index].num_nodes; i++) {
    if (c_db->domains[domain_index].nodes[i].node_num == node_num) {
      node_index = i;
      break;
    }
  }
  return node_index;
}
mcapi_uint16_t mcapi_trans_get_port_index(mcapi_uint_t node_num, mcapi_uint_t port_num)
{
	/* look up the node port*/
	int i, j;
	uint32_t domain_index = 0;
	uint32_t port_index = MCAPI_MAX_ENDPOINTS;
	for (i = 0; i < c_db->domains[domain_index].num_nodes; i++) {
		if (c_db->domains[domain_index].nodes[i].node_num == node_num) { 
			for (j = 0; j < c_db->domains[domain_index].nodes[i].node_d.num_endpoints; j++) {
				if ((c_db->domains[domain_index].nodes[i].node_d.endpoints[j].valid) && 
						(c_db->domains[domain_index].nodes[i].node_d.endpoints[j].port_num == port_num)) {
					/* return the handle */
					port_index = j;
					break;
				}
			}
		}
	}

	return port_index;
}




mcapi_boolean_t mcapi_trans_add_node (mcapi_domain_t domain_id, mcapi_uint_t node_id, const mcapi_node_attributes_t* node_attrs) 
{
	uint32_t domain_index = 0;
	mcapi_boolean_t rc = MCAPI_TRUE;
	mcapi_database* mcapi_db = c_db;
	int d = 0;
	int n = 0;
	int i = 0;


	COREB_DEBUG(2, "%s %d\n", __func__, __LINE__);
	/* lock the database */

	/* mcapi should have checked that the node doesn't already exist */

	if (mcapi_db->domains[domain_index].num_nodes == MCA_MAX_NODES) {
		rc = MCAPI_FALSE;
	}

	if (rc) {
		/* first see if this domain already exists */
		for (d = 0; d < MCA_MAX_DOMAINS; d++) {
			if (mcapi_db->domains[d].domain_id == domain_id) {
				break;
			}
		}
		if (d == MCA_MAX_DOMAINS) {
			/* it didn't exist so find the first available entry */
			for (d = 0; d < MCA_MAX_DOMAINS; d++) {
				if (mcapi_db->domains[d].valid == MCAPI_FALSE) {
					break;
				}
			}
		}
		if (d != MCA_MAX_DOMAINS) {

			/* now find an available node index...*/
			for (n = 0; n < MCA_MAX_NODES; n++) {
				/* Even though initialized() is checked by mcapi, we have to check again here because 
				   initialized() and initalize() are  not atomic at the top layer */
				if ((mcapi_db->domains[d].nodes[n].valid )&& 
						(mcapi_db->domains[d].nodes[n].node_num == node_id)) {
					/* this node already exists for this domain */
					rc = MCAPI_FALSE;
					mcapi_dprintf(1,"This node (%d) already exists for this domain(%d)",node_id,domain_id);
					COREB_DEBUG(2, "%s %d\n", __func__, __LINE__);
					break; 
				}
			}
			if (n == MCA_MAX_NODES) {
				/* it didn't exist so find the first available entry */
				for (n = 0; n < MCA_MAX_NODES; n++) {
					if (mcapi_db->domains[d].nodes[n].valid == MCAPI_FALSE)
						break;
				}
			}
		} else {
			/* we didn't find an available domain index */
			mcapi_dprintf(1,"You have hit MCA_MAX_DOMAINS, either use less domains or reconfigure with more domains");
			COREB_DEBUG(2, "%s %d\n", __func__, __LINE__);
			rc = MCAPI_FALSE;
		}

		if (n == MCA_MAX_NODES) {
			/* we didn't find an available node index */
			mcapi_dprintf(1,"You have hit MCA_MCA_MAX_NODES, either use less nodes or reconfigure with more nodes.");
			COREB_DEBUG(2, "%s %d\n", __func__, __LINE__);
			rc = MCAPI_FALSE;
		}
	}

	if (rc) {
		if (n < MCA_MAX_NODES) {
			/* add the caller to the database*/
			/* set the domain */
			mcapi_db->domains[d].domain_id = domain_id;
			mcapi_db->domains[d].valid = MCAPI_TRUE;
			/* set the node */ 
			mcapi_nindex = n;
			mcapi_node_num = node_id;
			mcapi_domain_id = domain_id;
			mcapi_dindex = d;
			mcapi_db->domains[d].nodes[n].valid = MCAPI_TRUE;
			mcapi_db->domains[d].nodes[n].node_num = node_id;
			mcapi_db->domains[d].num_nodes++;
			/* set the node attributes */
			if (node_attrs != NULL) {
				memcpy(&mcapi_db->domains[d].nodes[n].attributes,
						node_attrs,
						sizeof(mcapi_node_attributes_t));
			}
			/* initialize the attribute size for the only attribute we support */
			mcapi_db->domains[d].nodes[n].attributes.entries[MCAPI_NODE_ATTR_TYPE_REGULAR].bytes=
				sizeof(mcapi_node_attr_type_t);

			for (i = 0; i < MCAPI_MAX_ENDPOINTS; i++) {
				/* zero out all the endpoints */
				memset (&mcapi_db->domains[d].nodes[n].node_d.endpoints[i],0,sizeof(endpoint_entry));
			}
		} 
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
uint32_t mcapi_trans_encode_handle_internal (uint16_t domain_id, uint16_t node_id,uint16_t port_id) 
{
	/* The database should already be locked */
	uint32_t handle = 0;

	handle = ((domain_id & MCAPI_DOMAIN_MASK) << MCAPI_DOMAIN_SHIFT) |
			((node_id & MCAPI_NODE_MASK) << MCAPI_NODE_SHIFT) |
			((port_id & MCAPI_PORT_MASK) << MCAPI_PORT_SHIFT);

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
mcapi_boolean_t mcapi_trans_decode_handle_internal (uint32_t handle, uint16_t *domain_id, uint16_t *node_id,
		uint16_t *port_id) 
{
	int rc = MCAPI_FALSE;

	/* The database should already be locked */
	*domain_id = (handle >> MCAPI_DOMAIN_SHIFT) & MCAPI_DOMAIN_MASK;
	*node_id = (handle >> MCAPI_NODE_SHIFT) & MCAPI_NODE_MASK;
	*port_id = (handle >> MCAPI_PORT_SHIFT) & MCAPI_PORT_MASK;

	rc = MCAPI_TRUE;

	return rc;
}

void setup_request_internal (mcapi_endpoint_t send, mcapi_endpoint_t recv,
		mcapi_request_t* request, size_t size, mcapi_request_type type)
{
#if 0
	if (!request)
		return;

	COREB_DEBUG(2, "%s[%i,%i]\n", __func__, send, recv);
	request->send_endpoint = send;
	request->recv_endpoint = recv;
	request->valid = MCAPI_TRUE;
	request->size = size;
	request->cancelled = MCAPI_FALSE;
	request->type = type;

#endif
}

mcapi_boolean_t mcapi_trans_decode_request_handle(mcapi_request_t* request,uint16_t* r) 
{
	*r = *request;
	if ((*r < MCAPI_MAX_REQUESTS) && (*request & 0x80000000)) {
		return MCAPI_TRUE;
	}
	return MCAPI_FALSE;
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
	uint32_t domain_index = 0;
	uint16_t d,n,e;
	int index;
	int rc = MCAPI_FALSE;

	if (mcapi_trans_decode_handle_internal(endpoint,&d,&n,&e)) {
		if (n != SLAVE_NODE_NUM)
			return MCAPI_FALSE;
		index = mcapi_trans_get_port_index(n, e);
		if (index >= MCAPI_MAX_ENDPOINTS) {
			return MCAPI_FALSE;
		}

		rc = c_db->domains[domain_index].nodes[0].node_d.endpoints[index].valid;

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
	uint16_t d,n,e;
	int index;
	mcapi_trans_decode_handle_internal(endpoint,&d,&n,&e);
	index = mcapi_trans_get_port_index(n, e);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	return c_db->domains[0].nodes[0].node_d.endpoints[index].open;
}

/* checks if the channel is open for a given pktchan receive handle */
mcapi_boolean_t mcapi_trans_pktchan_recv_isopen (mcapi_pktchan_recv_hndl_t receive_handle) 
{ 
	return mcapi_trans_endpoint_channel_isopen((mcapi_endpoint_t)receive_handle);
}



/* checks if the channel is open for a given pktchan send handle */
mcapi_boolean_t mcapi_trans_pktchan_send_isopen (mcapi_pktchan_send_hndl_t send_handle) 
{
	return mcapi_trans_endpoint_channel_isopen((mcapi_endpoint_t)send_handle);
}



/* checks if the channel is open for a given sclchan receive handle */
mcapi_boolean_t mcapi_trans_sclchan_recv_isopen (mcapi_sclchan_recv_hndl_t receive_handle) 
{
	return mcapi_trans_endpoint_channel_isopen((mcapi_endpoint_t)receive_handle);
}



/* checks if the channel is open for a given sclchan send handle */
mcapi_boolean_t mcapi_trans_sclchan_send_isopen (mcapi_sclchan_send_hndl_t send_handle) 
{
	return mcapi_trans_endpoint_channel_isopen((mcapi_endpoint_t)send_handle);
}



/* checks if the given endpoint is owned by the given node */
mcapi_boolean_t mcapi_trans_endpoint_isowner (mcapi_endpoint_t endpoint)
{
	return MCAPI_TRUE;
}



channel_type mcapi_trans_channel_type (mcapi_endpoint_t endpoint)
{
	int index;
	uint16_t d,n,e;

	mcapi_trans_decode_handle_internal(endpoint,&d,&n,&e);

	index = mcapi_trans_get_port_index(n, e);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}
	return c_db->domains[0].nodes[n].node_d.endpoints[index].recv_queue.channel_type;
}



mcapi_boolean_t mcapi_trans_channel_connected  (mcapi_endpoint_t endpoint)
{
	mcapi_boolean_t rc = MCAPI_FALSE;
	uint16_t d,n,e;
	int index;
	int ret;
	uint32_t domain_index = 0;
	struct sm_session_status status;

	mcapi_trans_decode_handle_internal(endpoint,&d,&n,&e);

	index = mcapi_trans_get_port_index(n, e);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	rc = c_db->domains[domain_index].nodes[0].node_d.endpoints[index].connected;

	if (rc)
		return rc;
	else {
		ret = sm_get_session_status(index, &status);
		if (ret)
			return MCAPI_FALSE;

		if (status.flags == MCAPI_TRUE) {
			/* update ep status */
			c_db->domains[0].nodes[0].node_d.endpoints[index].connected = MCAPI_TRUE;
			c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.recv_endpt = status.remote_ep;

			COREB_DEBUG(2, "%s %d %d remote%d\n", __func__, __LINE__, status.flags, status.remote_ep);
			return MCAPI_TRUE;
		} else
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

mcapi_boolean_t mcapi_trans_initialized (mcapi_domain_t domain_id, mcapi_node_t node_id)
{
  int i;
  uint32_t domain_index = 0;
  mcapi_boolean_t rc = MCAPI_FALSE;

  if (c_db == NULL)
  	return rc;
    
    for (i = 0; i < MCA_MAX_NODES; i++) {
      if ((c_db->domains[domain_index].nodes[i].valid) && (c_db->domains[domain_index].nodes[i].node_num == node_id)) {
        rc = MCAPI_TRUE;
        break;
     }
    return rc;
  }

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
	uint16_t d,n,e;
	int index;
	int rc;
	int ret;
	uint32_t domain_index = 0;
	struct sm_session_status status;

	mcapi_trans_decode_handle_internal(endpoint,&d,&n,&e);

	index = mcapi_trans_get_port_index(n, e);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	rc = c_db->domains[domain_index].nodes[0].node_d.endpoints[index].connected;

	if (rc)
		return rc;
	else {
		ret = sm_get_session_status(index, &status);
		if (ret)
			return MCAPI_FALSE;

		COREB_DEBUG(2, "%s %d %d remote%d\n", __func__, __LINE__, status.flags, status.remote_ep);
		if (status.flags == MCAPI_TRUE) {
			/* update ep status */
			c_db->domains[0].nodes[0].node_d.endpoints[index].connected = MCAPI_TRUE;
			c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.recv_endpt = status.remote_ep;

			return MCAPI_TRUE;
		} else
			return MCAPI_FALSE;
	}
}

mcapi_boolean_t mcapi_trans_valid_version_param (mcapi_info_t* mcapi_version)
{
	return (mcapi_version != NULL);
}

mcapi_boolean_t mcapi_trans_valid_status_param (mcapi_status_t* mcapi_status)
{
	return (mcapi_status != NULL);
}

mcapi_boolean_t mcapi_trans_valid_buffer_param (void* buffer)
{
	return (buffer != NULL);
}


mcapi_boolean_t valid_request_param (mcapi_request_t* request)
{
  return MCAPI_TRUE;
}

mcapi_boolean_t valid_size_param (size_t* size)
{
  return MCAPI_TRUE;
}

mcapi_boolean_t mcapi_trans_valid_request_handle (mcapi_request_t* request)
{
	uint16_t r;
	return (mcapi_trans_decode_request_handle(request,&r));
}

mcapi_boolean_t mcapi_trans_valid_size_param (size_t* size)
{
	return (size != NULL);
}

/****************** initialization *************************/
mcapi_boolean_t mcapi_trans_initialize_() 
{
	int i;
	char *p;
	COREB_DEBUG(2, "%s %d\n", __func__, __LINE__);
	int semkey = 0;
	int shmkey = 0;
	mcapi_boolean_t rc = MCAPI_TRUE;

	/* lock the database */
	transport_sm_lock_semaphore(sem_id);

	if (c_db == NULL) {
		transport_sm_create_shared_mem(&shm_addr,shmkey,sizeof(mcapi_database));  

		if (!shm_addr) {
			mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);

			COREB_DEBUG(2, "%s %d\n", __func__, __LINE__);
			rc = MCAPI_FALSE;
		}

		c_db = shm_addr; 
		memset(c_db, 0, sizeof(mcapi_database));
		COREB_DEBUG(2, "%s %d db size %x\n", __func__, __LINE__, sizeof(mcapi_database));

		/* mcapi_icc_node_init(); */
	}
	transport_sm_unlock_semaphore(sem_id);
	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
	COREB_DEBUG(2, "%s %d\n", __func__, __LINE__);
	return MCAPI_TRUE;
}

/* initialize the transport layer */
mcapi_boolean_t mcapi_trans_initialize(mca_domain_t domain_id,mcapi_node_t node_num,const mcapi_node_attributes_t* node_attrs)
{
	COREB_DEBUG(2, "%s %d\n", __func__, __LINE__);
	if (mcapi_trans_initialize_()) {
		mcapi_trans_add_node(domain_id, node_num, node_attrs);
		return MCAPI_TRUE;
	}
	return MCAPI_FALSE;
}



/****************** tear down ******************************/
void mcapi_trans_finalize()
{
}


/****************** endpoints ******************************/
/* create endpoint <node_num,port_num> and return it's handle */
mcapi_boolean_t mcapi_trans_endpoint_create(mcapi_endpoint_t *endpoint,  mcapi_uint_t port_num,mcapi_boolean_t anonymous)
{
	int node_num;
	uint32_t node_index;
	int i;
	uint32_t domain_index = 0;
	mcapi_database *mcapi_db = c_db;
	int endpoint_index = sm_create_session(port_num, SP_PACKET);
	if (endpoint_index < 0) {
		return MCAPI_FALSE;
	}
	mcapi_trans_get_node_num(&node_num);
	node_index = mcapi_trans_get_node_index(node_num);

	COREB_DEBUG(2, " node index %d ep index %d\n", node_index, endpoint_index);
	if (mcapi_db->domains[domain_index].nodes[node_index].node_d.endpoints[endpoint_index].valid == MCAPI_FALSE) {

	/* initialize the endpoint entry*/  
	mcapi_db->domains[domain_index].nodes[node_index].node_d.endpoints[endpoint_index].valid = MCAPI_TRUE;
	mcapi_db->domains[domain_index].nodes[node_index].node_d.endpoints[endpoint_index].port_num = port_num;
	mcapi_db->domains[domain_index].nodes[node_index].node_d.endpoints[endpoint_index].open = MCAPI_FALSE;
	mcapi_db->domains[domain_index].nodes[node_index].node_d.endpoints[endpoint_index].anonymous = anonymous;
	mcapi_db->domains[domain_index].nodes[node_index].node_d.endpoints[endpoint_index].num_attributes = 0;

	mcapi_db->domains[domain_index].nodes[node_index].node_d.num_endpoints++; 

	*endpoint = mcapi_trans_encode_handle_internal(0, node_num, port_num);
	return MCAPI_TRUE;
	} else
	return MCAPI_FALSE;

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

	*e = mcapi_trans_encode_handle_internal (0, node_num, port_num);

	rc = MCAPI_TRUE;
	return rc;
}

/* non-blocking get endpoint for the given <node_num,port_num> and set endpoint parameter to it's handle */
void mcapi_trans_endpoint_get_i(  mcapi_endpoint_t* endpoint, mcapi_uint_t node_num, mcapi_uint_t port_num,mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	if (mcapi_trans_get_endpoint_internal(endpoint, node_num, port_num))
		*mcapi_status = MCAPI_SUCCESS;
	else
		*mcapi_status = MCAPI_ERR_PARAMETER;
}

/* blocking get endpoint for the given <node_num,port_num> and return it's handle */
mcapi_boolean_t mcapi_trans_endpoint_get(mcapi_endpoint_t *endpoint,mcapi_domain_t domain_num, mcapi_node_t node_num, mcapi_port_t port_num)
{
	mcapi_status_t* mcapi_status;
	mcapi_request_t request;
	mcapi_trans_endpoint_get_i(endpoint, node_num, port_num, &request, &mcapi_status);
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
void mcapi_trans_endpoint_delete( mcapi_endpoint_t endpoint)
{
	uint16_t d,n,e;
	uint16_t index;
	mcapi_trans_decode_handle_internal(endpoint,&d,&n,&e);

	index = mcapi_trans_get_port_index(n, e);
	COREB_DEBUG(2, "%s %d d %d n %d e %d index %d\n", __func__, __LINE__,d,n,e,index);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}
	sm_destroy_session(index);
}



/* get the attribute for the given endpoint and attribute_num */
void mcapi_trans_endpoint_get_attribute( mcapi_endpoint_t endpoint, mcapi_uint_t attribute_num, void* attribute, size_t attribute_size)
{
}



/* set the given attribute on the given endpoint */
void mcapi_trans_endpoint_set_attribute( mcapi_endpoint_t endpoint, mcapi_uint_t attribute_num, const void* attribute, size_t attribute_size)
{
}

mcapi_boolean_t mcapi_trans_node_set_attribute(
                mcapi_node_attributes_t* mcapi_node_attributes,
                mcapi_uint_t attribute_num,
                const void* attribute,
                size_t attribute_size,
                mcapi_status_t* mcapi_status
                ){
        mcapi_boolean_t rc = MCAPI_FALSE;

        if (attribute_num != MCAPI_NODE_ATTR_TYPE_REGULAR) {
                /* only the node_attr_type attribute is currently supported */
                *mcapi_status = MCAPI_ERR_ATTR_NOTSUPPORTED;
        } else if (attribute_size != sizeof(mcapi_node_attr_type_t) ) {
                *mcapi_status = MCAPI_ERR_ATTR_SIZE;
        } else {
                rc = MCAPI_TRUE;
                /* copy the attribute into the attributes data structure */
                memcpy(&mcapi_node_attributes->entries[attribute_num].attribute_d,
                                attribute,
                                attribute_size);
        }
        return rc;
}

mcapi_boolean_t mcapi_trans_node_get_attribute(
                mcapi_domain_t domain_id,
                mcapi_node_t node_id,
                mcapi_uint_t attribute_num,
                void* attribute,
                size_t attribute_size,
                mcapi_status_t* mcapi_status) {
        mcapi_boolean_t rc = MCAPI_FALSE;
        mcapi_boolean_t found_node = MCAPI_FALSE;
        mcapi_boolean_t found_domain = MCAPI_FALSE;
        uint32_t d,n;
        mcapi_domain_t my_domain_id;
        mcapi_node_t my_node_id;
        size_t size;
        mcapi_database *mcapi_db = c_db;

        // look for the <domain,node>
        for (d = 0; ((d < MCA_MAX_DOMAINS) && (found_domain == MCAPI_FALSE)); d++) {
                if (mcapi_db->domains[d].domain_id == domain_id) {
                        found_domain = MCAPI_TRUE;
                        for (n = 0; ((n < MCA_MAX_NODES) &&  (found_node == MCAPI_FALSE)); n++) {
                                if (mcapi_db->domains[d].nodes[n].node_num == node_id) { 
                                        found_node = MCAPI_TRUE;
                                        if (!mcapi_db->domains[d].valid) {
                                                *mcapi_status = MCAPI_ERR_DOMAIN_INVALID;
                                        } else if (!mcapi_db->domains[d].nodes[n].valid) {
                                                *mcapi_status = MCAPI_ERR_NODE_INVALID;
                                        } else {
                                                size = mcapi_db->domains[d].nodes[n].attributes.entries[attribute_num].bytes;
                                                if (size != attribute_size) {
                                                        *mcapi_status = MCAPI_ERR_ATTR_SIZE;
                                                } else {
                                                        memcpy(attribute,
                                                                        &mcapi_db->domains[d].nodes[n].attributes.entries[attribute_num].attribute_d,
                                                                        size);
                                                        rc = MCAPI_TRUE;
                                                }
                                        }
                                }
                        }
                }
        }
        if (!found_domain) {
                *mcapi_status = MCAPI_ERR_DOMAIN_INVALID;
        } else if (!found_node) {
                *mcapi_status = MCAPI_ERR_NODE_INVALID;
        }
        return rc;
}


mcapi_boolean_t mcapi_trans_node_init_attributes(
		mcapi_node_attributes_t* mcapi_node_attributes,
		mcapi_status_t* mcapi_status
		) {
	mcapi_boolean_t rc = MCAPI_TRUE;
	/* default values are all 0 */
	memset(mcapi_node_attributes,0,sizeof(mcapi_node_attributes));
	return rc;
}



/****************** msgs **********************************/
void mcapi_trans_msg_send_i( mcapi_endpoint_t  send_endpoint, mcapi_endpoint_t  receive_endpoint, char* buffer, size_t buffer_size, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se;
	uint16_t rd,rn,re;
	int ret;
	int index;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	mcapi_trans_decode_handle_internal(send_endpoint,&sd,&sn,&se);
	mcapi_trans_decode_handle_internal(receive_endpoint,&rd,&rn,&re);

	index = mcapi_trans_get_port_index(sn, se);

	COREB_DEBUG(2, "index %d, se %d, sn %d\n", index, se, sn);

	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	mcapi_dprintf(1,"index %d, re %d, rn %d%d\n", index, re, rn);
	COREB_DEBUG(2, "index %d, re %d, rn %d\n", index, re, rn);
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
	uint16_t sd,sn,se;
	uint16_t rd,rn,re;
	int ret;
	int index;
	uint32_t len;
	mcapi_endpoint_t  send_endpoint;
	char *buf = NULL;
	mcapi_uint_t *port_num;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	mcapi_trans_decode_handle_internal(receive_endpoint,&rd,&rn,&re);

	index = mcapi_trans_get_port_index(rn, re);

	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	ret = sm_recv_packet(index,&se, &sn, &buf, &len);
	if (ret < 0) {
		mcapi_dprintf(1,"recv failed\n");
		*mcapi_status = ret;
		return;
	}
	if (buf) {
		memcpy(buffer, buf, len);
		sm_recv_release(buf, len, index);
		*mcapi_status = MCAPI_SUCCESS;
	} else
		*mcapi_status = MCAPI_FALSE;

	send_endpoint = mcapi_trans_encode_handle_internal(0,sn,se);

	setup_request_internal(send_endpoint, receive_endpoint, request, len, RECV);
}



mcapi_boolean_t mcapi_trans_msg_recv( mcapi_endpoint_t  receive_endpoint,  char* buffer, size_t buffer_size, size_t* received_size)
{
  return MCAPI_FALSE;
}



mcapi_uint_t mcapi_trans_msg_available( mcapi_endpoint_t receive_endpoint)
{
	uint16_t rd,rn,re;
	int index;
	int ret;
	struct sm_session_status status;
	mcapi_trans_decode_handle_internal(receive_endpoint,&rd,&rn,&re);
	index = mcapi_trans_get_port_index(rn, re);

	if (index >= MCAPI_MAX_ENDPOINTS) {
		return -EINVAL;
	}

	ret = sm_get_session_status(index, &status);
	if (ret)
		return ret;
	mcapi_dprintf(1, "%s avail = %d\n", __func__, status.n_avail);
	return status.n_avail;
}


/****************** channels general ****************************/

/****************** pkt channels ****************************/
void mcapi_trans_pktchan_connect_i( mcapi_endpoint_t  send_endpoint, mcapi_endpoint_t  receive_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se,rd,rn,re;
	int ret;
	int index;
	mcapi_uint_t status = 0;
	mcapi_trans_decode_handle_internal(send_endpoint,&sd,&sn,&se);
	mcapi_trans_decode_handle_internal(receive_endpoint,&rd,&rn,&re);
	mcapi_dprintf(1, "%s se %d re %d rn= %d\n", __func__, se, re, rn);
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}
	ret = sm_connect_session(index, re, rn, SP_SESSION_PACKET);
	if (ret)
		*mcapi_status = MCAPI_FALSE;
	*mcapi_status = MCAPI_TRUE;
	/* update the send endpoint */
	c_db->domains[0].nodes[0].node_d.endpoints[index].connected = MCAPI_TRUE;
	c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.recv_endpt = receive_endpoint;
	c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.channel_type = MCAPI_PKT_CHAN;
}



void mcapi_trans_pktchan_recv_open_i( mcapi_pktchan_recv_hndl_t* recv_handle, mcapi_endpoint_t receive_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
        uint16_t rd,rn,re;
        int index;

        /* if errors were found at the mcapi layer, then the request is considered complete */
        mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

        mcapi_trans_decode_handle_internal(receive_endpoint,&rd,&rn,&re);
        index = mcapi_trans_get_port_index(rn, re);
        if (index >= MCAPI_MAX_ENDPOINTS) {
                *mcapi_status = MCAPI_FALSE;
                return;
        }


        if (!completed) {

                c_db->domains[0].nodes[0].node_d.endpoints[index].open = MCAPI_TRUE;

                /* fill in the channel handle */
                *recv_handle = mcapi_trans_encode_handle_internal(0,rn,re);


                /* has the channel been connected yet? */
                if ( c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.channel_type == MCAPI_PKT_CHAN) {
                        completed = MCAPI_TRUE;
                }

        }
}

void mcapi_trans_pktchan_send_open_i( mcapi_pktchan_send_hndl_t* send_handle, mcapi_endpoint_t  send_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;
	int index;
	mcapi_trans_decode_handle_internal(send_endpoint,&sd,&sn,&se);
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	if (!completed) {

		/* mark the endpoint as open */
		c_db->domains[0].nodes[0].node_d.endpoints[index].open = MCAPI_TRUE;

		/* fill in the channel handle */
		*send_handle = mcapi_trans_encode_handle_internal(0,sn,se);

		/* has the channel been connected yet? */
		if ( c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.channel_type == MCAPI_PKT_CHAN) {
			completed = MCAPI_TRUE;
			*mcapi_status = MCAPI_TRUE;
		}

	}

}

void  mcapi_trans_pktchan_send_i( mcapi_pktchan_send_hndl_t send_handle, void* buffer, size_t size, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se;
	uint16_t rd,rn,re;
	int ret;
	int index;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	mcapi_trans_decode_handle_internal(send_handle,&sd,&sn,&se);

	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	mcapi_trans_decode_handle_internal(c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.recv_endpt,&rd,&rn,&re);
	mcapi_dprintf(1,"index %d, re %d, rn %d\n", index, re, rn);
	ret = sm_send_packet(index, re, rn, buffer, size);
	if (ret) {
		mcapi_dprintf(1,"send failed\n");
		*mcapi_status = ret;
	} else
		*mcapi_status = MCAPI_SUCCESS;

	setup_request_internal(send_handle, c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.recv_endpt, request, size, SEND);
}


mcapi_boolean_t  mcapi_trans_pktchan_send( mcapi_pktchan_send_hndl_t send_handle, void* buffer, size_t size)
{
  return MCAPI_FALSE;
}



void mcapi_trans_pktchan_recv_i( mcapi_pktchan_recv_hndl_t receive_handle,  void** buffer, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se;
	uint16_t rd,rn,re;
	int ret;
	int index;
	uint32_t len;
	int i;
	mcapi_endpoint_t send_endpoint;
	buffer_entry* db_buff = NULL;
	void *buf = NULL;

	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	mcapi_trans_decode_handle_internal(receive_handle,&rd,&rn,&re);
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}


	ret = sm_recv_packet(index,&se, &sn, &buf, &len);
	if (ret < 0) {
		mcapi_dprintf(1,"recv failed\n");
		*mcapi_status = ret;
		return;
	}
	COREB_DEBUG(2, "index %d, se %d, sn %d\n", index, se, sn);

	/* find a free mcapi buffer (we only have to worry about this on the sending side) */
	for (i = 0; i < MCAPI_MAX_BUFFERS; i++) {
		if (!c_db->buffers[i].magic_num) {
			c_db->buffers[i].magic_num = MAGIC_NUM;
			db_buff = &c_db->buffers[i];
			break;
		}
	}
	if (i == MCAPI_MAX_BUFFERS) {
		/* we couldn't get a free buffer */
		mcapi_dprintf(2," ERROR mcapi_trans_send_internal: No more buffers available - try freeing some buffers. \n");
		COREB_DEBUG(2, "pktchan buffer is full\n");
		*mcapi_status = MCAPI_FALSE;
		return;
	}


	if (buf) {
		db_buff->paddr = (uint32_t)buf;
		db_buff->size = len;
		db_buff->id = index;
		*buffer = buf;
		*mcapi_status = MCAPI_SUCCESS;
	} else
		*mcapi_status = MCAPI_FALSE;


	send_endpoint = mcapi_trans_encode_handle_internal(0,sn,se);
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
	int i;

	/* optimization - just do pointer arithmetic on the buffer pointer to get
	   the base address of the buffer_entry structure. */
	for (i = 0; i < MCAPI_MAX_BUFFERS; i++) {
		if ((c_db->buffers[i].magic_num == MAGIC_NUM) 
			|| (c_db->buffers[i].paddr == (uint32_t)buffer)) {
			b_e = &c_db->buffers[i];
			sm_recv_release(buffer, b_e->size, b_e->id);
			memset(b_e,0,sizeof(buffer_entry));
			return rc;
		}
	}

	if (i == MCAPI_MAX_BUFFERS) {
		/* didn't find the buffer */
		COREB_DEBUG(2, "didn't find%08x\n", buffer);
		rc = MCAPI_FALSE;
	}

	return rc;
}



void mcapi_trans_pktchan_recv_close_i( mcapi_pktchan_recv_hndl_t  receive_handle,mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t rd,rn,re;
	int index;

	mcapi_trans_decode_handle_internal(receive_handle,&rd,&rn,&re);
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE; 
	if (!completed) {    

		c_db->domains[0].nodes[0].node_d.endpoints[index].open = MCAPI_FALSE;
		completed = MCAPI_TRUE;    
	}  

}



void mcapi_trans_pktchan_send_close_i( mcapi_pktchan_send_hndl_t  send_handle,mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se;
	int index;

	mcapi_trans_decode_handle_internal(send_handle,&sd,&sn,&se);
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	if (!completed) {
		c_db->domains[0].nodes[0].node_d.endpoints[index].open = MCAPI_FALSE;
	}

}



/****************** scalar channels ****************************/
void mcapi_trans_sclchan_connect_i( mcapi_endpoint_t  send_endpoint, mcapi_endpoint_t  receive_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se,rd,rn,re;
	int index;
	int ret;
	mcapi_uint_t status = 0;
	mcapi_trans_decode_handle_internal(send_endpoint,&sd,&sn,&se);
	mcapi_trans_decode_handle_internal(receive_endpoint,&rd,&rn,&re);
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}
	ret = sm_connect_session(index, re, rn, SP_SESSION_SCALAR);
	if (ret)
		*mcapi_status = MCAPI_FALSE;
	*mcapi_status = MCAPI_TRUE;
	/* update the send endpoint */
	COREB_DEBUG(2, "%s index %d ep%d\n", __func__, index, receive_endpoint);
	c_db->domains[0].nodes[0].node_d.endpoints[index].connected = MCAPI_TRUE;
	c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.recv_endpt = receive_endpoint;
	c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.channel_type = MCAPI_SCL_CHAN;

}



void mcapi_trans_sclchan_recv_open_i( mcapi_sclchan_recv_hndl_t* recv_handle, mcapi_endpoint_t receive_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t rd,rn,re;
	int index;
	mcapi_trans_decode_handle_internal(receive_endpoint,&rd,&rn,&re);
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	if (!completed) {

		c_db->domains[0].nodes[0].node_d.endpoints[index].open = MCAPI_TRUE;

		/* fill in the channel handle */
		*recv_handle = mcapi_trans_encode_handle_internal(0,rn,re);


		/* has the channel been connected yet? */
		if ( c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.channel_type == MCAPI_SCL_CHAN) {
			completed = MCAPI_TRUE;
		}

	}
}



void mcapi_trans_sclchan_send_open_i( mcapi_sclchan_send_hndl_t* send_handle, mcapi_endpoint_t  send_endpoint, mcapi_request_t* request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se;
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;
	int index;
	mcapi_trans_decode_handle_internal(send_endpoint,&sd,&sn,&se);
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	COREB_DEBUG(2, "  %s %d\n",__func__, __LINE__);
	if (!completed) {

		/* mark the endpoint as open */
		c_db->domains[0].nodes[0].node_d.endpoints[index].open = MCAPI_TRUE;

		/* fill in the channel handle */
		*send_handle = mcapi_trans_encode_handle_internal(0,sn,se);

		/* has the channel been connected yet? */
		if ( c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.channel_type == MCAPI_SCL_CHAN) {
			completed = MCAPI_TRUE;
			*mcapi_status = MCAPI_TRUE;
		}

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
	uint16_t rd,rn,re;
	int index;

	mcapi_trans_decode_handle_internal(recv_handle,&rd,&rn,&re);
	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE; 
	if (!completed) {

		c_db->domains[0].nodes[0].node_d.endpoints[index].open = MCAPI_FALSE;
		completed = MCAPI_TRUE;
	}
}

void mcapi_trans_sclchan_send_close_i( mcapi_sclchan_send_hndl_t send_handle,mcapi_request_t* mcapi_request,mcapi_status_t* mcapi_status)
{
	uint16_t sd,sn,se;
	int index;

	mcapi_trans_decode_handle_internal(send_handle,&sd,&sn,&se);
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		*mcapi_status = MCAPI_FALSE;
		return;
	}

	/* if errors were found at the mcapi layer, then the request is considered complete */
	mcapi_boolean_t completed =  (*mcapi_status == MCAPI_SUCCESS) ? MCAPI_FALSE : MCAPI_TRUE;

	if (!completed) {
		c_db->domains[0].nodes[0].node_d.endpoints[index].open = MCAPI_FALSE;
	}
}

mcapi_boolean_t mcapi_trans_sclchan_send( mcapi_sclchan_send_hndl_t send_handle,
		uint64_t dataword, uint32_t size)
{
	uint16_t sd,sn,se,rd,rn,re;
	uint32_t scalar0 = 0, scalar1 = 0;
	int ret;
	int index;
	int rc = MCAPI_FALSE;

	mcapi_trans_decode_handle_internal(send_handle,&sd,&sn,&se);
	index = mcapi_trans_get_port_index(sn, se);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	COREB_DEBUG(2, "mcapi_trans_sclchan_send %d %d %d %d\n",sd, sn, se, c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.recv_endpt);
	mcapi_trans_decode_handle_internal(c_db->domains[0].nodes[0].node_d.endpoints[index].recv_queue.recv_endpt,&rd,&rn,&re);

	COREB_DEBUG(2, "send_handle=%x index%d size %d %d %d %d\n",send_handle, index, size,rd,rn,re);

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
	uint16_t rd,rn,re;
	uint16_t sd,sn,se;
	size_t received_size;
	uint32_t scalar0 = 0, scalar1 = 0;
	uint32_t type;
	int ret;
	int index;
	int rc = MCAPI_FALSE;

	mcapi_trans_decode_handle_internal(receive_handle,&rd,&rn,&re);

	index = mcapi_trans_get_port_index(rn, re);
	if (index >= MCAPI_MAX_ENDPOINTS) {
		return MCAPI_FALSE;
	}

	ret = sm_recv_scalar(index, &se, &sn, &scalar0, &scalar1, &type);
	if (ret != 1) {
		COREB_DEBUG(2, "receive falied %d\n", ret);
		return MCAPI_FALSE;
	} 
	received_size = type;

	switch (received_size) {
	case 1:
	case 2:
	case 4:
		*data = (uint64_t)scalar0;
		break;
	case 8:
		*data = ((uint64_t)scalar0) << 32 | scalar1;
		break;
	default:
		return MCAPI_FALSE;
	}

	COREB_DEBUG(2, "[%llx] size %d received_size %d\n", *data, size, received_size);
	if (size != received_size)
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
	struct sm_session_status status;

#if 0
	if (request->valid == MCAPI_FALSE) {
		*mcapi_status = MCAPI_ERR_REQUEST_INVALID;
		rc = MCAPI_FALSE;
	} else if (request->cancelled) {
		*mcapi_status = MCAPI_ERR_REQUEST_CANCELLED;
		rc = MCAPI_FALSE;
	} else if (!(request->completed)) {
		/* try to complete the request */
		/*  receives to an empty channel or get_endpt for an endpt that
		    doesn't yet exist are the only two types of non-blocking functions
		    that don't complete immediately for this implementation */
		mcapi_trans_decode_handle_internal(request->handle,&rn,&re);
		index = mcapi_trans_get_port_index(rn, re);
		if (index >= MCAPI_MAX_ENDPOINTS) {
			*mcapi_status = MCAPI_FALSE;
			return MCAPI_FALSE;
		}

		ret = sm_get_session_status(index, &status);
		if (ret)
			return ret;

		switch (request->type) {
			case (RECV) :
				if (status.n_avail)
					rc = MCAPI_TRUE;
			case (SEND) :
				if (!status.n_uncompleted)
					rc = MCAPI_TRUE;
			case (GET_ENDPT) :
			default:
				break;
		};
	}
#endif
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
		/* yield */
		mcapi_dprintf(5," mcapi_trans_wait - attempting to yield\n");
		/* we don't have the lock, it's safe to just yield */
		if ((timeout !=  MCAPI_TIMEOUT_INFINITE) && (time >= timeout)) {
			*mcapi_status = MCAPI_ERR_REQUEST_INVALID;
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


