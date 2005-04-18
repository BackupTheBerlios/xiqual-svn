#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"

/**
 Write an xinet packet header.
@doc
@param sock Socket filedescriptor
@param id Packet header identification
@return Any value other than sizeof(xipacket) indicates an error.
@author Ronny Bangsund
*/
ssize_t net_writeh(int sock, ulong id)
{
	struct xipacket pkt;

	pkt.type = _SwapBE32(id);
	pkt.size = _SwapBE32(sizeof(struct xipacket));
	return write(sock, &pkt, sizeof(struct xipacket));
}
