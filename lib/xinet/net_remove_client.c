/*
 * Internal function - removes a client connection from a
 * server's list, then calls its ondisconnect() hook.
 */

#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"


void net_remove_client(SelectServer *server, Node *n)
{
	int	sock;

	if((!n) || (!server)) return;

	sock = (int)n->data;
	if(FD_ISSET(sock, &server->set)) {
		FD_CLR(sock, &server->set);
	}
	list_remove(&server->connectlist, n);
	list_add(&server->available, n);
	if(server->ondisconnect) server->ondisconnect(server, n);
}
