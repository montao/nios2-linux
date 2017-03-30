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

#include <mcapi.h>
#include <mcapi_trans.h>
#include <mcapi_dev_impl.h>
#include <config.h> /* for MCAPI version */

#include <string.h> /* for strncpy */

/***********************************************************************
NAME
mcapi_initialize - Initializes the MCAPI implementation.
DESCRIPTION
mcapi_initialize() initializes the MCAPI environment on a given MCAPI node. 
It has to be called by each node using MCAPI.  mcapi_version is set to the 
to the implementation version number. A node is a process, a thread, or a 
processor (or core) with an independent program counter running a piece 
of code. In other words, an MCAPI node is an independent thread of 
control. An MCAPI node can call mcapi_initialize() once per node, and it 
is an error to call mcapi_initialize() multiple times from a given node. 
A given MCAPI implementation will specify what is a node (i.e., what 
thread of control - process, thread, or other -- is a node)  in that  
implementation. A thread and process are just two examples of threads 
of control, and there could be other. 
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS. On error, 
*mcapi_status is set to the appropriate error defined below.
ERRORS
MCAPI_ENO_INIT The MCAPI environment could not be initialized.
MCAPI_INITIALIZED The MCAPI environment has already been initialized.
MCAPI_ENODE_NOTVALID The parameter is not a valid node.
MCAPI_EPARAM Incorrect mcapi_status or mcapi_version parameter.
NOTE
SEE ALSO 
mcapi_finalize()	
************************************************************************/
void mcapi_initialize(
                      MCAPI_IN mcapi_node_t node_id, 
                      MCAPI_OUT mcapi_version_t* mcapi_version, 
                      MCAPI_OUT mcapi_status_t* mcapi_status)
{
	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
  if (!valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
    *mcapi_status = MCAPI_SUCCESS;
    if (!valid_version_param(mcapi_version)) {

	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);

      *mcapi_status = MCAPI_EPARAM;
    } else {

	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
      (void)strncpy(*mcapi_version,MCAPI_VERSION,sizeof(MCAPI_VERSION));
      if (!mcapi_trans_valid_node(node_id)) {

	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
        *mcapi_status = MCAPI_ENODE_NOTVALID;
      } else if (mcapi_trans_initialized(node_id)) {

	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
        *mcapi_status = MCAPI_INITIALIZED;
      } else if (!mcapi_trans_initialize(node_id)) {

	mcapi_dprintf(1, "%s %d\n", __func__, __LINE__);
        *mcapi_status = MCAPI_ENO_INIT;
      } 
    }
  }
}

/***********************************************************************
NAME
mcapi_finalize - Finalizes the MCAPI implementation.
DESCRIPTION
mcapi_finalize() finalizes the MCAPI environment on a given MCAPI node. 
It has to be called by each node using MCAPI.  It is an error to call 
mcapi_finalize() without first calling mcapi_initialize().  An MCAPI 
node can call mcapi_finalize() once for each call to 
mcapi_initialize(), but it is an error to call mcapi_finalize() 
multiple times from a given node unless mcapi_initialize() has been 
called prior to each mcapi_finalize() call.
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS. On error, 
*mcapi_status is set to the appropriate error defined below.
ERRORS
MCAPI_ENO_FINAL The MCAPI environment could not be finalized.
MCAPI_EPARAM  Incorrect mcapi_status_ parameter.
NOTE
SEE ALSO
   mcapi_initialize()
************************************************************************/
void mcapi_finalize(
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! mcapi_trans_finalize()) {
      *mcapi_status = MCAPI_ENO_FINAL;
    }
  }
}

/***********************************************************************
NAME
mcapi_get_node_id - return the node number associated with the local node
DESCRIPTION
Returns the node id associated with the local node.
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS. On error, 
*mcapi_status is set to the appropriate error defined below.
ERRORS
MCAPI_ENODE_NOTINIT The node is not initialized.
MCAPI_EPARAM Incorrect mcapi_status parameter.
NOTE
SEE ALSO 
************************************************************************/
mcapi_uint_t mcapi_get_node_id(
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{
  mcapi_uint_t node;
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if  (!mcapi_trans_get_node_num(&node)) {
      *mcapi_status = MCAPI_ENODE_NOTINIT;
    } 
  }
  
  return node;
}

