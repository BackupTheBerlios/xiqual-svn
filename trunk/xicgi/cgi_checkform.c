/*
 * Check if a form has any content.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: cgi_checkform.c,v 1.3 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: cgi_checkform.c,v $
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
 * Revision 1.4  2001/12/19 09:00:57  nny
 * Pruned logs.
 *
 *
 */

#include "stdlib.h"
#include "string.h"

#include "xicgi.h"

int cgi_checkform()
{
	char *tmp;

	cgi->method = getenv("REQUEST_METHOD");
	tmp = getenv("QUERY_STRING");
	if(tmp) {
		cgi->contentlength = strlen(tmp);
		cgi->query = calloc(1, cgi->contentlength+1);
		if(cgi->query)
			strcpy(cgi->query, tmp);
			cgi->query[strlen(tmp)] = 0;
	}
	cgi->pathinfo = getenv("PATH_INFO");
	cgi->pathtrans = getenv("PATH_TRANSLATED");

	tmp = getenv("CONTENT_LENGTH");
	if(tmp)
		cgi->contentlength = strtol(tmp, NULL, 10);

	/* If there is a GET query, the data is passed through an environment variable */
	if((cgi->query) && (strlen(cgi->query)) && (strcmp(cgi->method, "GET")==0)) {
		return 1;
	}
	/* With POST, the data is passed on the standard input */
	if((cgi->contentlength) && (strcmp(cgi->method, "POST")==0)) {
		return 1;
	}

	return 0;
}
