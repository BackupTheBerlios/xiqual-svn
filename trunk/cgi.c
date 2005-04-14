/*
 * Demo CGI program for Xiqual Tools
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:26 $
 * $Id: cgi.c,v 1.16 2003/05/30 08:54:26 nny Exp $
 *
 * $Log: cgi.c,v $
 * Revision 1.16  2003/05/30 08:54:26  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.13  2001/11/21 20:25:35  nny
 * Big cleanup; only one single warning left on my system.
 *
 * Revision 1.12  2001/11/13 19:26:32  nny
 * Removed tag_inarray.c, as it was just a waste of space.
 * Added CVS variables to most files.
 *
 */

#include "stdio.h"
#include "stdlib.h"

#include "xicgi.h"

xicgi *cgi;

int main()
{
	cookie	*c;
	char	buf[40];
	List	*mylist;
	cgivar	*var;

	cgi = cgi_init(HTTP_CONTENT, MIME_HTML, TAG_END);
	cgi_expiry(CGI_BUFFER, buf, CGI_DATE_DAYS, 3, TAG_END);
	cgi_setcookie(COOKIE_NAME, "beenhere", COOKIE_DATA, "yes", HTTP_EXPIRY, "0", COOKIE_DONE, 0,
				COOKIE_NAME, "second_cookie", COOKIE_DATA, "2", HTTP_EXPIRY, "0", COOKIE_DONE, 0,
				TAG_END);
	if(cgi) {
		atexit(cgi_end);
		cgi_title("About..");
		cgi_body("bgcolor=#1010bb text=#dddddd");	/* Background set to 'Eye-sore Blue', text to 'Icky White' */
		if(!cgi_checkform()) {
			printf("<P>No POST/GET<BR>\n");
		} else {
			mylist = cgi_form();
			if(mylist) {
				printf("<P>GET/POST arguments:<BR>\n");
				var = (cgivar *)mylist->head;
				while(var) {
					printf("%s = %s<BR>\n", var->name, var->value);
					var = var->next;
				}
				list_free(mylist);
				printf("Done with GET/POST arguments<BR><BR>\n");
			}
		}
		/* Testing that we can actually do this without crashes :P
		 * (Don't ask. It was horrible.)
		 */
		printf("<P>Looking for cookies..<BR>\n");
		c = cgi_getcookie("beenhere");
		if(c) {
			printf("beenhere='%s'<BR>\n", c->value);
			cgi_freecookie(c);
		}
		c = cgi_getcookie("second_cookie");
		if(c) {
			printf("second_cookie='%s'<BR>\n", c->value);
			cgi_freecookie(c);
		}
		printf("Finished looking for cookies.<BR>\n");
		printf("<PRE>%s<BR>\n%s<BR>\n%s<BR>\n%s<BR>\n</PRE>", cgi->gateway, cgi->servname, cgi->servsoftware, cgi->servprotocol);
	}
	return 0;
}
