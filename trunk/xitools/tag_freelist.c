/*
 * Free a (possibly chained) taglist
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: tag_freelist.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: tag_freelist.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.7  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
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
