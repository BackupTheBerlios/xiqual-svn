/* Operating system-specific bits.
 * Not much here yet until I get help :/
 */

#if !defined(_XIQUAL_OS_H)
#define _XIQUAL_OS_H

/* Heh. Not much going on here yet. */
//#if defined(STDC_HEADERS)
//#else
//#endif

#include "ctype.h"
#include "errno.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#if !defined(WIN32)
#	include "unistd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
#	if !defined(ulong)
		typedef unsigned int ulong;
#	endif
#	if !defined(ushort)
		typedef unsigned short ushort;
#	endif
#	if !defined(socklen_t)
	typedef int socklen_t;
#	endif
#	if !defined(ssize_t)
	typedef size_t ssize_t;
#	endif
#	if !defined(in_addr_t)
	typedef ulong in_addr_t;
#	endif
#endif

#if !defined(snprintf)
/* Win32 oddness */
#	if defined(WIN32)
#		include "direct.h"
#		include "io.h"
#		define chdir _chdir
#		define close _close
#		define getcwd _getcwd
#		define snprintf _snprintf
#		define strcasecmp stricmp
#		define strncasecmp strnicmp
#	endif
#endif


/* Undefine the somewhat unsafe strndup() function.
 */
#undef strndup

/* Redefine it as a macro pointing to
 * a safer internal function.
 */
extern char *xi_strndup(const char *, size_t);
#define strndup xi_strndup


#ifdef __cplusplus
}
#endif

#endif /* EOF */
