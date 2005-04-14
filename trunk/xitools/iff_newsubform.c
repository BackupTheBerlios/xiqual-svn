/*
 * Create a subform - only one level of nesting allowed.
 *
 * $Author: nny $
 * $Date: 2003/05/30 09:04:31 $
 * $Id: iff_newsubform.c,v 1.1 2003/05/30 09:04:31 nny Exp $
 *
 * $Log: iff_newsubform.c,v $
 * Revision 1.1  2003/05/30 09:04:31  nny
 *
 * Added Files:
 * 	prefs.prefs xitools/iff_newsubform.c
 *
 * Revision 1.3  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"

#include "iffparse.h"
#include "xiendian.h"

int iff_newsubform(IFFHandle *handle, ulong type)
{
	ulong	header[3];

	if((!handle) || (!type))
		return FALSE;

	if(!iff_goodtype(type))
		return FALSE;

	/* Already writing a subform */
	if(handle->subtype)
		return FALSE;

	header[0] = _SwapBE32(ID_FORM);
	header[1] = 0;
	header[2] = _SwapBE32(type);
	handle->chunksize = 0;
	handle->size += 12;
	handle->pos += 12;
	handle->subtype = type;
	handle->subsize = 0;
	fwrite(&header, 4, 3, handle->iff);
	return TRUE;
}