/***********************************************************************
NAME
mcapi_create_endpoint - create an endpoint.
DESCRIPTION
mcapi_create_endpoint() is used to create endpoints, using the node_id 
of the local node calling the API function and specific port_id, 
returning a reference to a globally unique endpoint which can later be 
referenced by name using mcapi_get_endpoint() (see Section 4.2.3).  The 
port_id can be set to MCAPI_PORT ANY to request the next available 
endpoint on the local node. 
MCAPI supports a simple static naming scheme to create endpoints based 
on global tuple names. Other nodes can access the created endpoint by 
calling mcapi_get_endpoint() and specifying the appropriate node and 
port id's. Enpoints can be passed on to other endpoints and an 
endpoint created using MCAPI_PORT ANY has to be passed on to other 
endpoints by the creator, to facilitate communication.
Static naming allows the programmer to define an MCAPI communication 
topology at compile time.  This facilitates simple initialization. 
Section 7.1 illustrates an example of initialization and bootstrapping 
using static naming. Creating endpoints using MCAPI_PORT ANY provides 
a convenient method to create endpoints without having to specify the 
port_id. 
RETURN VALUE
On success, an endpoint is returned and *mcapi_status is set to 
MCAPI_SUCCESS. On error, MCAPI_NULL is returned and *mcapi_status is 
set to the appropriate error defined below.
ERRORS
MCAPI_EPORT_NOTVALID The parameter is not a valid port.
MCAPI_EENDP_ISCREATED  The endpoint is already created.
MCAPI_ENODE_NOTINIT The node is not initialized.
MCAPI_EENDP_LIMIT Exceeded maximum number of endpoints allowed.
MCAPI_EEP_NOTALLOWED Endpoints cannot be created on this node.
MCAPI_EPARAM Incorrect mcapi_status parameter.
NOTE
The node number can only be set using the mcapi_intialize() function.
SEE ALSO
   mcapi_initialize() 
************************************************************************/
mcapi_endpoint_t mcapi_create_endpoint(
 	MCAPI_IN mcapi_port_t port_id, 
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{
  mcapi_endpoint_t e;
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (mcapi_trans_endpoint_exists (port_id)) {
      *mcapi_status = MCAPI_EENDP_ISCREATED;
    } else if (mcapi_trans_num_endpoints () == MAX_ENDPOINTS) {
      *mcapi_status = MCAPI_EENDP_LIMIT;
    } else if (!mcapi_trans_valid_port(port_id)) {
      *mcapi_status = MCAPI_EPORT_NOTVALID;  
    } else if (!mcapi_trans_create_endpoint(&e,port_id,MCAPI_FALSE))  {
      *mcapi_status = MCAPI_EEP_NOTALLOWED;
    }
  }
  return e;
}


/***********************************************************************
NAME
mcapi_get_endpoint_i - obtain the endpoint associated with a given tuple.
DESCRIPTION
mcapi_get_endpoint_i() allows other nodes ("third parties") to get the 
endpoint identifier for the endpoint associated with a global tuple name 
<node_id, port_id>.  This function is non-blocking and will return 
immediately.
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS. On error, 
*mcapi_status is set to the appropriate error defined below.
ERRORS
MCAPI_EPORT_NOTVALID The parameter is not a valid port.
MCAPI_ENODE_NOTVALID The parameter is not a valid node.
MCAPI_EPARAM Incorrect mcapi_status parameter.
NOTE
Use the mcapi_test(), mcapi_wait() and mcapi_wait_any() functions to 
query the status of and mcapi_cancel() function to cancel the operation.
SEE ALSO
  mcapi_get_node_id() 
************************************************************************/
void mcapi_get_endpoint_i(
                          MCAPI_IN mcapi_node_t node_id, 
                          MCAPI_IN mcapi_port_t port_id, 
                          MCAPI_OUT mcapi_endpoint_t* endpoint, 
                          MCAPI_OUT mcapi_request_t* request, 
                          MCAPI_OUT mcapi_status_t* mcapi_status)
{
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! mcapi_trans_valid_node (node_id)){
      *mcapi_status = MCAPI_ENODE_NOTVALID;
    } else if ( ! mcapi_trans_valid_port (port_id)) {
      *mcapi_status = MCAPI_EPORT_NOTVALID;
    } 
    mcapi_trans_get_endpoint_i (endpoint,node_id,port_id,request,mcapi_status); 
  }
}

/***********************************************************************
NAME
mcapi_get_endpoint - obtain the endpoint associated with a given tuple.
DESCRIPTION
mcapi_get_endpoint() allows other nodes ("third parties") to get the 
endpoint identifier for the endpoint associated with a global tuple name 
<node_id, port_id>.  This function will block until the specified remote 
endpoint has been created via the mcapi_create_endpoint() call. 
RETURN VALUE
On success, an endpoint is returned and *mcapi_status is set to 
MCAPI_SUCCESS. On error, MCAPI_NULL is returned and *mcapi_status is set 
to the appropriate error defined below.
ERRORS
MCAPI_EPORT_NOTVALID The parameter is not a valid port.
MCAPI_ENODE_NOTVALID The parameter is not a valid node.
MCAPI_EPARAM Incorrect mcapi_status parameter.
NOTE
SEE ALSO 
************************************************************************/
mcapi_endpoint_t mcapi_get_endpoint(
 	MCAPI_IN mcapi_node_t node_id, 
 	MCAPI_IN mcapi_port_t port_id, 
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{ 
  mcapi_endpoint_t e;

  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! mcapi_trans_valid_node (node_id)){
      *mcapi_status = MCAPI_ENODE_NOTVALID;
    } else if ( ! mcapi_trans_valid_port (port_id)) {
      *mcapi_status = MCAPI_EPORT_NOTVALID;
    } else {
      mcapi_trans_get_endpoint (&e,node_id,port_id);
    }
  }
  
  return e;
}

