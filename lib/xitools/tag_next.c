/*
 * Go to next valid tag, or return 0 if no more tags exist.
 * NOTE: This function modifies the actual taglist pointer!
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: tag_next.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: tag_next.c,v $
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
 * Revision 1.8  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "xitags.h"


/**
 Get next tagitem in the array.
@doc
 Use this function to smooth over any uses of TAG_SKIP, TAG_IGNORE and TAG_MORE.

 NOTE: Designed to work exactly like the Amiga functions from utility.library,
 so the pointer passed in is modified.
@param taglist A pointer to a tagarray's address
@return A pointer to the next tagitem in the array, or NULL (=TAG_END).
@author Ronny Bangsund
@see tag_alloclist(), tag_freelist(), tag_finditem(), tag_getdata(), tag_next()
*/
tagitem *tag_next(tagitem **taglist)
{
	tagitem **t;

	if(!(*taglist))
		return NULL;

	t = taglist;

	while(1) {
		switch((*t)->tag) {
		case TAG_MORE:
			if(!(*t)->data)
				return NULL;

			(*t) = (void *)(*t)->data;
			continue;
		case TAG_SKIP:
			(*t) = (*t)->data; //FIXME: Look again when not drunk/stoned/etc.
			continue;
		case TAG_IGNORE:
			break;
		case TAG_END:
			return NULL;
		default:
			return (*t)++;
		}
		(*t)++;
	}
	return (*t)++;
}
