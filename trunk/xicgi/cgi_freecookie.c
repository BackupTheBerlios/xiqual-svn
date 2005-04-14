/*
 * Free memory of a cookie allocated by cgi_getcookie()
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: cgi_freecookie.c,v 1.2 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: cgi_freecookie.c,v $
 * Revision 1.2  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.5  2001/12/19 09:00:57  nny
 * Pruned logs.
 *
 *
 */

#include "stdlib.h"

#include "xicgi.h"

void cgi_freecookie(cookie *c)
{
	if(!c)
		return;

	free(c->value);
	free(c);
}
