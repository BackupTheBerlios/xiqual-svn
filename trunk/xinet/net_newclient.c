#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"


/* If there is a slot available for a new connection,
 * we let it connect now. TODO: Hooks for server full
 * and other conditions.
 */
void net_newclient(SelectServer *server)
{
	int	client;
	Node	*node;

	/* We want to briefly connect, whether we have space
	 * or not. Politely turn any clients away if we're full.
	 */
	client = accept(server->sock, NULL, NULL);
	if(client < 0) {
		perror("accept");
		return;
	}
	net_setnonblock(client);

	/* No room? Turn down client */
	if(!server->available.head) {
		printf("Client turned down: %ld clients already connected\n", server->connectlist.size);
		close(client);
	}

	/* Move an available node over to connected list */
	node = server->available.head;
	(int)node->data = client;
	list_remove(&server->available, node);
	list_add(&server->connectlist, node);
	if(server->onconnect) {
		server->onconnect(client);
	}
}
