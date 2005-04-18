/*
 * Xiqual networking.
 * Note: This code hasn't been tested on anything but
 * Linux (Debian Unstable) yet. It should work on a few
 * close ones, but I know it might not compile properly
 * on at least SCO operating systems and FreeBSD/OpenBSD.
 * If anyone uses this on those platforms, let me know.
 * The issue at least with SCO systems is libx (linker
 * argument -lx), but headers are also slightly different
 * (headers were just one issue that prompted xiendian.h)
 *
 */

#if !defined(_XIQUAL_NET_H)
#define _XIQUAL_NET_H

/* "Borrowed" defs from SDL_net... 
 * These might help a bit towards porting, though.
 */
#if defined(__WIN32__) || defined(WIN32)
#	define Win32_Winsock
#	include <windows.h>
#else /* UNIX */
#	include <netinet/in.h>
#	include <sys/socket.h>
#	include <sys/time.h>
#	include <fcntl.h>
#	include <netdb.h>
#	include <unistd.h>
#	if !defined(__BEOS__)
#		include <arpa/inet.h>
#	endif
#	if defined(linux)
#		include <netinet/tcp.h>
#	endif
#endif /* WIN32 */

#if !defined(Win32_Winsock)
#	define closesocket	close
#	define SOCKET	int
#	define INVALID_SOCKET	-1
#endif /* Win32_Winsock */

#include "lists.h"
#include "xitags.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@name Networking structures
 Stuff goes here.
@doc
 More stuff.
*/

/*@{*/
/**
 Server structure returned by net_create_server().
@doc
 If you get this structure returned, then it is ready to receive
 incoming connections.
@author Ronny Bangsund
@see xipacket, net_create_server(), bind(), listen(), accept(), List
*/
typedef struct SelectServer {
/**
 File descriptor for the server's listening socket.
*/
	int	sock;
/**
	Socket set.
*/
	fd_set	set;
/**
 Maximum waiting connections for listen().
*/
	ulong	queue;
/**
 Timeout in seconds, if you prefer long waits.
*/
	ulong	timeout_s;
/**
 Timeout in milliseconds, if you also need to handle other processing.
*/
	ulong	timeout_ms;
/**
 Address structure for bind().
@doc
 It will be filled in with INADDR_ANY by net_create_server().
*/
	struct sockaddr_in	address;
/**
 Command mode.
@doc
 NET_MODE_LONG or NET_MODE_TEXT. The former implies strictly aligned
 blocks of header+data, the latter means the commands from the clients
 are in plain old text.
*/
	int	mode;
/**
 A list of accept()'d, active sockets.
@doc
 In other words, our active clients.
*/
	List	connectlist;
/**
 Unused connections.
@doc
 As long as there are items in this List, we're able to accept().
*/
	List	available;
/**
 Command list.
@doc
 This is a list of text commands plus functions to handle them.
*/
	List	commands;
/**
 Called when a new connection is made.
@doc
 The socket has already been accepted with accept().
@param socket Socket where client is connected.
*/
	void	(*onconnect)(int socket);
/**
 Called right before closing down a socket connection.
@param socket Socket where client is connected.
*/
	void	(*onclose)(int socket);
/**
 Required hook to do the actual processing when clients send data.
@doc
 The node parameter is there to allow removal of a client from the lists.

 NOTE: This function isn't 100% decided upon yet. Parameters may change.
@param server Server created by net_create_server().
@param node Pointer to this socket's Node structure.
*/
	void	(*onread)(struct SelectServer *server, Node *node);
/**
 Hook for use when xinet detects the server is full.
@doc
 When there are no more nodes in the available list in the SelectServer
 structure, xinet will respond with an XIPKT_FULL message to any new
 clients attempting to connect. If you install a hook here, you may
 for instance allocate more nodes, or respond more fully with a packet
 containing a reference to some other server.
@param server Server created by net_create_server().
@param node Pointer to this socket's Node structure.
*/
	void	(*onserverfull)(struct SelectServer *server, Node *node);
/**
 Disconnection hook.
@doc
 When a client wants to disconnect/has disconnected, this hook
 is called if it exists. Since the client may already be gone,
 it's necessary to be flexible.
*/
	void	(*ondisconnect)(struct SelectServer *server, Node *node);
} SelectServer;


typedef struct CmdNode {
	struct CmdNode	*next;
	struct CmdNode	*prev;
	char	*name;
	void	*func;
} CmdNode;


