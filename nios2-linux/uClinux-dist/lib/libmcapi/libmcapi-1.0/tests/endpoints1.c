/* Test: endpoints1
   Description: Tests creating and getting endpoints on a single node.  Tests all 
   error conditions.  For invalid endpoints, it has to use the non-blocking get_endpoint_i,
   otherwise we will deadlock waiting for the endpoint to be created.
*/

#include <mcapi.h>
#include <mcapi_datatypes.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */

#define NODE_NUM 3
#define PORT_NUM 115


#define FAIL 1
#define SUCCESS 0

#define WRONG wrong(__LINE__);
void wrong(unsigned line)
{
  fprintf(stderr,"WRONG: line=%u\n", line);
  fflush(stdout);
  exit(1);
}

int main () {
  mcapi_status_t status;
  mcapi_version_t version;
  mcapi_request_t request;
  size_t size;
  int node_num,i;
  mcapi_endpoint_t ep[MAX_ENDPOINTS];
  mcapi_endpoint_t ep1,ep2,ep3;

  /* create a node */
  mcapi_initialize(NODE_NUM,&version,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  
  /* get my node_num */
  node_num = mcapi_get_node_id (&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  if (node_num != NODE_NUM) { WRONG }
  
  /* create an endpoint */
  ep1 = mcapi_create_endpoint (PORT_NUM,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  
  /* create endpoint */
  ep2 =mcapi_create_endpoint (MCAPI_PORT_ANY,&status);
  if (status != MCAPI_SUCCESS) { WRONG}
  
  /* lookup a valid endpoint */
  ep3 = mcapi_get_endpoint (NODE_NUM, PORT_NUM,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  if (ep3 != ep1) { WRONG}

  /* delete the valid endpoints we've created so far */
  mcapi_delete_endpoint(ep1,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  mcapi_delete_endpoint(ep2,&status);
  if (status != MCAPI_SUCCESS) { WRONG }

  /* delete an invalid endpoint */
  /* ep3 == ep1 and we already deleted ep1, so expect an error */
  mcapi_delete_endpoint(0xdeadbeef,&status);
  if (status != MCAPI_ENOT_ENDP) { WRONG }


  /* try to get an endpoint that never existed (and no endpoints currently exist) */
  mcapi_get_endpoint_i (NODE_NUM,10,&ep1,&request,&status);
  
   mcapi_finalize(&status);
  printf("   Test PASSED\n");
  return SUCCESS;
}
