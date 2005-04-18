/*
 * Convert a whole string to uppercase
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: strupper.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: strupper.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.3  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "xios.h"

/**
 Turn a string into all-uppercase characters.
@doc
 Uses toupper(), so locale will be handled if supported by the local C library.
@param text A string to change the case of
@return A pointer to the string passed in
@author Ronny Bangsund
@see strlower(), toupper()
*/
char *strupper(char *text)
{
	char *c;
	int ch;

	c = text;

	while (*c) {
		ch = c[0];
		c[0] = toupper(ch);
		c++;
	}

	return text;
}
