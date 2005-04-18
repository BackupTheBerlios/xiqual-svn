/*
 * Look for a tag ID and return corresponfing data, if found
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: tag_getdata.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: tag_getdata.c,v $
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
 Find a tagitem's data by tag ID.
@param tag The program-specific identifier to look for
@param defaultval Value to return if a tag is not found
@param taglist A pointer to a tagarray's address
@return Data in the tagitem structure, or defaultval if not found.
@author Ronny Bangsund
@see tag_alloclist(), tag_freelist(), tag_finditem(), tag_next()
*/
unsigned long tag_getdata(unsigned long tag, unsigned long defaultval, tagitem **taglist)
{
	tagitem	*t;

	t = tag_finditem(tag, taglist);
	if(t) {
		return t->data;
	}
	return defaultval;
}
