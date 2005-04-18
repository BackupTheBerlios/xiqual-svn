#include "lists.h"
#include "prefs.h"

/**
 Find a named section in a list.
@doc
 The search starts at the head every time; use cfg_findnext() to search
 from the current node.
@param list List of sections (preferences structures).
@param sectionname An exact match to look for.
@return First matching section, or NULL.
@author Ronny Bangsund
@see preferences, cfg_findnext()
*/
preferences *cfg_findsection(List *list, char *sectionname)
{
	if((!sectionname) || (!list)) return NULL;

	return (preferences *)node_findbyname(list->head, sectionname);
}
