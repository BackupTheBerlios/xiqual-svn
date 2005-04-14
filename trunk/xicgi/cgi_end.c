/*
 * Free resources and print end tags
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: cgi_end.c,v 1.2 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: cgi_end.c,v $
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
#include "stdlib.h"

#include "xicgi.h"

extern xicgi *cgi;

void cgi_end()
{
	if(!cgi)
		return;

	free(cgi->query);

	if(cgi->mime == MIME_HTML) {
		printf("</BODY></HTML>\n\n");
	}
}