/***********************************************************************
NAME
mcapi_delete_endpoint - delete an endpoint.
DESCRIPTION
Deletes an MCAPI endpoint. Pending messages are discarded.  If an 
endpoint has been connected to a packet or scalar channel, the 
appropriate close method must be called before deleting the endpoint.  
Delete is a blocking  operation. Since the connection is closed before 
deleting the endpoint, the delete method does not require any 
cross-process synchronization and is guaranteed to return in a timely 
manner (operation will return without having to block on any IPC to any 
remote nodes). It is an error to attempt to delete an endpoint that has 
not been closed. Only the node that created an endpoint can delete it.
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS.  On error, 
*mcapi_status is set to the appropriate error defined below.
ERRORS
MCAPI_ENOT_ENDP Argument is not a valid endpoint descriptor.
MCAPI_ECHAN_OPEN A channel is open, deletion is not allowed.
MCAPI_ENOT_OWNER An endpoint can only be deleted by its creator.
MCAPI_EPARAM Incorrect mcapi_status parameter.
NOTE
SEE ALSO
mcapi_create_endpoint() 
************************************************************************/
void mcapi_delete_endpoint(
 	MCAPI_IN mcapi_endpoint_t endpoint, 
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if ( ! mcapi_trans_valid_endpoint(endpoint)) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    } else if (!mcapi_trans_endpoint_isowner (endpoint)) {
      *mcapi_status = MCAPI_ENOT_OWNER;
    } else if ( mcapi_trans_endpoint_channel_isopen (endpoint)) {
      *mcapi_status = MCAPI_ECHAN_OPEN;
    } else {
      /* delete the endpoint */
      mcapi_trans_delete_endpoint (endpoint);
    }
  }
}

/***********************************************************************
NAME
mcapi_msg_send_i - sends a (connectionless) message from a send endpoint 
to a receive endpoint.
DESCRIPTION
Sends a (connectionless) message from a send endpoint to a receive 
endpoint. It is a non-blocking function, and returns immediately. 
send_endpoint, is a local endpoint identifying the send endpoint, 
receive_endpoint identifies a receive endpoint. buffer is the application 
provided buffer, buffer_size is the buffer size in bytes, priority 
determines the message priority and request is the identifier used to 
determine if the send operation has completed on the sending endpoint 
and the buffer can be reused by the application. Furthermore, this 
method will abandon the send and return MCAPI_ENO_MEM if the system 
cannot allocate enough memory at the send endpoint to queue up the 
outgoing message.
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS. On error, 
*mcapi_status is set to the appropriate error defined below.
ERRORS
MCAPI_ENOT_ENDP Argument is not an endpoint descriptor.
MCAPI_EMESS_LIMIT The message size exceeds the maximum size allowed by the 
MCAPI implementation.
MCAPI_ENO_BUFFER No more message buffers available.
MCAPI_ENO_REQUEST No more request handles available.
MCAPI_ENO_MEM  No memory available.
MCAPI_EPRIO Incorrect priority level.
MCAPI_EPARAM Incorrect request or mcapi_status parameter.
NOTE
Use the mcapi_test(), mcapi_wait() and mcapi_wait_any() functions to 
query the status of and mcapi_cancel() function to cancel the operation.
SEE ALSO 
************************************************************************/
void mcapi_msg_send_i(
 	MCAPI_IN mcapi_endpoint_t send_endpoint, 
 	MCAPI_IN mcapi_endpoint_t receive_endpoint, 
 	MCAPI_IN void* buffer, 
 	MCAPI_IN size_t buffer_size, 
 	MCAPI_IN mcapi_priority_t priority, 
 	MCAPI_OUT mcapi_request_t* request, 
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* MCAPI_ENO_BUFFER, MCAPI_ENO_REQUEST, and MCAPI_ENO_MEM handled at the transport layer */
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! mcapi_trans_valid_priority (priority)){
      *mcapi_status = MCAPI_EPRIO;
    } else if (!mcapi_trans_valid_endpoints(send_endpoint,receive_endpoint)) {
      *mcapi_status = MCAPI_ENOT_ENDP; /* FIXME (errata A1) */
    } else if (buffer_size > MAX_MSG_SIZE) {
      *mcapi_status = MCAPI_EMESS_LIMIT;
    }
    mcapi_trans_msg_send_i (send_endpoint,receive_endpoint,buffer,buffer_size,request,mcapi_status);
  }
}


