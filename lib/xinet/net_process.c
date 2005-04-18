#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"


/**
 Handle a select() server's incoming and outgoing data.
@doc
 All user-supplied hooks will be called as required.
 Sockets may be shut down on error.
@param server SelectServer structure created with net_create_server().
@return Returnvalue of the select() call.
@author Ronny Bangsund
@see SelectServer, net_server(), xipacket, net_server_free()
*/
int net_process(SelectServer *server)
{
	struct timeval	timeout;
	int	highest, read, socket;
	Node	*node;

	timeout.tv_sec = server->timeout_s;
	timeout.tv_usec = server->timeout_ms;

	socket = server->sock;
	highest = socket;
	node = server->connectlist.head;
	while(node) {
		if((int)node->data > highest) highest = (int)node->data;
		node = node->next;
	}

	read = select(highest+1, &server->set, NULL, NULL, &timeout);

	/* Ooh..something failed bad! */
	if(read == -1) {
		perror("select");
		return read;
	}
	/* read is now number of clients active */
	if(read > 0) {
		socket = server->sock;
		if(FD_ISSET(socket, &server->set)) {
			net_accept(server);
		}

		node = server->connectlist.head;
		while(node) {
			socket = (int)node->data;
			if(FD_ISSET(socket, &server->set)) {
				net_handle(server, node);
			}
			node = node->next;
		}
	}
	/* Zero means there's nothing to read */
	return read;
}
