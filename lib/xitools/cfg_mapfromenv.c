#include "xios.h"
#include "prefs.h"

/* Find variables in List variables that match any of the PREFS_NAME
 entries in the following taglist. If an environment variable with an
 exact match exists, the variable is changed to that value.
 Future extension: Tag to indicate pattern to match variable names on (env_*..).
 */
void cfg_mapfromenv(List *variables, ulong tag1, ... )
{
	XI_VOIDARG(cfg_mapfromenvA(variables, (tagitem *)&tag1))
}

void cfg_mapfromenvA(List *variables, tagitem *tags)
{
	tagitem	*ti;

	if(!variables)
		return;

	if(!variables->head)
		return;

	ti = tag_next(&tags);
	while(ti) {
		switch(ti->tag) {
		case PREFS_NAME:
			break;
		}
		ti = tag_next(&tags);
	}
}