/***********************************************************************
NAME
mcapi_msg_send - sends a (connectionless) message from a send endpoint to 
a receive endpoint.
DESCRIPTION
Sends a (connectionless) message from a send endpoint to a receive endpoint. 
It is a blocking function, and returns once the buffer can be reused by the 
application. send_endpoint is a local endpoint identifying the send endpoint, 
receive_endpoint identifies a receive endpoint. buffer is the application 
provided buffer and buffer_size is the buffer size in bytes, and priority 
determines the message priority
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS. On error, *mcapi_status 
is set to the appropriate error defined below. Success means that the entire 
buffer has been sent. 
ERRORS
MCAPI_ENOT_ENDP Argument is not an endpoint descriptor.
MCAPI_EMESS_LIMIT The message size exceeds the maximum size allowed by the 
MCAPI implementation.
MCAPI_ENO_BUFFER No more message buffers available.
MCAPI_EPRIO Incorrect priority level.
MCAPI_EPARAM Incorrect mcapi_status parameter.
NOTE
SEE ALSO 
************************************************************************/
void mcapi_msg_send(
 	MCAPI_IN mcapi_endpoint_t  send_endpoint, 
 	MCAPI_IN mcapi_endpoint_t  receive_endpoint, 
 	MCAPI_IN void* buffer, 
 	MCAPI_IN size_t buffer_size, 
 	MCAPI_IN mcapi_priority_t priority, 
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{  

  /* FIXME: (errata B1) is it an error to send a message to a connected endpoint? */

  /* MCAPI_ENO_BUFFER handled at the transport layer */
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! mcapi_trans_valid_priority (priority)) {
      *mcapi_status = MCAPI_EPRIO;
    } else if (!mcapi_trans_valid_endpoints(send_endpoint,receive_endpoint)) {
      *mcapi_status = MCAPI_ENOT_ENDP; /* FIXME (errata A1) */
    } else if (buffer_size > MAX_MSG_SIZE) {
      *mcapi_status = MCAPI_EMESS_LIMIT;
    } else if ( !mcapi_trans_msg_send (send_endpoint,receive_endpoint,buffer,buffer_size)) {
      /* assume couldn't get a buffer */
      *mcapi_status = MCAPI_ENO_BUFFER;
    } 
  }
}


/***********************************************************************
NAME
mcapi_msg_recv_i - receives a (connectionless) message from a receive 
endpoint.
DESCRIPTION
Receives a (connectionless) message from a receive endpoint. It is a 
non-blocking function, and returns immediately. receive_endpoint is a 
local endpoint identifying the receive endpoint. buffer is the 
application provided buffer, and buffer_size is the buffer size in bytes. 
request is the identifier used to determine if the receive operation has 
completed (all the data is in the buffer). 
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS. On error, *mcapi_status 
is set to the appropriate error defined below.
ERRORS
MCAPI_ENOT_ENDP Argument is not a valid endpoint descriptor.
MCAPI_ETRUNCATED The message size exceeds the buffer_size.
MCAPI_ENO_REQUEST No more request handles available.
MCAPI_EPARAM Incorrect buffer, request and/or mcapi_status parameter.
NOTE
Use the mcapi_test() , mcapi_wait() and mcapi_wait_any() functions to 
query the status of and mcapi_cancel() function to cancel the operation.
SEE ALSO 
************************************************************************/
void mcapi_msg_recv_i(
 	MCAPI_IN mcapi_endpoint_t  receive_endpoint,  
 	MCAPI_OUT void* buffer, 
 	MCAPI_IN size_t buffer_size, 
 	MCAPI_OUT mcapi_request_t* request, 
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* MCAPI_ENO_REQUEST handled at the transport layer */
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! valid_buffer_param(buffer)) {
      *mcapi_status = MCAPI_EPARAM;
    } else   if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (!mcapi_trans_valid_endpoint(receive_endpoint)) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    }
    mcapi_trans_msg_recv_i(receive_endpoint,buffer,buffer_size,request,mcapi_status);
  }
}



/***********************************************************************
NAME
mcapi_msg_recv - receives a (connectionless) message from a receive endpoint.
DESCRIPTION
Receives a (connectionless) message from a receive endpoint. It is a 
blocking function, and returns once a message is available and the received 
data filled into the buffer. receive_endpoint is a local endpoint identifying 
the receive endpoint. buffer is the application provided buffer, and 
buffer_size is the buffer size in bytes.  The received_size parameter is 
filled with the actual size of the received message.
RETURN VALUE
On success, *mcapi_status is set to MCAPI_SUCCESS. On error, *mcapi_status 
is set to the appropriate error defined below.
ERRORS
MCAPI_ENOT_ENDP Argument is not a valid endpoint descriptor.
MCAPI_ETRUNCATED The message size exceeds the buffer_size.
MCAPI_EPARAM Incorrect buffer and/or mcapi_status parameter.
NOTE
SEE ALSO 
************************************************************************/
void mcapi_msg_recv(
 	MCAPI_IN mcapi_endpoint_t  receive_endpoint,  
 	MCAPI_OUT void* buffer, 
 	MCAPI_IN size_t buffer_size, 
 	MCAPI_OUT size_t* received_size, 
 	MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* FIXME: (errata B1) is it an error to try to receive a message on a connected endpoint?  */
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else  {
    *mcapi_status = MCAPI_SUCCESS;
    if (! valid_buffer_param(buffer)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (!mcapi_trans_valid_endpoint(receive_endpoint)) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    } else {
      mcapi_trans_msg_recv(receive_endpoint,buffer,buffer_size,received_size);
      if (*received_size > buffer_size) {
        *received_size = buffer_size;
        *mcapi_status = MCAPI_ETRUNCATED;
      }  
    }
  }
}

