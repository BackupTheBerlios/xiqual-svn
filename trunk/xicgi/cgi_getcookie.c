/*
 * Request a specific cookie from the browser.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: cgi_getcookie.c,v 1.3 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: cgi_getcookie.c,v $
 * Revision 1.3  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.2  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.8  2001/12/19 09:00:57  nny
 * Pruned logs.
 *
 *
 */

#include "ctype.h"
#include "stdlib.h"
#include "string.h"

#include "xicgi.h"

cookie *cgi_getcookie(char *name)
{
	char *cookies, *val=0, *ptr=0;
	cookie *c=0;

	cookies = getenv("HTTP_COOKIE");
	if(!cookies)
		return 0;

	if((val = strstr(cookies, name))) {
		c = calloc(1, sizeof(cookie));
		if(!c) {
			return 0;
		}
		c->name = name;
		c->value = calloc(1, strlen(cookies));
		if(!c->value) {
			free(c);
			return 0;
		}
		memset(c->value, 0, strlen(cookies));
		while(val[0]) {
			switch(val[0]) {
			case '=':
				ptr = c->value;
				val++;
				while((val[0]!=';') && (val[0])) {
					ptr[0] = val[0];
					val++; ptr++;
				}
				return c;
			case ';':
			case 0:
				/* We'll free this cookie if there was no value */
				if(!ptr) {
					free(c->value);
					free(c);
					return 0;
				} else {
					return c;
				}
			}
			val++;
		}
	}
	return c;
}
