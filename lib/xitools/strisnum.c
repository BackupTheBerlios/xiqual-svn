#include "xios.h"

/**
 Check if a string is all numbers and whitespace.
@doc
 Any regular alphabetic symbols or punctuation encountered invalidate the string
 as a numeric string. This can be used to check a multiple-value string.
 The string must be NULL-terminated, or the call never ends.
@param text A string to check.
@return <em>TRUE</em> if the string only contains whitespace and numbers.
@author Ronny Bangsund
*/
int strisnum(char *text)
{
	char	c;

	c = *text;
	while(c) {
		if((isalpha(c)) || (ispunct(c))) {
			return 0;
		}
		text++;
		c = *text;
	}
	return 1;
}