mcapi_uint_t mcapi_msg_available(
		MCAPI_IN mcapi_endpoint_t receive_endpoint, 
		MCAPI_OUT mcapi_status_t* mcapi_status)
{
	mcapi_uint_t rc = 0;
	if (! valid_status_param(mcapi_status)) {
		if (mcapi_status != NULL) {
			*mcapi_status = MCAPI_EPARAM;
		}
	} else {
		*mcapi_status = MCAPI_SUCCESS;
		if( !mcapi_trans_valid_endpoint(receive_endpoint)) {
			*mcapi_status = MCAPI_ENOT_ENDP;
		} else {
			rc = mcapi_trans_msg_available(receive_endpoint);
		}
	}
	return rc;
}

void mcapi_connect_pktchan_i(
        MCAPI_IN mcapi_endpoint_t  send_endpoint, 
        MCAPI_IN mcapi_endpoint_t  receive_endpoint, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* MCAPI_ENO_REQUEST handled at the transport layer */

  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if ( ! mcapi_trans_valid_endpoints(send_endpoint,receive_endpoint)) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    } else if (( mcapi_trans_channel_connected (send_endpoint)) ||  
               ( mcapi_trans_channel_connected (receive_endpoint))) {
      *mcapi_status = MCAPI_ECONNECTED;
    } else if (! mcapi_trans_compatible_endpoint_attributes (send_endpoint,receive_endpoint)) {
      *mcapi_status = MCAPI_EATTR_INCOMP;
    } 
    mcapi_trans_connect_pktchan_i (send_endpoint,receive_endpoint,request,mcapi_status);
  }
}

void mcapi_open_pktchan_recv_i(
        MCAPI_OUT mcapi_pktchan_recv_hndl_t* recv_handle, 
        MCAPI_IN mcapi_endpoint_t receive_endpoint, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status) 
{
  /* FIXME: (errata B2) shouldn't this function also check  MCAPI_ENO_REQUEST, there are several
     non-blocking functions that don't check for this - is that intentional or an
     oversight in the spec? */

  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;   
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_endpoint(receive_endpoint) ) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    } else if ( mcapi_trans_channel_type (receive_endpoint) == MCAPI_SCL_CHAN) {
      *mcapi_status = MCAPI_ECHAN_TYPE;
    } else if (! mcapi_trans_recv_endpoint (receive_endpoint)) {
      *mcapi_status = MCAPI_EDIR;
    } else if ( !mcapi_trans_connected (receive_endpoint)) {
      *mcapi_status = MCAPI_ENOT_CONNECTED;
    }
    mcapi_trans_open_pktchan_recv_i(recv_handle,receive_endpoint,request,mcapi_status);
  }
}

void mcapi_open_pktchan_send_i(
        MCAPI_OUT mcapi_pktchan_send_hndl_t* send_handle, 
        MCAPI_IN mcapi_endpoint_t  send_endpoint, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* FIXME: (errata B2) shouldn't this function also check  MCAPI_ENO_REQUEST? */
  /* FIXME: (errata B4) shouldn't this function also check MCAPI_ENOT_CONNECTED?  I do, but it's not in the spec */

  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_endpoint(send_endpoint) ) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    } else if ( mcapi_trans_channel_type (send_endpoint) == MCAPI_SCL_CHAN){
      *mcapi_status = MCAPI_ECHAN_TYPE;
    } else if (! mcapi_trans_send_endpoint (send_endpoint)) {
      *mcapi_status = MCAPI_EDIR;
    } else if ( !mcapi_trans_connected (send_endpoint)) {
      *mcapi_status = MCAPI_ENOT_CONNECTED;
    }
    mcapi_trans_open_pktchan_send_i(send_handle,send_endpoint,request,mcapi_status);
  }
}

void mcapi_pktchan_send_i(
        MCAPI_IN mcapi_pktchan_send_hndl_t send_handle, 
        MCAPI_IN void* buffer, 
        MCAPI_IN size_t size, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* MCAPI_ENO_BUFFER, MCAPI_ENO_REQUEST and MCAPI_ENO_MEM handled at the transport layer */
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_pktchan_send_handle(send_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else if ( size > MAX_PKT_SIZE) {
      *mcapi_status = MCAPI_EPACK_LIMIT; 
    }
    mcapi_trans_pktchan_send_i(send_handle,buffer,size,request,mcapi_status);
  }
}

