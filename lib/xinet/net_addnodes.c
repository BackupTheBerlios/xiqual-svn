#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"


/**
 Increase the maximum allowed number of clients in a SelectServer.
@param server Server struct to add slots to.
@param num Number of new nodes to allocate.
@see net_server(), SelectServer
*/
void net_addnodes(SelectServer *server, ulong num)
{
	ulong	count;
	Node	*node;

	count = num;
	while(count) {
		node = list_newitem(&server->available, NULL);
		if(!node) {
			return;
		}
		count--;
	}
}
