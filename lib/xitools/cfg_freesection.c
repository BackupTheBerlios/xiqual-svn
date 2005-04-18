#include "xios.h"
#include "prefs.h"

/**
 Free a preferences section and its variables.
@doc
 If you used cfg_loadprefs() to load the structures from a file,
 this function will be called as a destructor on each section.
 A simple list_free() on the List will deallocate all resources.
@param prefs Preferences structure (section), as created by cfg_newsection().
@author Ronny Bangsund
@see preferences, cfg_loadprefs(), cfg_newsection()
*/
void cfg_freesection(preferences *prefs)
{
	variable	*var;

	if(!prefs) return;

	while((var = (variable *)prefs->variables.head)) {
		list_remove(&prefs->variables, (Node *)var);
		(void)cfg_freevar(var);
	}

	if(prefs->name) free(prefs->name);
	free(prefs);
}
