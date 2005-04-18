/*
 * Return tag if it exists in a taglist
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: tag_finditem.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: tag_finditem.c,v $
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
 * Revision 1.9  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
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
tagitem *tag_finditem(ulong tag, struct tagitem **taglist)
{
	struct tagitem *t;

	while((t = tag_next(taglist))) {
		if(t->tag == tag) return t;
	}
	return NULL;
}
