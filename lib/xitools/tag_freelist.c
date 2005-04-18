/*
 * Free a (possibly chained) taglist
 *
 */

#include "stdlib.h"

#include "xitags.h"


/**
 Free a tagarray previously created with tag_alloclist().
@doc
 This function is just as dusty as tag_alloclist().
@param taglist A pointer to a tagarray's address
@author Ronny Bangsund
@see tag_alloclist(), tag_finditem(), tag_getdata(), tag_next()
*/
void tag_freelist(tagitem **taglist)
{
	tagitem	*t, *next;

	t = (*taglist);
	while(t) {
		next = tag_finditem(TAG_MORE, &t);
		free(t);
		t = next;
	}
}