void mcapi_pktchan_send(
        MCAPI_IN mcapi_pktchan_send_hndl_t send_handle, 
        MCAPI_IN void* buffer, 
        MCAPI_IN size_t size, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{

  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! mcapi_trans_valid_pktchan_send_handle(send_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else if ( size > MAX_PKT_SIZE) {
      *mcapi_status = MCAPI_EPACK_LIMIT; 
    } else  {
      if (!mcapi_trans_pktchan_send (send_handle,buffer,size)) {
        *mcapi_status = MCAPI_ENO_BUFFER;
      }
    }
  }
}

void mcapi_pktchan_recv_i(
        MCAPI_IN mcapi_pktchan_recv_hndl_t receive_handle,  
        MCAPI_OUT void** buffer, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{ 
  /* MCAPI_EPACKLIMIT, MCAPI_ENO_BUFFER, and MCAPI_ENO_REQUEST are handled at the transport layer */

  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else   if (! valid_buffer_param(buffer)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_pktchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    }
    mcapi_trans_pktchan_recv_i (receive_handle,buffer,request,mcapi_status);
  }
}

void mcapi_pktchan_recv(
        MCAPI_IN mcapi_pktchan_recv_hndl_t receive_handle, 
        MCAPI_OUT void** buffer, 
        MCAPI_OUT size_t* received_size, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;   
    if (! valid_buffer_param(buffer)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_pktchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else  {
      if (mcapi_trans_pktchan_recv (receive_handle,buffer,received_size)) {
        if ( *received_size > MAX_PKT_SIZE) {
          *mcapi_status = MCAPI_EPACK_LIMIT;
        } 
      } else {
        *mcapi_status = MCAPI_ENO_BUFFER;
      }
    }
  }
}

mcapi_uint_t mcapi_pktchan_available(
        MCAPI_IN mcapi_pktchan_recv_hndl_t receive_handle, 
        MCAPI_OUT mcapi_status_t* mcapi_status) 
{
  int num = 0;

  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! mcapi_trans_valid_pktchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else {
      num = mcapi_trans_pktchan_available(receive_handle);
    }
  }
  return num;
}

void mcapi_pktchan_free(
        MCAPI_IN void* buffer, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{

  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (!mcapi_trans_pktchan_free (buffer)) {
      *mcapi_status = MCAPI_ENOT_VALID_BUF;
    }
  }
}

void mcapi_pktchan_recv_close_i(
		MCAPI_IN mcapi_pktchan_recv_hndl_t receive_handle, 
		MCAPI_OUT mcapi_request_t* request, 
		MCAPI_OUT mcapi_status_t* mcapi_status)
{

	if (! valid_status_param(mcapi_status)) {
		if (mcapi_status != NULL) {
			*mcapi_status = MCAPI_EPARAM;
		}
	} else {
		*mcapi_status = MCAPI_SUCCESS;  
		if (! valid_request_param(request)) {
			*mcapi_status = MCAPI_EPARAM;
		} else if (! mcapi_trans_valid_pktchan_recv_handle(receive_handle) ) {
			*mcapi_status = MCAPI_ENOT_HANDLE;
		} else if (! mcapi_trans_pktchan_recv_isopen (receive_handle)) {
			*mcapi_status = MCAPI_ENOT_OPEN;
		}
		mcapi_trans_pktchan_recv_close_i (receive_handle,request,mcapi_status);
	}
}

void mcapi_pktchan_send_close_i(
		MCAPI_IN mcapi_pktchan_send_hndl_t send_handle, 
		MCAPI_OUT mcapi_request_t* request, 
		MCAPI_OUT mcapi_status_t* mcapi_status)
{
	if (! valid_status_param(mcapi_status)) {
		if (mcapi_status != NULL) {
			*mcapi_status = MCAPI_EPARAM;
		}
	} else {
		*mcapi_status = MCAPI_SUCCESS;
		if (! mcapi_trans_valid_pktchan_recv_handle(send_handle) ) {
			*mcapi_status = MCAPI_ENOT_HANDLE;
		} else if (! mcapi_trans_pktchan_send_isopen (send_handle)) {
			*mcapi_status = MCAPI_ENOT_OPEN;
		} if (! valid_request_param(request)) {
			*mcapi_status = MCAPI_EPARAM;
		}  
		mcapi_trans_pktchan_send_close_i (send_handle,request,mcapi_status);
	}
}



void  mcapi_connect_sclchan_i(
        MCAPI_IN mcapi_endpoint_t send_endpoint, 
        MCAPI_IN mcapi_endpoint_t receive_endpoint, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if ( ! mcapi_trans_valid_endpoints(send_endpoint,receive_endpoint)) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    } else if (( mcapi_trans_channel_connected (send_endpoint)) ||  
               ( mcapi_trans_channel_connected (receive_endpoint))) {
      *mcapi_status = MCAPI_ECONNECTED;
    } else if (! mcapi_trans_compatible_endpoint_attributes (send_endpoint,receive_endpoint)) {
      *mcapi_status = MCAPI_EATTR_INCOMP;
    } 
    mcapi_trans_connect_sclchan_i (send_endpoint,receive_endpoint,request,mcapi_status);
  }
}

