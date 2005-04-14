/*
 * Definitions for Xiqual CGI
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: xicgi.h,v 1.20 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: xicgi.h,v $
 * Revision 1.20  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.19  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.16  2001/11/15 16:18:38  nny
 * cgi_findval.c added; return the data a named cgivar points to.
 *
 * Revision 1.15  2001/11/14 14:04:06  nny
 * Extracted file_getsize.c from files.c
 * Added cgi_include.c to make it easy to include bits of HTML.
 *
 * Revision 1.14  2001/11/13 19:26:33  nny
 * Removed tag_inarray.c, as it was just a waste of space.
 * Added CVS variables to most files.
 *
 */

#if !defined(XIQUAL_CGI_H)
#define XIQUAL_CGI_H

#include "lists.h"
#include "xitags.h"

#ifdef __cplusplus
extern "C" {
#endif

/* For storing environment - created by cgi_init() */
typedef struct xicgi {
	ulong	mime;	/* TRUE if HTML is to be output, FALSE if plain text */
	char *gateway;
	char *servname;
	char *servsoftware;
	char *servprotocol;
	char *servport;
	char *method;		/* The literal string "GET" or "POST" */
	char *pathinfo;
	char *pathtrans;
	char *scriptname;
	char *docroot;
	char *query;			/* Query when passed through GET */
	char *remotehost;
	char *remoteaddr;
	char *authtype;
	char *remoteuser;
	char *remoteident;
	char *contenttype;
	char *servadmin;	/* This may not be available */
	long contentlength;	/* Length of data passed through POST */
	char *httpfrom;
	char *httpaccept;
	char *httpuseragent;
	char *httpreferer;
	char *httpacceptlang;
} xicgi;

extern xicgi *cgi;

typedef struct cookie {
	char	*name;
	char	*value;
} cookie;

typedef struct cgivar {
	struct cgivar	*next;
	struct cgivar	*prev;
	char	*name;
	char	*value;
} cgivar;

/* Tags for cgi_init() and cgi_setcookie() */
enum {
	CGI_BASE = TAG_USER+0x8888,
	COOKIE_NAME,
	COOKIE_DATA,
	HTTP_EXPIRY,
	COOKIE_PATH,
	COOKIE_SECURE,
	COOKIE_DOMAIN,
	HTTP_LOCATION,
	COOKIE_DONE,
	HTTP_PRAGMA,
	HTTP_CONTENT
};

/*
 * Date conversion tags - should be enough to cover all sane case.
 * For instance, to expire a cookie in 24 hours, you can use either
 * CGI_DATE_HOURS, 24 or CGI_DATE_DAYS, 1.
 */
#define CGI_DATE_WEEKS	CGI_BASE+100
#define CGI_DATE_DAYS		CGI_BASE+101
#define CGI_DATE_HOURS	CGI_BASE+102
#define CGI_DATE_MINUTES	CGI_BASE+103
#define CGI_DATE_SECONDS	CGI_BASE+104
#define CGI_BUFFER		CGI_BASE+105

#define MIME_HTML		0
#define MIME_TEXT		1
#define MIME_JPEG		2
#define MIME_PNG		3
#define MIME_GIF			4

extern void cgi_content(ulong type);
extern xicgi *cgi_initA(tagitem *tags);
extern xicgi *cgi_init(ulong tag1, ... );
extern void cgi_end();
extern void cgi_title(char *title);
extern void cgi_body(char *attr);
extern int cgi_checkform();
extern List *cgi_form();
extern void cgi_unescape(char *string, char *dest);
extern int cgi_include(char *filename);
extern char *cgi_findval(List *l, char *string);

extern void cgi_setcookieA(tagitem *tags);
extern void cgi_setcookie(ulong tag1, ... );
extern cookie *cgi_getcookie(char *name);
extern void cgi_freecookie(cookie *c);
extern void cgi_clearcookie(char *name);
extern void cgi_expiryA(tagitem *tags);
extern void cgi_expiry(ulong tag1, ... );

#ifdef __cplusplus
}
#endif

#endif	/* EOF */
