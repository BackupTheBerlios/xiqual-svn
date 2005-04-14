/*
 * Set a cookie
 * Note: It's rude to set cookies with a lifetime of several years.
 * A couple of weeks should be enough; setting it 35+ years into
 * the future is not nice.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: cgi_setcookie.c,v 1.2 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: cgi_setcookie.c,v $
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

#include "xicgi.h"
#include "xitags.h"

/* The actual function - note that expiry is required */
void cgi_setcookieA(tagitem *tags)
{
	char	*name=0, *data=0, *expiry=0, *path=0, *domain=0, *secure=0, *location=0;
	tagitem	*t;

	t = tags;
	while(t) {
		switch(t->tag) {
		case COOKIE_NAME: name = (char *)t->data; break;
		case COOKIE_DATA: data = (char *)t->data; break;
		case HTTP_EXPIRY: expiry = (char *)t->data; break;
		case COOKIE_PATH: path = (char *)t->data; break;
		case COOKIE_SECURE: secure = (char *)t->data; break;
		case COOKIE_DOMAIN: domain = (char *)t->data; break;
		case HTTP_LOCATION: location = (char *)t->data; break;
		case COOKIE_DONE:
			if(!(name || data || expiry)) {
				return;
			}
			if(!secure) {
				secure = "0";
			}
			printf("Set-Cookie: %s=%s", name, data);
			if(path) {
				printf("; path=%s", path);
			}
			printf("; expires=%s", expiry);
			if(domain) {
				printf("; domain=%s", domain);
			}
			if(secure) {
				printf("; secure=%s", secure);
			}
			if(location) {
				printf("\nLocation: %s", location);
			}
			printf("\n");
			break;
		}
		t = tag_next(&tags);
	}
}

/* The interface */
void cgi_setcookie(ulong tag1, ... )
{
	XI_VOIDARG(cgi_setcookieA((tagitem *)&tag1))
}
