/*
 * If you can't handle the chunk data, use this.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: iff_skipchunkdata.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: iff_skipchunkdata.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.5  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"

#include "iffparse.h"
#include "xiendian.h"

void iff_skipchunkdata(IFFHandle *handle)
{
	if(!handle)
		return;

	handle->pos += handle->chunksize;
	fseek(handle->iff, handle->chunksize, SEEK_CUR);
}
