/*
 * Extra string functions.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: xistr.h,v 1.6 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: xistr.h,v $
 * Revision 1.6  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.5  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.2  2002/06/09 21:50:11  nny
 *
 * Added a list of SON's changes, plus a way to explicitly load a prefs file as
 * strings only (used in a webshop at the moment).
 *
 * Revision 1.1  2001/11/21 20:24:44  nny
 * Moved definitions for strings into its own header file, and added a better
 * needle-in-haystack routine for case-insensitive searches.
 *
 */

#if !defined(XIQUAL_STRING_H)
#define XIQUAL_STRING_H

#include "wchar.h"
#include "xios.h"
#include "lists.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 The token structure.
@see strtokenise(), List
*/
typedef struct strtoken {
/**
 A copy of the original string.
@doc
 There will be "holes" where the delimiters were,
 so use the tokens list to access.
*/
	char	*s;
/**
 The length of the string memory.
@doc
 Used internally, but might be useful to users.
*/
	size_t	len;
/**
 The delimiter string.
*/
	char	*d;
/**
 Length of the delimiter string.
@doc
 Maybe not the most useful element.
*/
	size_t	dlen;
/**
 The individual tokens or words.
@doc
 The size element in the list will contain
 the number of tokens. Each token is a plain
 Node structure with its data pointer being
 a pointer to a string (the token).
*/
	List	tokens;
} strtoken;


extern char *strlower(char *word);
extern char *strupper(char *word);
extern char *strsplit(char *text);
extern char *strfind(char *haystack, char *needle, int sense);
extern int str_getvalue(int *val, char *str, int offset);
extern int strisnum(char *text);
extern strtoken *str_tokenise(char *s, char *delim);
void str_freetoken(strtoken *t);

#ifdef __cplusplus
}
#endif

#endif /* EOF */
