/*
 * Read the next chunk's header information
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_readchunkheader.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_readchunkheader.c,v $
 * Revision 1.2  2003/05/30 08:54:58  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.4  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 * Revision 1.3  2001/11/16 21:24:12  nny
 * Changes to accomodate the new iff_rewind() function.
 *
 */

#include "stdio.h"

#include "iffparse.h"
#include "xiendian.h"

int iff_readchunkheader(IFFHandle *handle)
{
	ulong	header[2];
	size_t	read;

	if(!handle)
		return FALSE;

	handle->prevchunksize = handle->chunksize;

	read = fread(&header, 4, 2, handle->iff);
	if(!read) return FALSE;

	handle->id = _SwapBE32(header[0]);
	handle->chunksize = _SwapBE32(header[1]);
	handle->chunksize += handle->chunksize %2;
	handle->pos += 8;

//	if(handle->id == MAKE_ID('F','O','R','M')) {
//		read = fread(&header, 4, 1, handle->iff);
//		if(handle->chunksize) {
//			handle->subtype = _SwapBE32(header[0]);
//			handle->subsize = handle->chunksize;
//			handle->chunksize = 0;
//		}
//		handle->pos += 4;
//	}
	return TRUE;
}
