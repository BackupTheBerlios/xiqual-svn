/*
 * Create a connection to a server and return a structure with data.
 */

#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"

NetClient *net_clientA(tagitem *tags)
{
	tagitem	*ti;
	struct hostent *host=0;
	NetClient *client;
	int result;

	client = calloc(1, sizeof(struct NetClient));
	if(!client) return NULL;

	client->sock = socket(AF_INET, SOCK_STREAM, 0);
	if(client->sock == -1) {
		free(client);
		return NULL;
	}

	ti = tag_next(&tags);
	while(ti) {
		switch(ti->tag) {
		case XN_PORT:
			client->address.sin_port = _SwapBE16(ti->data);
			break;
		case XN_ADDRESS:
			client->address.sin_addr.s_addr = inet_addr((char *)ti->data);
			break;
		case XN_HOSTNAME:
			/* Untested, but should work if the name is in DNS */
			if(host == gethostbyname((char *)ti->data)) {
				client->address.sin_addr.s_addr = (in_addr_t)host->h_addr_list[0];
			}
			break;
		}
		ti = tag_next(&tags);
	}
	client->address.sin_family = AF_INET;
	result = connect(client->sock, (struct sockaddr *)&client->address,
		sizeof(struct sockaddr_in));
	if(result == -1) {
		free(client);
		return NULL;
	}
	return client;
}

/**
 Allocate a NetClient structure and connect to a server.
@doc
 Creates a simple TCP connection to the server and port
 specified in its tags.

 Supported tags:

 <b>XN_PORT</b>: Port number in the range 1-65535.

 <b>XN_ADDRESS</b>: Address string in dot notation format,
 for example "10.0.0.1".

 <b>XN_HOSTNAME</b>: Server hostname to resolve and connect
 to, for example "example.net".

 A typical call would look like this:

 NetClient *client;
 client = net_client(XN_PORT, 1042, XN_ADDRESS, "10.0.0.1", 
TAG_END);
@author Ronny Bangsund
@see xipacket, net_client_free()
*/
NetClient *net_client(ulong tag1, ... )
{
	XI_VARARG(net_clientA((tagitem *)&tag1), NetClient)
}
