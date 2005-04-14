/*
 * Finish off a subform
 *
 * $Author: nny $
 * $Date: 2003/05/30 09:02:24 $
 * $Id: iff_endsubform.c,v 1.1 2003/05/30 09:02:24 nny Exp $
 *
 * $Log: iff_endsubform.c,v $
 * Revision 1.1  2003/05/30 09:02:24  nny
 *
 * Added Files:
 *  	font/font.def font/gnat.iff font/gnat.png include/lui.h
 *  	include/lui_button.h include/lui_list.h include/lui_msg.h
 *  	xitools/iff_endsubform.c xitools/iffp_new.c
 *
 * Revision 1.3  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"

#include "iffparse.h"
#include "xiendian.h"

void iff_endsubform(IFFHandle *handle)
{
	ulong	size[1];

	fseek(handle->iff, -(handle->subsize ) -8, SEEK_END);
	size[0] = _SwapBE32(handle->subsize);
	fwrite(&size, 4, 1, handle->iff);
	fseek(handle->iff, 0, SEEK_END);
}
