#include "xios.h"
#include "prefs.h"
#include "xistr.h"

// Environment variables: Create names from sectionname_variablename and check.

variable *cfg_newvar(ulong tag1, ... )
{
	XI_VARARG(cfg_newvarA((tagitem *)&tag1), variable)
}

variable *cfg_newvarA(tagitem *tags)
{
	tagitem	*ti;
	variable	*var;
	List	*list=0;
	char	*value=0;

	var = calloc(1, sizeof(variable));
	if(!var) return NULL;

	ti = tag_next(&tags);
	while(ti) {
		switch(ti->tag) {
		case PREFS_NAME:
			var->name = strdup((char *)ti->data);
			break;
		case VAR_VALUE:
			value = (char *)ti->data;
			break;
		case VAR_TYPE:
			var->type = ti->data;
			break;
		case PREFS_LIST:
			list = (List *)ti->data;
			break;
		}
		ti = tag_next(&tags);
	}
	if(!var->name) {
		free(var);
		return NULL;
	}
	switch(var->type) {
	case V_BOOL:
		if(value) {
			var->value = cfg_isbool(value);
		} else {
			var->value = 1;
		}
		break;
	case V_STRING:
		if(value) {
			var->string = strdup(value);
		}
		if(!var->string) {
			cfg_freevar(var);
			return NULL;
		}
		break;
	case V_VALUE:
		var->value = (int)value;
		break;
	}
	list_add(list, (Node *)var);
	return var;
}
