/*
 * Initialise the CGI library
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: cgi_init.c,v 1.3 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: cgi_init.c,v $
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

#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"

#include "xicgi.h"
#include "xitags.h"

extern xicgi *cgi;

/*
 * Print the content header and get environment variables
 * Set 'html' to 0 for plain text and anything else for HTML output.
 * There is no need to call cgi_end() if this fails.
 */
xicgi *cgi_initA(tagitem *tags)
{
	tagitem	*t;
	ulong mime;

	mime = tag_getdata(HTTP_CONTENT, MIME_HTML, &tags);
	cgi_content(mime);
	t = tag_finditem(HTTP_EXPIRY, &tags);
	if(t) {
		printf("Expires: %s\n", (char *)t->data);
	}
	t = tag_finditem(HTTP_PRAGMA, &tags);
	if(t) {
		printf("Pragma: %s\n", (char *)t->data);
	}
	t = tag_finditem(HTTP_LOCATION, &tags);
	if(t) {
		printf("Location: %s\n", (char *)t->data);
	}

	cgi = calloc(1, sizeof(xicgi));
	if(!cgi) {
		printf("\n");
		printf("Memory error\n");
		return 0;
	}

	cgi->mime = mime;
	cgi->gateway = getenv("GATEWAY_INTERFACE");
	cgi->servname = getenv("SERVER_NAME");
	cgi->servsoftware = getenv("SERVER_SOFTWARE");
	cgi->servprotocol = getenv("SERVER_PROTOCOL");
	cgi->servport = getenv("SERVER_PORT");
	cgi->scriptname = getenv("SCRIPT_NAME");
	cgi->docroot = getenv("DOCUMENT_ROOT");
	cgi->remotehost = getenv("REMOTE_HOST");
	cgi->remoteaddr = getenv("REMOTE_ADDR");
	cgi->authtype = getenv("AUTH_TYPE");
	cgi->remoteuser = getenv("REMOTE_USER");
	cgi->remoteident = getenv("REMOTE_IDENT");
	cgi->contenttype = getenv("CONTENT_TYPE");
	cgi->servadmin = getenv("SERVER_ADMIN");

	cgi->httpfrom = getenv("HTTP_FROM");
	cgi->httpaccept = getenv("HTTP_ACCEPT");
	cgi->httpuseragent = getenv("HTTP_USER_AGENT");
	cgi->httpreferer = getenv("HTTP_REFERER");
	cgi->httpacceptlang = getenv("HTTP_ACCEPT_LANGUAGE");
	return cgi;
}

xicgi *cgi_init(ulong tag1, ... )
{
	XI_VARARG(cgi_initA((tagitem *)&tag1), xicgi)
}
