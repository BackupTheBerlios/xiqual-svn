/*
 * Return offset of next value in a string with multiple values
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: str_getvalue.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: str_getvalue.c,v $
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
 * Revision 1.1  2002/03/24 06:00:23  nny
 *
 * Merged the latest batch of diffs from SON.
 *
*/
#include "xios.h"

/**
 Converts a string to an integer.
@doc
 The string value '*string' ia converted into an integer,
 and stored '*var'.<br>
 This function is capable of returning multiple values within
 one string if they are separated by white spaces. Example:<br>

 offset = str_getvalue(&val, "1 2 3 4", offset);
@param val Pointer to an integer variable to store the next
 converted value.
@param str Pointer to the NULL-terminated string to read from.
@param offset Offset into the string to read from next.
 It should be zero on first call.
@return Offset to next value within the string. This can be used
 on the next call to convert the next value. Returns zero when it
 reaches the end of the string.
@author Shane O'Neill
@see cfg_loadprefs()
*/
int str_getvalue(int *val, char *str, int offset)
{
	while ( ((isdigit(str[offset])) == 0 ) && (str[offset] != '\0')) {
		offset++;
	}

	if (str[offset] != '\0')
		*val = atoi((str+offset));

	//Find next space.
	while ( (isspace(str[offset]) == 0) && (str[offset] != '\0') ) {
		offset++;
	}

	if (str[offset] == '\0')
		return(0);

	return(offset);
}
