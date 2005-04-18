#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"


/* If there is a slot available for a new connection,
 * we let it connect now. TODO: Hooks for server full
 * and other conditions.
 */
void net_accept(SelectServer *server)
{
	int	client;
	Node	*node;
	struct sockaddr	addr;
	socklen_t	len;

	/* We want to briefly connect, whether we have space
	 * or not. Politely turn any clients away if we're full.
	 */
	len = sizeof(struct sockaddr);
	client = accept(server->sock, &addr, &len);
	if(client < 0) {
		perror("accept");
		return;
	}
	net_setnonblock(client);

	/* No room? Turn down client */
	if(!server->available.head) {
		printf("Client turned down: %ld clients already connected\n", server->connectlist.size);
		close(client);
		return;
	}

	/* Move an available node over to connected list */
	node = server->available.head;
	(int)node->data = client;
	list_remove(&server->available, node);
	list_add(&server->connectlist, node);
	if(server->onconnect) {
		server->onconnect(client);
	}
	FD_SET(client, &server->set);
}
