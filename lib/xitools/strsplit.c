/*
 * Split "variable = value" string.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: strsplit.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: strsplit.c,v $
 * Revision 1.4  2003/12/11 08:54:55  nny
 *
 * Modified Files:
 * All.
 *
 */

#include "xios.h"


/**
 Split a variable-setting string.
@doc
 Given a string containing two words, separated by eiher
 whitespace or an equals sign, it returns a pointer to the
 second word (value), after clearing the spaces between.

 The original string is modified so that the pointer
 given only points to the first word.

 Given the string:
	"var = val"
 the original pointer will point to "var" and the returned
 pointer to "val".

 All whitespace around the variable and value is stripped.
@param text A string
@return Pointer to the second word
@author Ronny Bangsund, Shane O'Neill
*/
char *strsplit(char *text)
{
	char	*tmp, *tmp2;

	/* Skip past variable name */
	tmp = text;
	while((isalnum(tmp[0])) || (tmp[0]=='_')) {
		tmp++;
	}

	if(tmp[0] == 10) return NULL;

	/*  Skip past equals sign, tabs and spaces */
	while((isspace(tmp[0])) || (tmp[0] == '=')) {
		tmp[0] = 0;
		tmp++;
	}

	if(tmp[0] == 10)
		return NULL;

	/* Everything following the equals sign is variable data */
	tmp2 = tmp;
	while((tmp2[0]!=0) && (tmp2[0]!=10)) {
		tmp2++;
	}
	tmp2[0] = 0;

	return tmp;
}
