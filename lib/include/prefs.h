/*
 * Preferences structures and definitions
 *
 * Files are either a simple list of  name = value pairs, or a
 * sectioned list of entries, each block starting with [category],
 * where "category" is replaced with a meaningful name.
 * Variables in the preferences structure are case-insensitive.
 *
 * Known problems:
 * No way of recognising floating point numbers. You'll have to do
 * something tricky yourself (like preceding floats with a symbol)
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: prefs.h,v 1.12 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: prefs.h,v $
 * Revision 1.12  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.11  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 */

#if !defined(_XIQUAL_PREFS_H)
#define _XIQUAL_PREFS_H

#include "xios.h"

#include "lists.h"
#include "files.h"
#include "xicommon.h"
#include "xitags.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Return codes (in errno) */
enum {
	PREFS_ERR = 1,		/* Unknown error */
	PREFS_NOMEM,		/* All or some allocations failed */
	PREFS_NOSUCHFILE,	/* Missing file */
	PREFS_NOLOAD,		/* Unspecified file error */
	PREFS_NODIR			/* Couldn't chdir() (permissions?) */
};

#define PREFS_NUMERRORS PREFS_NODIR

/**
 A preferences section.
@doc
 Each preferences file may consist of several sections like this.
@author Ronny Bangsund
@see cfg_loadprefs(), cfg_newsection(), cfg_newvar(), cfg_addvar(), variable
*/
typedef struct preferences {
	struct preferences	*next;
	struct preferences	*prev;
/**
 Name of section.
*/
	char	*name;
/**
 Directory we prefer to save this in.
*/
	char	*dir;
/**
 A chain of variables.
*/
	List	variables;
/**
 Boolean - true if we don't want to convert numbers to integers.
@doc
 When true, the function that scans each line will just blindly
 copy the variable data to the string element. The type will be
 set to PREFS_STRING.
*/
	char	stringonly;
} preferences;


extern List *cfg_loadprefs(ulong tag1, ... );
extern List *cfg_loadprefsA(tagitem *tags);
extern preferences *cfg_newsection(ulong tag1, ... );
extern preferences *cfg_newsectionA(tagitem *tags);
extern void cfg_freesection(preferences *prefs);
extern int cfg_saveprefs(List *list, char *dirname, char *filename);

extern variable *cfg_newvar(ulong tag1, ... );
extern variable *cfg_newvarA(tagitem *tags);
extern variable *cfg_freevar(variable *var);

extern void cfg_mapfromenv(List *variables, ulong tag1, ... );
extern void cfg_mapfromenvA(List *variables, tagitem *tags);
extern int cfg_isbool(char *value);

extern preferences *cfg_findsection(List *list, char *name);
extern preferences *cfg_findnext(preferences *prefs, char *name);
extern variable *cfg_findvar(preferences *, char *);

enum {
	PREFS_BASE = (('G'<<24) | ('L'<<16) | ('O'<<8) | ('B')),
	PREFS_FILENAME,	/* Load this file */
	PREFS_NAME,	/* Name of preferences section or variable */
	PREFS_STRINGS,	/* Load file as strings; don't convert numbers to integers */
	PREFS_LIST,	/* Add preferences/variables structures to supplied List */
	PREFS_STRING_HOOK,	/* When you want special reading of a string */
	PREFS_VALUE_HOOK,	/* Same for values */
	VAR_TYPE,
	VAR_VALUE
};

#define prefs_findnext(section, sectionname)	node_findbyname(section, sectionname)

#ifdef __cplusplus
}
#endif


#endif /* _XIQUAL_PREFS_H */
