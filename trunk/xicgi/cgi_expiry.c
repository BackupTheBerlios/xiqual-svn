/*
 * Calculate expiry dates from current time.
 * Use this to generate some sane expiry times for cookies.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: cgi_expiry.c,v 1.2 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: cgi_expiry.c,v $
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

#include "stdarg.h"
#include "string.h"
#include "time.h"

#include "xicgi.h"
#include "xitags.h"

void cgi_expiryA(tagitem *tags)
{
	ulong	weeks=0, days=0, hours=0, minutes=0, seconds=0;
	tagitem	*t;
	char	*buf;
	time_t	tt;

	if(!tags)
		return;

	tt = time(NULL);

	/* A buffer to hold the date string must be specified */
	t = tag_finditem(CGI_BUFFER, &tags);
	if(!t)
		return;

	buf = (char *)t->data;
	t = tags;
	while(t) {
		switch(t->tag) {
		case CGI_DATE_WEEKS:
			weeks = t->data;
			break;
		case CGI_DATE_DAYS:
			days = t->data;
			break;
		case CGI_DATE_HOURS:
			hours = t->data;
			break;
		case CGI_DATE_MINUTES:
			minutes = t->data;
			break;
		case CGI_DATE_SECONDS:
			seconds = t->data;
			break;
		}
		t = tag_next(&tags);
	}
	tt += seconds;
	tt += minutes*60;
	tt += hours*60*60;
	tt += days*60*60*24;
	tt += weeks*60*60*24*7;
	sprintf(buf, "%s", ctime(&tt));
	/* Strip off \n at end */
	buf[strlen(buf)-1] = 0;
}

void cgi_expiry(ulong tag1, ... )
{
	XI_VOIDARG(cgi_expiryA((tagitem *)&tag1))
}
