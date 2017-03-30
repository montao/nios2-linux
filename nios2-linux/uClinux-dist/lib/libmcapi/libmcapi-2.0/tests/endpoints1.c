/* Test: endpoints1
   Description: Tests creating and getting endpoints on a single node.  Tests all 
   error conditions.  For invalid endpoints, it has to use the non-blocking get_endpoint_i,
   otherwise we will deadlock waiting for the endpoint to be created.
*/

#include <mca.h>
#include <mcapi_test.h>
#include <mcapi.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <mcapi_impl_spec.h>

#define NODE_NUM 3
#define PORT_NUM 115

#define DOMAIN 0
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
  mcapi_info_t version;
  mcapi_request_t request;
  mcapi_param_t parms;

  size_t size;
  int node_num,i;
  mcapi_endpoint_t ep[MCAPI_MAX_ENDPOINTS];
  mcapi_endpoint_t ep1,ep2,ep3;

  /* create a node */
  mcapi_initialize(DOMAIN,NODE_NUM,NULL,&parms,&version,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  
  /* get my node_num */
  node_num = mcapi_node_id_get (&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  if (node_num != NODE_NUM) { WRONG }
  
  /* create an endpoint */
  ep1 = mcapi_endpoint_create (PORT_NUM,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  
  /* create endpoint */
  ep2 =mcapi_endpoint_create (MCAPI_PORT_ANY,&status);
  if (status != MCAPI_SUCCESS) { WRONG}
  
  /* lookup a valid endpoint */
  ep3 = mcapi_endpoint_get (DOMAIN, NODE_NUM, PORT_NUM,MCA_INFINITE,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  if (ep3 != ep1) { WRONG}

  /* delete the valid endpoints we've created so far */
  mcapi_endpoint_delete(ep1,&status);
  if (status != MCAPI_SUCCESS) { WRONG }
  mcapi_endpoint_delete(ep2,&status);
  if (status != MCAPI_SUCCESS) { WRONG }

  /* delete an invalid endpoint */
  /* ep3 == ep1 and we already deleted ep1, so expect an error */
  mcapi_endpoint_delete(0xdeadbeef,&status);
  if (status != MCAPI_ERR_ENDP_INVALID) { WRONG }


  /* try to get an endpoint that never existed (and no endpoints currently exist) */
  mcapi_endpoint_get_i (DOMAIN,NODE_NUM,10,&ep1,&request,&status);
  
   mcapi_finalize(&status);
  printf("   Test PASSED\n");
  return SUCCESS;
}
