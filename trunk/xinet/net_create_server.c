/*
 * A simple system to create select()-based servers.
 * Future additions might include a default onread() function
 * that will call different hooks for special packet IDs.
 *
 */

#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"


SelectServer *net_create_serverA(tagitem *tags)
{
	SelectServer	*server;
	int	true = 1;

/* Win32 wants to be weird */
#ifdef Win32_Winsock
	WORD version_wanted = MAKEWORD(1,1);
	WSADATA wsaData;

	if(WSAStartup(version_wanted, &wsaData)) {
		fprintf(stderr, "Couldn't initialize Winsock 1.1\n");
		return 0;
	}
#endif

	server = malloc(sizeof(SelectServer));
	if(!server)
		return NULL;

	memset(server, 0, sizeof(SelectServer));
	server->queue = 5;

	/* Get descriptor for listening socket */
	server->sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server->sock < 0) {
		perror("socket");
		free(server);
		return NULL;
	}

	/* We want to bind without TIME_WAIT problems */
	setsockopt(server->sock, SOL_SOCKET, SO_REUSEADDR,
		(char *)&true, sizeof(true));
	net_setnonblock(server->sock);

	server->connectlist.destructor = (void *)&destroy_socket;
	server->available.destructor = (void *)&destroy_socket;
	server->address.sin_family = AF_INET;
	server->address.sin_addr.s_addr = _SwapBE32(INADDR_ANY);
	net_tag_serverA(server, tags);

	if (bind(server->sock,
		(struct sockaddr *) &server->address,
		sizeof(server->address)) < 0 ) {
		perror("bind");
		close(server->sock);
		net_free_server(server);
		return NULL;
	}
	listen(server->sock, server->queue);
	return server;
}


/**
 Create a new select() server.
@doc
 All parameters are passed in as a taglist. To create a simple
 server for no more than 8 connections, at port 1242, calling
 client_read() to process each incoming message:

 SelectServer	*server;

 server = net_create_server(XN_NUMCONNECTIONS, 16,
 XN_TIMEOUT_S, 1, XN_PORT, 1242, XN_ONREAD_FUNC, client_read,
 TAG_END);

 The accepted tags for this function are as follows:

 <B>XN_NUMCONNECTIONS</B>: Maximum number of connection nodes to make available.

 <B>XN_TIMEOUT_S</B>: Timeout in seconds before select() returns.

 <B>XN_TIMEOUT_MS</B>: As above, but milliseconds - 10ms being the lowest you can
 safely expect to work.

 <B>XN_PORT</B>: Port to wait for connections on.

 <B>XN_LINGER</B>: Number of seconds for sockets to linger after closing. If a socket
 doesn't have the linger setting activated, it may take up to 5 minutes before
 the operating system releases all resources properly.

 <B>XN_CONNECT_FUNC</B>: Hook called right after connection.

 <B>XN_CLOSE_FUNC</B>: Hook called just before shutting down a socket.

 <B>XN_ONREAD_FUNC</B>: Hook called to read incoming messages. There is no
 default reader in libxinet yet.

 <B>XN_QUEUE</B>: Maximum listen() queue. Defaults to 5.

@param tags Tags defining all server settings.
@return A pointer to a valid SelectServer structure, with sockets able to listen().
@author Ronny Bangsund
@see SelectServer, listen(), net_process(), xipacket, net_free_server()
*/
SelectServer *net_create_server(ulong tag1, ... )
{
	XI_VARARG(net_create_serverA((tagitem *)&tag1), SelectServer)
}


void destroy_socket(Node *socket)
{
	free(socket);
}
