/*
 * Convert string with escaped symbols changed to proper characters.
 * You should split input arguments into variables first, of course.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: cgi_unescape.c,v 1.2 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: cgi_unescape.c,v $
 * Revision 1.2  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.3  2001/12/19 09:00:57  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"
#include "stdlib.h"

void cgi_unescape(char *string, char *dest)
{
	char hex[3];

	while(string[0]) {
		switch(string[0]) {
		case '+':
			string++;
			dest[0] = ' ';
			dest++;
			break;
		case '%':
			string++;
			hex[0] = string[0];
			hex[1] = string[1];
			hex[2] = 0;
			sprintf(dest, "%c", (int)strtol(hex, 0, 16));
			string += 2;
			dest++;
			break;
		default:
			dest[0] = string[0];
			dest++;
			string++;
		}
	}
}
