/*
 * Tokeniser. A replacement for the unsafe
 * strtok() functions in GNU libc
 * (latest broken version).
 *
 */

#include "xistr.h"


/**@name String tokeniser
 The GNU libc functions strtok() and strok_r() seem to be
 rather unsafe to use, so I wrote this replacement. It is
 not superfast (memory allocations to hold tokens), but
 will not mess up a threaded application, give you a whole
 bunch of valid tokens immediately, and is portable.

 MingW32 seems to be missing strtok_r(), among other things.
 And the big fat warnings put me off using the strtok-family
 too much, anyway. I quote the BUGS section: "Never use these
 functions".
*/

/*@{*/
/**
 Create a tokeniser object from a string.
@doc
 The supplied string is copied, and this copy is
 modified with NULL bytes in place of its delimiters.
 This function does a better job than strtok() and
 strtok_r(). This function is thread-safe, hopefully.
@author Ronny Bangsund
@see str_freetoken(), strtoken, strtok(), strtok_r()
*/
strtoken *str_tokenise(char *s, char *delim)
{
	strtoken	*t;
	size_t	sptr=0, dptr, prev=0;

	if((!s) || (!delim)) return NULL;

	t = calloc(1, sizeof(strtoken));
	if(!t) return NULL;

	t->s = strdup(s);
	t->d = strdup(delim);
	if((!t->d) || (!t->s)) {
		str_freetoken(t);
		return NULL;
	}
	t->len = strlen(t->s);
	t->dlen = strlen(t->d);

	/* All memory allocations succeeded!
	 * The real tokenisation can now begin.
	 */
	while(sptr <= t->len) {	/* Also look at the NULL terminator! */
		dptr = 0;
		while(dptr <= t->dlen) {
			/* Speedup-trick, I hope..last iteration compares
			 * against the NULL byte, instead of doing two
			 * compares per iteration.
			 */
			if(t->s[sptr] == t->d[dptr]) {
				t->s[sptr] = 0;
				if(t->s[prev] && isprint(t->s[prev])) list_newitem(&t->tokens, t->s+prev);
				dptr = t->dlen;
				prev = 0;
				if(sptr) prev = sptr+1;
			}
			dptr++;
		}
		sptr++;
	}
	return t;
}


/**
 Free a tokeniser object.
@param strtoken A strtoken structure created by str_tokenise().
@author Ronny Bangsund
@see str_tokenise(), strtoken
*/
void str_freetoken(strtoken *t)
{
	Node	*n;

	if(!t) return;

	while(t->tokens.head) {
		n = t->tokens.head;
		list_remove(&t->tokens, n);
		free(n);
	}
	if(t->s) free(t->s);
	if(t->d) free(t->d);
	free(t);
}

/*@}*/
