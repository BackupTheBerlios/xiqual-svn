/*
 * Write chunk contents
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: iff_writechunkdata.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: iff_writechunkdata.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.3  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"

#include "iffparse.h"


/**
 Write data to an IFF where you have just started a chunk.
@doc
 Before you can write arbitrary data to an IFF, you must
 create a chunk with iff_newchunk(). You may repeat this
 call any number of times if not all data is available
 on the first call.
@author Ronny Bangsund
@see iff_new(), iff_newchunk()
*/
size_t iff_writechunkdata(IFFHandle *handle, char *buffer, size_t size)
{
	IFFChunk	*chunk;
	size_t	written;

	if(!(handle && buffer && size)) return 0;

	chunk = (IFFChunk *)handle->chunks.tail;
	if(!chunk) return 0;

	written = fwrite(buffer, 1, size, handle->iff);
	handle->size += written;
	chunk->size += written;
	handle->pos += written;
	return written;
}
