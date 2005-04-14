#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"
#include "xios.h"

/**
 Read an incoming xinet package.
@doc
 A new buffer is allocated to hold an entire message
 conforming to the xipacket structure. On little-endian
 platforms, the header data is converted to local format.

 The buffer must be freed after use.
@param sock Socket filedescriptor to read from
@return A pointer to the buffer
@see xipacket, net_writeh(), write()
*/
void *net_read(int sock)
{
	char *buf=0;
	ssize_t len;
	struct xipacket pkt;

	len = read(sock, &pkt, sizeof(struct xipacket));
	if(len != sizeof(struct xipacket)) return NULL;

	len = _SwapBE32(pkt.size);
	buf = calloc(1, len);
	if(!buf) return NULL;

	buf[0] = _SwapBE32(pkt.type);
	buf[1] = len;
	len = read(sock, buf+sizeof(struct xipacket), len);
	return buf;
}
