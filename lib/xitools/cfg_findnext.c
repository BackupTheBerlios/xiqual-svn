#include "lists.h"
#include "prefs.h"

/**
 Find a named section starting at a specific section.
@doc
 The given section is also compared against the name.
@param prefs Preferences section to start comparison at.
@param sectionname An exact match to look for.
@return First matching section, or NULL.
@author Ronny Bangsund, Shane O'Neill
@see preferences, cfg_findsection()
*/
preferences *cfg_findnext(preferences *prefs, char *sectionname)
{
	return (preferences *)node_findbyname((Node *)prefs, sectionname);
}
