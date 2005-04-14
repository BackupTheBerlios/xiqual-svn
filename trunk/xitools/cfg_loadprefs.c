#include "xios.h"
#include "lists.h"
#include "prefs.h"
#include "xistr.h"

/**
 Load preferences from an INI-style file.
@doc
 The minimum configuration file accepted has one line with a variable name
 on it:

 mybool

 A standalone variable like this would be considered a boolean variable.
 When loaded into memory, you would be returned a List containing only
 one preferences section, with no name.

 A larger file should be divided into sections, like this:

 [main]<BR>mybool<BR>something = something else

 The List returned from cfg_loadprefs() would now contain one preferences
 section named "main", with two variable - mybool is a boolean (set to TRUE),
 and the second variable would be named "something", a string variable set
 to "something else".

 Whenever a new set of square brackets are encountered, the name between
 them is used in a new preferences section. All the following variables
 will be added to that until yet another section name is found.

 Legal variable names are anything not containing whatever your system
 considers whitespace.

 Accepted tags:

 <B>PREFS_FILENAME</B>: Name of file to load. Required.

 <B>PREFS_STRINGS</B>: Load all data as string variables.

 <B>PREFS_STRING_HOOK</B>, <B>PREFS_VALUE_HOOK</B>:
 Normally, variables are tokenised to include the whole string (for string
 variables) or get numbers until atoi() returns (integers). Use these tags
 to specify an alternate function to handle the processing of the value.
 Useful if a string contains multiple filenames, or several numbers to process.

 A variable structure will not be created, but the hook gets the name
 of the variable that otherwise would be created, along with the
 preferences section pointer and a string containing the rest of the
 line. You will then need to duplicate any strings passed with strdup().

 The hook functions are declared as such in the sourcecode:

 void	(*strhook)(struct preferences *section, char *varname, char *values);

 void	(*valhook)(struct preferences *section, char *varname, char *values);

@author Ronny Bangsund
@see preferences, variable, cfg_findsection(), cfg_findvar(), list_free()
 strtok(), atoi(), strdup()
*/
extern List *cfg_loadprefs(ulong tag1, ... )
{
	XI_VARARG(cfg_loadprefsA((tagitem *)&tag1), List)
}

extern List *cfg_loadprefsA(tagitem *tags)
{
	tagitem	*ti;
	List	*list;
	void	(*strhook)(struct preferences *section, char *varname, char *values);
	void	(*valhook)(struct preferences *section, char *varname, char *values);
	char	*filename=0;
	strtoken	*lines, *words;
	Node	*line, *token;
	char	*wordptr, *varname, *varvalue;
	int	stringonly=0;
	autofile	*af;
	preferences	*section=0;

	strhook = 0; valhook = 0;

	ti = tag_next(&tags);
	while(ti) {
		switch(ti->tag) {
		case PREFS_FILENAME:
			filename = (char *)ti->data;
			break;
		case PREFS_STRINGS:
			stringonly = ti->data;
			break;
		case PREFS_STRING_HOOK:
			strhook = (void *)ti->data;
			break;
		case PREFS_VALUE_HOOK:
			valhook = (void *)ti->data;
			break;
		}
		ti = tag_next(&tags);
	}

	if(!filename) return NULL;

	list = list_new(cfg_freesection);

	if(!list) return NULL;


	af = file_new(filename);
	if(!af) {
		list_free(list);
		return NULL;
	}

	file_load(af);
	lines = str_tokenise(af->buf, "\n");
	line = lines->tokens.head;
	while(line) {
		words = 0;
		wordptr = line->data;
		if(wordptr[0] == '#') goto endline;
		words = str_tokenise(line->data, "=");
		token = words->tokens.head;
		if(strchr(token->data, '[')) {
			wordptr = strrchr(token->data, ']');
			if(wordptr) wordptr[0] = 0;
			wordptr = (char *)token->data+1;
			while(wordptr[0] > 0) {
				if(isspace(wordptr[0])) wordptr[0] = 0;
				wordptr++;
			}
			section = cfg_newsection(PREFS_NAME, (char *)token->data+1,
				PREFS_STRINGS, stringonly,
				PREFS_LIST, list, TAG_END);
			goto endline;
		}	
		varname = token->data;
		if(token->next) {
			varvalue = token->next->data;
			while(varvalue[0] == ' ') {
				varvalue++;
			}
			wordptr = varname;
			while(wordptr[0] > 0) {
				if(isspace(wordptr[0])) wordptr[0] = 0;
				wordptr++;
			}
		} else {
			varvalue = 0;
		}
		if(!section) {
			section = cfg_newsection(PREFS_STRINGS, stringonly,
				PREFS_LIST, list, TAG_END);
			if(!section) {
				goto endline;
			}
		}
		if(cfg_isbool(varvalue)) {
			(void)cfg_newvar(PREFS_NAME, varname,
				VAR_TYPE, V_BOOL,
				VAR_VALUE, varvalue,
				PREFS_LIST, &section->variables,
				TAG_END);
			goto endline;
		}
		if((stringonly) || (!strisnum(varvalue))) {
			if(strhook) {
				strhook(section, varname, varvalue);
			} else {
				(void)cfg_newvar(PREFS_NAME, varname,
					VAR_TYPE, V_STRING,
					VAR_VALUE, varvalue,
					PREFS_LIST, &section->variables,
					TAG_END);
			}
		} else {
			if(valhook) {
				valhook(section, varname, varvalue);
			} else {
				(void)cfg_newvar(PREFS_NAME, varname,
					VAR_TYPE, V_VALUE,
					VAR_VALUE, atoi(varvalue),
					PREFS_LIST, &section->variables,
					TAG_END);
			}
		}
endline:
		str_freetoken(words);
		line = line->next;
	} /* line */
	str_freetoken(lines);
	file_free(af);
	return list;
}
