/*
 * Search for a variable name (case insensitive) in a
 * preferences structure.
 */

#include "xios.h"
#include "errno.h"

#include "lists.h"
#include "prefs.h"

/**
 Find a variable in a prefs section.
@doc
 This search is case insensitive.
 The search starts at the head every time.
@param prefs Preferences structure to look in
@param name An exact match to look for.
@return First matching variable, or NULL.
@author Shane O'Neill
@see preferences
*/
variable *cfg_findvar(preferences *prefs, char *name)
{
	if(!prefs) return NULL;

	return (variable *)node_findbyname(prefs->variables.head, name);
}
