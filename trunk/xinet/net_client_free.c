#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"

/**
 Close and free a client structure.
@doc
 This is merely a convenience call. All it does is to close
 the socket filedescriptor and free the structure.
@param client The NetClient structure to be freed.
@author Ronny Bangsund
@see net_client()
*/
void net_client_free(struct NetClient *client)
{
	if(client->sock) close(client->sock);
	free(client);
}
