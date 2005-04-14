/*
 * Finish writing chunk and fix chunksize
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_endchunk.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_endchunk.c,v $
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


/**
 Finishes writing a chunk.
@doc
 This should be called when you are done with your
 iff_writechunkdata() calls. If the resulting chunksize
 is odd, an extra null-byte will be added at the end
 of the chunk, and the header will have its size set
 to the new value.
@author Ronny Bangsund
@see iff_new(), iff_writechunkdata()
*/
void iff_endchunk(IFFHandle *handle)
{
	IFFChunk	*chunk;
	ulong	size[1];

	if(!handle->chunks.tail) return;

	chunk = (IFFChunk *)handle->chunks.tail;
	list_remove(&handle->chunks, (Node *)chunk);

	if(chunk->size % 2) iff_correctchunk(handle, chunk);

	fseek(handle->iff, chunk->pos + 4, SEEK_SET);
	size[0] = _SwapBE32(chunk->size);
	fwrite(&size, 4, 1, handle->iff);
	fseek(handle->iff, chunk->pos + chunk->size + 8, SEEK_SET);
	handle->pos = ftell(handle->iff);
	free(chunk);
}
