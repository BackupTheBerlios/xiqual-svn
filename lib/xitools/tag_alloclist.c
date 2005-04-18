/*
 * Allocate a taglist with room for numtags items
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
tagitem *tag_alloclist(unsigned long numtags)
{
	tagitem	*tag;
	unsigned long	size;

	size = numtags * sizeof(tagitem);
	tag = calloc(1, size);
	return tag;
}
