#include "xios.h"
#include "prefs.h"

/**
 Allocate a new preferences section.
@doc
 This is used by cfg_loadprefs() when parsing the loaded file.

 Accepted tags:

 <B>PREFS_NAME</B>: A name for the section. This is optional,
 but strongly recommended to include.

 <B>PREFS_STRINGS</B>: <B>TRUE</B> if you want to load the file as strings.

 <B>PREFS_LIST</B>: List to add section to. Strongly recommended.
@return A section (preferences structure) if all went well.
@author Ronny Bangsund
@see List, preferences, variable, cfg_loadprefs(), cfg_newvar()
*/
preferences *cfg_newsection(ulong tag1, ... )
{
	XI_VARARG(cfg_newsectionA((tagitem *)&tag1), preferences)
}

preferences *cfg_newsectionA(tagitem *tags)
{
	tagitem	*ti;
	preferences	*prefs;
	List	*list=0;

	prefs = calloc(1, sizeof(preferences));
	if(!prefs) return NULL;

	memset(prefs, 0, sizeof(preferences));
	ti = tag_next(&tags);
	while(ti) {
		switch(ti->tag) {
		case PREFS_NAME:
			prefs->name = strdup((char *)ti->data);
			break;
		case PREFS_STRINGS:
			prefs->stringonly = ti->data;
			break;
		case PREFS_LIST:
			list = (List *)ti->data;
			break;
		}
		ti = tag_next(&tags);
	}
	list_add(list, (Node *)prefs);
	return prefs;
}
