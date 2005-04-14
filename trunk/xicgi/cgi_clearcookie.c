/*
 * Remove a cookie from a client's browser.
 * Useful when you want to play nice.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: cgi_clearcookie.c,v 1.2 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: cgi_clearcookie.c,v $
 * Revision 1.2  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.6  2001/12/19 09:00:57  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"

#include "xicgi.h"

void cgi_clearcookie(char *name)
{
	if(name)
		printf("Set-Cookie: %s=; expires Thu, 01-Jan-1970 00:00:00 GMT\n", name);
}
