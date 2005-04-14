#include "xios.h"
#include "prefs.h"

/**
 Free a variable and its strings.
@param var Variable structure of any type.
@return Next variable.
@author Ronny Bangsund
@see preferences, variable, cfg_newvar()
*/
variable *cfg_freevar(variable *var)
{
	variable	*v;

	if(!var) return NULL;

	v = var->next;
	if(var->name) free(var->name);
	if(var->string) free(var->string);
	free(var);
	return v;
}
