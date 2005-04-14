/*
 * Use this as a generic test of cfg_* and your preferences files.
 * Let me know what it chokes on.
 */
#include "xios.h"
#include "prefs.h"

const char *_boolean(int value)
{
	if(value)
		return "TRUE";

	return "FALSE";
}

int main()
{
	List	*list;
	preferences	*section;
	variable	*var;

	list = cfg_loadprefs(PREFS_FILENAME, "prefs.prefs", TAG_END);
	if(!list)
		return 1;

	section = (preferences *)list->head;
	while(section) {
		printf("\n[%s]\n", section->name);
		var = (variable *)section->variables.head;
		while(var) {
			switch(var->type) {
			case V_BOOL:
				printf("BOOL:%s is %s\n", var->name, _boolean(var->value));
				break;
			case V_VALUE:
				printf("INT:%s is %d\n", var->name, var->value);
				break;
			case V_STRING:
				printf("STR:%s is '%s'\n", var->name, var->string);
				break;
			default:
				printf("Unknown variable type. Could be an error.\n");
				break;
			}
			var = var->next;
		}
		section = section->next;
	}
	list_free(list);

	return 0;
}
