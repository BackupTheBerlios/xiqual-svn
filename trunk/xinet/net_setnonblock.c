#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"


/* This may not be 100% correct,
 * but should work for most systems.
 */
void net_setnonblock(int sock)
{
	int	arg = 1;
#if defined(WIN32)
	/* passing a non-zero value, socket mode set non-blocking */
	unsigned long true = 1;

	ioctlsocket(sock, FIONBIO, &arg);
#else
	int	opts;

	opts = fcntl(sock, F_GETFL);
	if(opts < 0) {
		perror("fcntl(F_GETFL)");
		return;
	}

	opts = (opts | O_NONBLOCK);
	if(fcntl(sock, F_SETFL, opts) < 0) {
		perror("fcntl(F_SETFL)");
	}
#endif
/* There might be more cases, but I currently won't focus on
 * other ports than UNIX-like and Win32. OpenTransport is icky.
 * No Mac port of this networking code planned yet :/
 */

	/* Supposedly useful for realtime games ;) */
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&arg, sizeof(arg));
}
