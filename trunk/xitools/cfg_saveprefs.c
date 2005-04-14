#include "xios.h"
#include "prefs.h"

/**
 Save a List of preferences sections to file.
@doc
 All sections will be visited in order, saving all variables without
 comments for each. If there is a previous file with the supplied name,
 it will be deleted first.
@param list The list containing pointers to all your preferences sections.
@param dirname An optional directory to change to before saving.
@param filename The filename to save the structure as.
@author Ronny Bangsund
@see List, preferences, variable, cfg_loadprefs(), cfg_newsection()
*/
int cfg_saveprefs(List *list, char *dirname, char *filename)
{
	preferences *prefs;
	variable *var;
	char olddir[500];
	FILE *file;
	size_t size = 0;

	errno = 0;
	olddir[0] = 0;

	if(!list) return 0;

	if(!filename) return 0;

	getcwd(olddir, 500);
	if((dirname) && (chdir(dirname) == -1)) {
		errno = PREFS_NODIR;
		return 0;
	}

	remove(filename);
	file = fopen(filename, "w+");
	if(!file) {
		if(dirname) chdir(olddir);
		return 0;
	}

	prefs = (preferences *)list->head;
	while(prefs) {
		if(prefs->name) {
			size += fprintf(file, "[%s]\n", prefs->name);
		}
		var = (variable *)prefs->variables.head;

		while(var) {
			size += fprintf(file, "%s = ", var->name);
			switch(var->type) {
			case V_STRING:
				size += fprintf(file, "%s\n", var->string);
				break;
			case V_BOOL:
				if(var->value) {
					size += fprintf(file, "TRUE\n");
				} else {
					size += fprintf(file, "FALSE\n");
				}
				break;
			case V_VALUE:
				size += fprintf(file, "%d\n", var->value);
				break;
			default:
				size += fprintf(file, "?\n");
			}
			var = var->next;
		}
		size += fprintf(file, "\n");
		prefs = prefs->next;
	}

	fclose(file);

	if(olddir[0]) chdir(olddir);

	return size;
}
