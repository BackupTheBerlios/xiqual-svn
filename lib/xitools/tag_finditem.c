/*
 * Return tag if it exists in a taglist
 *
 */

#include "stdio.h"

#include "xitags.h"


/**
 Look for a tag identifier in a taglist, and return a pointer to the tagitem.
@param tag A tag id (program/library specific)
@param taglist A pointer to a tagarray's address
@return A pointer to a tagitem entry matching the tag parameter, or NULL.
@author Ronny Bangsund
@see tag_alloclist(), tag_freelist(), tag_getdata(), tag_next()
*/
tagitem *tag_finditem(unsigned long tag, struct tagitem **taglist)
{
	struct tagitem *t;

	while((t = tag_next(taglist))) {
		if(t->tag == tag) return t;
	}
	return NULL;
}
