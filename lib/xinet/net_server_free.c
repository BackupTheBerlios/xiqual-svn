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
@see SelectServer, net_server()
*/
void net_server_free(SelectServer *server)
{
	Node	*n;
	CmdNode	*cmd;
	int	s;

	if(!server) return;

	while((n = server->connectlist.head)) {
		s = (int)n->data;
		if(server->onclose) server->onclose(s);
		/* Merciless shutdown - no further reads */
		(void)close(s);
		list_delete(&server->connectlist, n);
	}
	while((n = server->available.head)) {
		list_delete(&server->available, n);
	}
	while((cmd = (struct CmdNode *)server->commands.head)) {
		free(cmd->name);
		list_delete(&server->commands, (Node *)cmd);
	}
	close(server->sock);

#ifdef Win32_Winsock
	if(WSACleanup() == -1) {
		if(WSAGetLastError() == WSAEINPROGRESS) {
			WSACancelBlockingCall();
			WSACleanup();
		}
	}
#endif
}
