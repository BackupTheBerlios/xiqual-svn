/*
 * Go to next valid tag, or return 0 if no more tags exist.
 * NOTE: This function modifies the actual taglist pointer!
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
