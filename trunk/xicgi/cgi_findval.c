/*
 * Return value of a named CGI variable
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: cgi_findval.c,v 1.2 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: cgi_findval.c,v $
 * Revision 1.2  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.1  2001/11/15 16:18:39  nny
 * cgi_findval.c added; return the data a named cgivar points to.
 *
 */

#include "string.h"
#include "lists.h"
#include "xicgi.h"

char *cgi_findval(List *l, char *string)
{
	cgivar *v;

	v = (cgivar *)l->head;
	while(v) {
		if(strcmp(v->name, string)==0)
			return v->value;
		v = v->next;
	}
	return 0;
}
