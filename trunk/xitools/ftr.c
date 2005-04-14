/*
 * Fsck, that's retarded!
 *
 * Some functions that are so damn useful you'd want 'em everywhere
 * simply aren't in every OS. They might not be in the POSIX standards,
 * but I tend to rely on many of them.
 */

#include "xios.h"

/**
 Duplicate a string and return a safe buffer.
@doc
 The Linux strndup() function uses a memory allocation function
 that its own documentation warns against. Win32, and possibly
 other systems, don't have strndup() at all (and I'm not going
 to research the names on every platform). This function is a
 working, hopefully safe, implementation of it.

 The old strndup() is undefined by Xiqual, and by including
 xios.h, you'll get strndup defined as xi_strndup. This applies
 to all platforms. The function itself resides in the core
 xitools library.

 (Linux strndup() uses alloca(), which allocates memory in such
 a way that the memory is no longer safe to use once the calling
 function returns. This is clearly not good enough for a library
 where lots of nodes might be created and passed around like bongs
 at Woodstock.)
@param s The string to duplicate
@param n Number of characters to copy
@return A copy of the string with terminating NULL byte.
@author Ronny Bangsund
@see calloc()
*/
char *xi_strndup(const char *s, size_t n)
{
	char *newstr;

	newstr = calloc(1, n+1);
	if(!newstr) return NULL;

	strncpy(newstr, s, n);
	return newstr;
}
