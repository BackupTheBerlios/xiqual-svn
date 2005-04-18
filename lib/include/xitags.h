/*
 * Tags and defines
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: xitags.h,v 1.10 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: xitags.h,v $
 * Revision 1.10  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.9  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.6  2001/11/13 19:26:33  nny
 * Removed tag_inarray.c, as it was just a waste of space.
 * Added CVS variables to most files.
 *
 */
#if !defined(_XIQUAL_TAGS_H)
#define _XIQUAL_TAGS_H

#include "xios.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 A tagitem.
@doc
 Many Xiqual function calls accept an array of these as its
 sole parameter.
@author Ronny Bangsund
@see tag_alloclist(), tag_freelist(), tag_finditem(), tag_getdata(), tag_next()
*/
typedef struct tagitem {
/**
 A program/library-specific ID.
@doc
 Should be fairly unique within the program or library, and not equal to one
 of TAG_DONE, TAG_IGNORE, TAG_MORE or TAG_SKIP.
*/
	ulong	tag;
/**
 The tagitem's data.
@doc
 Can be just about anything that fits in a pointer-sized integer. Common uses are flags, pointers
 to further data/strings or simply integers.
*/
	ulong	data;
} tagitem;

#define TAG_DONE	0	/* End taglists with this in the last tagitem's tag */
#define TAG_END		TAG_DONE
#define TAG_IGNORE	1	/* Ignore just this item */
#define TAG_MORE	2	/* Pointer to another array of tagitems (end of this array */
#define TAG_SKIP	3	/* Skip this plus the number of items specfied in data */

#define TAG_USER		0xff00

extern tagitem *tag_alloclist(ulong numtags);
extern void tag_freelist(tagitem **taglist);
extern tagitem *tag_next(tagitem **taglist);
extern tagitem *tag_finditem(ulong id, tagitem **taglist);
extern ulong tag_getdata(ulong id, ulong defaultval, tagitem **taglist);

/* These macros saves typing/cut & paste */

/**
 Macro for vararg taghandling.
@doc
 This macro is used internally in Xiqual to process the taglists passed
 to many of its functions. It may also be of use if anyone else wants to
 make use of tags in their functions.

 Examples of its use can be found in the Xiqual sources. Basically, the
 calling method in Xiqual has the function taking arguments (ulong tag1, ...),
 then calling XI_VARARG(function_nameA((tagitem *)&tag1), argtype).
 Not much easier, but less typing.
@param call A function call, passing (tagitem *)&tag1 as one of its parameters.
 You take all the parameters not automatically handled by this macro in this.
@param ret A typedef for the returnvalue. Note: this must be a pointer.
 Not very flexible, but these macros are for special purposes, anyway.
@return Whatever you pass as parameter ret.
@see XI_VOIDARG(), va_start(), va_arg(), va_end()
*/
#define XI_VARARG(call, ret) \
	va_list ti; \
	ret *retval; \
	va_start(ti, tag1); \
	retval = call; \
	va_end(ti); \
	return retval;
/**
 Macro for vararg taghandling.
@doc
 This works like XI_VARARG(), but has no return value.
@param call As XI_VARARG().
@see XI_VARARG(), va_start(), va_arg(), va_end()
*/
#define XI_VOIDARG(call) \
	va_list ti; \
	va_start(ti, tag1); \
	call; \
	va_end(ti);


#if defined(__cplusplus)
}
#endif

#endif	/* EOF */
