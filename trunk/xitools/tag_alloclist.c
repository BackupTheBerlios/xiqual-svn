/*
 * Allocate a taglist with room for numtags items
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: tag_alloclist.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: tag_alloclist.c,v $
 * Revision 1.4  2003/12/11 08:54:55  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.6  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "xitags.h"


/**
 Allocate a tag array big enough for numtags items.
@doc
 This function is dusty, and may disappear soon.
@param numtags The number of tags to hold.
@author Ronny Bangsund
@see tag_finditem(), tag_freelist(), tag_getdata(), tag_next()
*/
tagitem *tag_alloclist(ulong numtags)
{
	tagitem	*tag;
	ulong	size;

	size = numtags * sizeof(tagitem);
	tag = calloc(1, size);
	return tag;
}
