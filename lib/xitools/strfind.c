/*
 * Find string in other string, case sensitive or not
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: strfind.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: strfind.c,v $
 * Revision 1.4  2003/12/11 08:54:55  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.1  2001/11/21 20:24:45  nny
 * Moved definitions for strings into its own header file, and added a better
 * needle-in-haystack routine for case-insensitive searches.
 *
 */

#include "xios.h"


/**
 Looks for a string within another string.
@doc
 This is both a case-insensitive version of, and a wrapper around, strstr().
 If sense is non-zero, strstr() will be called. If zero, an insensitive search
 will be used.
@param haystack A string to look inside
@param needle The string to look for in haystack
@param sense Case-sensitivity (TRUE or FALSE)
@return A pointer to the position in haystack where needle was found, or NULL
@author Ronny Bangsund
@see strstr()
*/
char *strfind(char *haystack, char *needle, int sense)
{
	int	len, len2, pos = 0;

	if(!(haystack || needle))
		return NULL;

	if(needle[0] == 0) return haystack;

	if(sense) {
		return strstr(haystack, needle);
	}

	len = strlen(haystack);
	len2 = strlen(needle);
	len -= len2;
	while(pos < len) {
		if(strncasecmp(haystack+pos, needle, len2)==0) return haystack+pos;
		pos++;
	}
	return NULL;
}
