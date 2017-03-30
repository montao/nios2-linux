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

#ifndef _TRANSPORT_SM_H_
#define _TRANSPORT_SM_H_

#include "mcapi_datatypes.h"

#include <stdarg.h> /* for va_list */
#include <stdio.h> /* for the inlined dprintf routine */

#define MCAPI_MANAGE_SESSION 100

#define MCAPI_HEAD(x) ((x) >> 16 & 0xffff)
#define MCAPI_CMD(x) ((x)& 0xffff)
#define MCAPI_CMD_HEAD 0xFE
#define MCAPI_CMD_GET_ID        0x1
#define MCAPI_CMD_GET_TYPE      0x2
#define MCAPI_ACK_HEAD   0xFF

#define MK_MCAPI_CMD(x) (((x)& 0xffff) | (MCAPI_CMD_HEAD << 16))
#define MK_MCAPI_CMD_ACK(x) (((x)& 0xffff) | (MCAPI_ACK_HEAD << 16))

#define MAX_NODES 1
#define MAX_ENDPOINTS 32
#define MAX_BUFFERS 32
#define MCAPI_MAX_REQUESTS 32
/*******************************************************************
  definitions and constants
*******************************************************************/    
/* the debug level */
extern int mcapi_debug;

/* we leave one empty element so that the array implementation 
   can tell the difference between empty and full */
#define MAX_QUEUE_ENTRIES (MAX_QUEUE_ELEMENTS + 1)


/*******************************************************************
  mcapi_trans data types
*******************************************************************/    
/* buffer entry is used for msgs, pkts and scalars */
/* NOTE: if you change the buffer_entry data structure then you also
   need to update the pointer arithmetic in mcapi_trans_pktchan_free */
typedef struct {
  uint32_t magic_num;
  uint32_t size; /* size (in bytes) of the buffer */
  mcapi_boolean_t in_use;
  char buff [MAX_PKT_SIZE];
  uint64_t scalar;
} buffer_entry;

typedef struct {
  mcapi_request_t* request; /* holds a reservation for an outstanding receive request */
  buffer_entry* b;          /* the pointer to the actual buffer entry in the buffer pool */
  mcapi_boolean_t invalid;
} buffer_descriptor;

typedef struct {
  uint32_t port_num;
  mcapi_boolean_t valid;
  mcapi_boolean_t anonymous;
  mcapi_boolean_t open;
  mcapi_boolean_t connected;
  mcapi_endpoint_t recv_endpt;
  uint32_t type;
} endpoint_entry;

typedef struct {
  uint16_t num_endpoints;
  endpoint_entry endpoints[MAX_ENDPOINTS];
} node_descriptor;

typedef struct {
  uint32_t node_num;
  uint32_t node_file;
  mcapi_boolean_t finalized;
  mcapi_boolean_t valid;
  node_descriptor node_d;
} node_entry;


typedef struct {
  uint16_t num_nodes;
  node_entry nodes[MAX_NODES + MAX_NODES];
  mcapi_request_t requests[MCAPI_MAX_REQUESTS];
  buffer_entry buffers[MAX_BUFFERS];
} mcapi_database;


#define WITH_DEBUG 1
/*******************************************************************
  mcapi_trans function prototypes (public)
 *******************************************************************/    
/* initialization */
extern mcapi_boolean_t transport_sm_initialize();
/* shared memory management */
extern mcapi_boolean_t transport_sm_create_shared_mem(void** addr,uint32_t shmkey,uint32_t size);
extern mcapi_boolean_t transport_sm_free_shared_mem(uint32_t shmid,void* shm_address);
/* semaphore management */
extern uint32_t transport_sm_create_semaphore(uint32_t semkey);
extern mcapi_boolean_t transport_sm_get_semaphore(uint32_t semkey);
extern mcapi_boolean_t transport_sm_delete_semaphore (uint32_t semid);
extern mcapi_boolean_t transport_sm_lock_semaphore(uint32_t semid);
extern mcapi_boolean_t transport_sm_unlock_semaphore(uint32_t semid);
extern void transport_sm_yield_internal();


#endif