void mcapi_open_sclchan_recv_i(
        MCAPI_OUT mcapi_sclchan_recv_hndl_t* receive_handle, 
        MCAPI_IN mcapi_endpoint_t receive_endpoint, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status) 
{
  /* FIXME: (errata B2) shouldn't this function also check  MCAPI_ENO_REQUEST? */
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;  
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_endpoint(receive_endpoint) ) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    } else if ( mcapi_trans_channel_type (receive_endpoint) == MCAPI_PKT_CHAN) {
      *mcapi_status = MCAPI_ECHAN_TYPE;
    } else if (! mcapi_trans_recv_endpoint (receive_endpoint)) {
      *mcapi_status = MCAPI_EDIR;
    } else if ( !mcapi_trans_connected (receive_endpoint)) {
      *mcapi_status = MCAPI_ENOT_CONNECTED;
    }
    mcapi_trans_open_sclchan_recv_i(receive_handle,receive_endpoint,request,mcapi_status);
  }
}

void mcapi_open_sclchan_send_i(
        MCAPI_OUT mcapi_sclchan_send_hndl_t* send_handle, 
        MCAPI_IN mcapi_endpoint_t send_endpoint, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;  
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_endpoint(send_endpoint) ) {
      *mcapi_status = MCAPI_ENOT_ENDP;
    } else if  (mcapi_trans_channel_type (send_endpoint) == MCAPI_PKT_CHAN){
      *mcapi_status = MCAPI_ECHAN_TYPE;
    } else if (! mcapi_trans_send_endpoint (send_endpoint)) {
      *mcapi_status = MCAPI_EDIR;
    } else if ( !mcapi_trans_connected (send_endpoint)) {
      *mcapi_status = MCAPI_ENOT_CONNECTED;
    }
    /* FIXME:(errata B2) shouldn't this function also check  MCAPI_ENO_REQUEST  */
    /* FIXME: (errata B4) shouldn't this function also check  MCAPI_ENOT_CONNECTED.  
       I do, but it's not in the spec */
    mcapi_trans_open_sclchan_send_i(send_handle,send_endpoint,request,mcapi_status);
  } 
}

void mcapi_sclchan_send_uint64(
        MCAPI_IN mcapi_sclchan_send_hndl_t send_handle,  
        MCAPI_IN mcapi_uint64_t dataword, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* FIXME: (errata B3) this function needs to check MCAPI_ENO_BUFFER */
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! mcapi_trans_valid_sclchan_send_handle(send_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    }  else if (!mcapi_trans_sclchan_send (send_handle,dataword,8)) {
      *mcapi_status = MCAPI_ENO_BUFFER;  /* MR: added this  */
    } 
  }
}
void mcapi_sclchan_send_uint32(
        MCAPI_IN mcapi_sclchan_send_hndl_t send_handle,  
        MCAPI_IN mcapi_uint32_t dataword, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* FIXME: (errata B3) this function needs to check MCAPI_ENO_BUFFER */
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! mcapi_trans_valid_sclchan_send_handle(send_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    }  else if (!mcapi_trans_sclchan_send (send_handle,dataword,4)) {
      *mcapi_status = MCAPI_ENO_BUFFER;
    } 
  }
}

void mcapi_sclchan_send_uint16(
        MCAPI_IN mcapi_sclchan_send_hndl_t send_handle,  
        MCAPI_IN mcapi_uint16_t dataword, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{ 
  /* FIXME: (errata B3) this function needs to check MCAPI_ENO_BUFFER */
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! mcapi_trans_valid_sclchan_send_handle(send_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    }  else if (!mcapi_trans_sclchan_send (send_handle,dataword,2)) {
      *mcapi_status = MCAPI_ENO_BUFFER; 
    }
  }
}

void mcapi_sclchan_send_uint8(
        MCAPI_IN mcapi_sclchan_send_hndl_t send_handle,  
        MCAPI_IN mcapi_uint8_t dataword, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  /* FIXME: (errata B3) this function needs to check MCAPI_ENO_BUFFER */
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
 if (! mcapi_trans_valid_sclchan_send_handle(send_handle) ) {
    *mcapi_status = MCAPI_ENOT_HANDLE;
  }  else if (!mcapi_trans_sclchan_send (send_handle,dataword,1)) {
    *mcapi_status = MCAPI_ENO_BUFFER;    
  }
}
}

mcapi_uint64_t mcapi_sclchan_recv_uint64(
        MCAPI_IN mcapi_sclchan_recv_hndl_t receive_handle, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  uint64_t dataword = 0;
  uint32_t exp_size = 8; 
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! mcapi_trans_valid_sclchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    }else if (! mcapi_trans_sclchan_recv (receive_handle,&dataword,exp_size)) {
      *mcapi_status = MCAPI_ESCL_SIZE;
    }
  }
  return dataword;
}

