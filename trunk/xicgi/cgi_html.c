/*
 * Calling these also gives you the necessary HTML and BODY tags.
 * It is bad form to not supply a title for your document.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: cgi_html.c,v 1.2 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: cgi_html.c,v $
 * Revision 1.2  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.11  2001/12/19 09:00:57  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"

#include "xicgi.h"

void cgi_content(ulong type)
{
	printf("Content-type: ");
	switch(type) {
	case MIME_HTML:
		printf("text/html");
		break;
	case MIME_TEXT:
		printf("text/plain");
		break;
	case MIME_JPEG:
		printf("image/jpeg");
		break;
	case MIME_PNG:
		printf("image/png");
		break;
	case MIME_GIF:
		printf("image/gif");
		break;
	default:
		printf("text/plain");
	}
	printf("\n");
}

void cgi_title(char *title)
{
	printf("\n<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n<HTML>\n<HEAD>\n<TITLE>%s</TITLE>\n</HEAD>\n", title);
}

void cgi_body(char *attr)
{
	printf("<BODY");
	if(attr) {
		printf(" %s", attr);
	}
	printf(">\n");
}
