#include "xios.h"

/**
 Internal function for cfg_loadprefs().
@doc
 This will return <em>TRUE</em> if a string pointed to is a boolean value.
 There is a special case, however; if there is no string, it also
 returns <em>TRUE</em>. cfg_loadprefs() will pass the value from strtok_r()
 unchecked because I couldn't be arsed to do it any other way.
@param value A string pointer (probably).
@return <em>TRUE</em> if the string pointed to is a boolean value, or NULL.
@author Ronny Bangsund
@see variable, cfg_loadprefs()
*/
int cfg_isbool(char *value)
{
	if(!value)
		return 1;

	if(strcasecmp(value, "yes") == 0) return 1;

	if(strcasecmp(value, "true") == 0) return 1;

	if(strcasecmp(value, "no") == 0) return 1;

	if(strcasecmp(value, "false") == 0) return 1;

	return 0;
}