/**
 Simple network packet header.
@doc
 All data is in network order. Any amount of data may follow
 directly after this structure.
@see SelectServer
*/
typedef struct xipacket {
/**
 Either one of the few Xiqual-defined types, or program-specific.

 Xiqual-defined types are the following:

 <b>XIPKT_HELLO</b>: When sent from the server, a request for info.
 The packet should contain protocol-specific data to tell the client
 what specific info it wants.

 <b>XIPKT_BYE</b>: Coming from a server, it means you already have
 been disconnected. From a client, it's a note that it's leaving.
 Might not be required in all cases; some programs might require
 a special shutdown procedure to allow saving of some data, but it
 should be rare.

 <b>XIPKT_FULL</b>: This server is full. Data might contain a retry
 timeout, or maybe a reference to another server to try. It's up to
 you to think of something clever here.

 <b>XIPKT_WELCOME</b>: Normal welcome. Your hello has been acknowledged,
 and you may start normal communication.

 <b>XIPKT_CERT</b>: This is the welcome message given in cases where
 encryption is required. A certificate or key is included in this
 package. This system is still in the design phase..

 <b>XIPKT_TEXT</b>: Tells the other side that all communication should
 be in the form of plaintext commands instead of 4-byte aligned codes.
*/
	ulong	type;
/**
 Size of the following chunk of data.
@doc
 xinet will basically ignore any network communication less than eight
 bytes. Size should always be minimum that to tell the core system that
 it's a well-formed package. Or so we may hope.
*/
	ulong	size;
} xipacket;


/**
 Network client convenience structure.
@doc
 A handy struct for use with the equally handy convenience functions
 xinet has to offer for TCP connections. Mostly useful with the
 SelectServer system.
@see SelectServer
*/
typedef struct NetClient {
/**
 Socket filedescriptor.
*/
	int	sock;
/**
 Address and port of server to connect to.
*/
	struct sockaddr_in address;
} NetClient;


typedef struct cmdarray {
	char	*text;
	void	*command;
} cmdarray;


/* Packet types - contents may vary between programs.
 * You may choose to pass login names etc. in a hello,
 * for example.
 * These are merely suggestions, but a more automated system is
 * planned for xinet. A default package will be used to initiate
 * a connection and pass keys/certificates.
 */
#define XIPKT_NULL	0	/* You're not supposed to have a NULL packet */
#define XIPKT_HELLO	1	/* I present to you some of my details.. (name string etc.) */
#define XIPKT_BYE	2	/* I'm disconnecting/you are not allowed to play anymore (reason follows) */
#define XIPKT_FULL	3	/* Server full..text with more info follows ("Max n players allowed") */
#define XIPKT_WELCOME	4	/* Server has room for you - token follows */
#define XIPKT_CERT	5	/* Crypto-certificate follows - use whenever
							you're sending commands hereafter */
#define	XIPKT_TEXT	6	/* Packet has command in plaintext */

extern void destroy_socket(Node *socket);
extern SelectServer *net_serverA(tagitem *tags);
extern SelectServer *net_server(ulong tag1, ... );
extern void net_server_free(SelectServer *server);
extern void net_tag_serverA(SelectServer *server, tagitem *tags);
extern void net_tag_server(SelectServer *server, ulong tag1, ... );
extern void net_addnodes(SelectServer *server, ulong num);
extern void net_setnonblock(int sock);
extern void net_accept(SelectServer *server);
extern void net_remove_client(SelectServer *server, Node *n);
extern void net_handle(SelectServer *server, Node *node);
extern int net_process(SelectServer *server);
extern NetClient *net_clientA(tagitem *tags);
extern NetClient *net_client(ulong tag1, ... );
extern void net_client_free(struct NetClient *client);
extern void *net_read(int sock);
extern ssize_t net_writeh(int sock, ulong id);

enum {
	XINET_BASE = (('N'<<24) | ('E'<<16) | ('T'<<8) | (' ')),
	XN_NUMCONNECTIONS,	/* Maximum number of connection nodes available */
	XN_TIMEOUT_S,	/* Timeout in seconds before select() returns */
	XN_TIMEOUT_MS,	/* As above, but milliseconds - 10ms is about
					  the lowest you can hope for on today's systems */
	XN_PORT,	/* Port to wait for connections on */
	XN_LINGER,	/* Number of seconds for sockets to linger after closing */
	XN_CONNECT_FUNC,	/* Called right after connection */
	XN_CLOSE_FUNC,	/* Called just before shutting down a socket */
	XN_READ_FUNC,	/* Read network packets here */
	XN_SERVERFULL_FUNC,	/* Special action if no more clients are allowed */
	XN_DISCONNECT_FUNC,
	XN_QUEUE,
	XN_ADDRESS,		/* Address for client functions (dot-notation) */
	XN_HOSTNAME,	/* Alternative to XN_ADDRESS */
	XN_COMMANDS,	/* Array of textcommands + function pointers */
	XN_MODE			/* Mode of communication */
};


enum {
	NET_MODE_LONG=0,
	NET_MODE_TEXT
};


#ifdef __cplusplus
}
#endif

/*@}*/

#endif	/* EOF */
