#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"


/**
 Release all resources related to a Xiqual select() server.
@doc
 This shuts down the sockets, calling each socket's onclose()
 hook if available, and cleaning up Win32 sockets if compiled
 for Win32.
@author Ronny Bangsund
@see SelectServer, net_create_server()
*/
void net_free_server(SelectServer *server)
{
	Node	*socket;
	int	s;

	if(!server)
		return;

	while((socket = server->connectlist.head)) {
		s = (int)socket->data;
		if(server->onclose)
			server->onclose(s);

		/* Merciless shutdown - no further reads */
		(void)close(s);
		list_delete(&server->connectlist, socket);
	}
	while((socket = server->available.head)) {
		list_delete(&server->available, socket);
	}

#ifdef Win32_Winsock
	if(WSACleanup() == -1) {
		if(WSAGetLastError() == WSAEINPROGRESS) {
			WSACancelBlockingCall();
			WSACleanup();
		}
	}
#endif
}
