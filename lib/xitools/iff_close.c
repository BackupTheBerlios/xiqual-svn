/*
 * Close opened IFF
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_close.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_close.c,v $
 * Revision 1.2  2003/05/30 08:54:58  nny
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
#include "stdlib.h"

#include "iffparse.h"
#include "xiendian.h"


/**
 Closes an IFF.
@doc
 If it was opened with iff_new(), the final filesize
 is written to its header.
@author Ronny Bangsund
@see iff_open(), iff_new()
*/
void iff_close(IFFHandle *handle)
{
	IFFChunk	*chunk, *next;
	ulong	size[1];

	if(!handle) return;

	while(handle->chunks.head) {
		chunk = (IFFChunk *)handle->chunks.head;
		list_remove(&handle->chunks, (Node *)chunk);
		if(handle->chunks.head) {
			next = (IFFChunk *)handle->chunks.head;
			chunk->size = next->pos - chunk->pos - 8;
		} else {
			chunk->size = handle->size - chunk->pos - 8;
		}
		size[0] = _SwapBE32(chunk->size);
		fseek(handle->iff, chunk->pos+4, SEEK_SET);
		fwrite(&size, 4, 1, handle->iff);
	}
	if(handle->write) iff_fixsize(handle);
	if(handle->iff) fclose(handle->iff);
	free(handle);
}