mcapi_uint32_t mcapi_sclchan_recv_uint32(
        MCAPI_IN mcapi_sclchan_recv_hndl_t receive_handle, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  uint64_t dataword = 0;
  uint32_t exp_size = 4; 
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;
    if (! mcapi_trans_valid_sclchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else if (! mcapi_trans_sclchan_recv (receive_handle,&dataword,exp_size)) {
      *mcapi_status = MCAPI_ESCL_SIZE;
    } 
  }
  return dataword;
}

mcapi_uint16_t mcapi_sclchan_recv_uint16(
        MCAPI_IN mcapi_sclchan_recv_hndl_t receive_handle, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{

  uint64_t dataword = 0;
  uint32_t exp_size = 2; 
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! mcapi_trans_valid_sclchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else if (! mcapi_trans_sclchan_recv (receive_handle,&dataword,exp_size)) {  
      *mcapi_status = MCAPI_ESCL_SIZE;
    } 
  }
  return dataword;
}

mcapi_uint8_t mcapi_sclchan_recv_uint8(
        MCAPI_IN mcapi_sclchan_recv_hndl_t receive_handle, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  uint64_t dataword = 0;
  uint32_t exp_size = 1; 
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! mcapi_trans_valid_sclchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else if (! mcapi_trans_sclchan_recv (receive_handle,&dataword,exp_size)) {
      *mcapi_status = MCAPI_ESCL_SIZE;
    }
  }
  return dataword;
}

mcapi_uint_t mcapi_sclchan_available (
        MCAPI_IN mcapi_sclchan_recv_hndl_t receive_handle, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  int num = 0;
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! mcapi_trans_valid_sclchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else {
      num = mcapi_trans_sclchan_available_i(receive_handle);
    }
  }
  return num;
}

void mcapi_sclchan_recv_close_i(
        MCAPI_IN mcapi_sclchan_recv_hndl_t receive_handle, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_sclchan_recv_handle(receive_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else if (! mcapi_trans_sclchan_recv_isopen (receive_handle)) {
      *mcapi_status = MCAPI_ENOT_OPEN;
    } 
    mcapi_trans_sclchan_recv_close_i (receive_handle,request,mcapi_status);
  }
}

void mcapi_sclchan_send_close_i(
        MCAPI_IN mcapi_sclchan_send_hndl_t send_handle, 
        MCAPI_OUT mcapi_request_t* request, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS;   
    if (! valid_request_param(request)) {
      *mcapi_status = MCAPI_EPARAM;
    } else if (! mcapi_trans_valid_sclchan_recv_handle(send_handle) ) {
      *mcapi_status = MCAPI_ENOT_HANDLE;
    } else if (! mcapi_trans_sclchan_send_isopen (send_handle)) {
      *mcapi_status = MCAPI_ENOT_OPEN;
    } 
    mcapi_trans_sclchan_send_close_i (send_handle,request,mcapi_status);
  }
}


mcapi_boolean_t mcapi_test(
        MCAPI_IN mcapi_request_t* request, 
        MCAPI_OUT size_t* size, 
        MCAPI_OUT mcapi_status_t* mcapi_status)
{
  mcapi_boolean_t rc = MCAPI_FALSE;
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_INCOMPLETE;  
    if (! valid_size_param(size)) {
      *mcapi_status = MCAPI_EPARAM;
       rc = MCAPI_TRUE;
    } else {
      rc = mcapi_trans_test_i(request,size,mcapi_status);
    }
  }
  return rc;
}

mcapi_boolean_t mcapi_wait(
        MCAPI_IN mcapi_request_t* request, 
        MCAPI_OUT size_t* size, 
        MCAPI_OUT mcapi_status_t* mcapi_status, 
        MCAPI_IN mcapi_timeout_t timeout)
{
  mcapi_boolean_t rc = MCAPI_FALSE;
 
   if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! valid_size_param(size)) {
      *mcapi_status = MCAPI_EPARAM;
      rc = MCAPI_TRUE;
    } else {
      rc = mcapi_trans_wait(request,size,mcapi_status,timeout);
    }
  }
  return rc;
}

mcapi_int_t mcapi_wait_any(
        MCAPI_IN size_t number, 
        MCAPI_IN mcapi_request_t** requests, 
        MCAPI_OUT size_t* size, 
        MCAPI_OUT mcapi_status_t* mcapi_status, 
        MCAPI_IN mcapi_timeout_t timeout)
{
  mcapi_boolean_t rc = MCAPI_FALSE;
  
  if (! valid_status_param(mcapi_status)) {
    if (mcapi_status != NULL) {
      *mcapi_status = MCAPI_EPARAM;
    }
  } else {
    *mcapi_status = MCAPI_SUCCESS; 
    if (! valid_size_param(size)) {
      *mcapi_status = MCAPI_EPARAM;
      rc = MCAPI_TRUE;
    } else {
      rc = mcapi_trans_wait_any(number,requests,size,mcapi_status,timeout);
    }
  }
  return rc;
}

