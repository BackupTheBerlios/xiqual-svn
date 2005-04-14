/*
 * Start writing a new chunk
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: iff_newchunk.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: iff_newchunk.c,v $
 * Revision 1.4  2003/12/11 08:54:55  nny
 *
 * Modified Files:
 * All.
 *
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

#include "iffparse.h"
#include "xiendian.h"


/**
 Start on a new chunk in an IFF.
@doc
 You should finish writing any other chunks first with
 iff_endchunk() before calling this. The supplied chunk
 identifier will be written in big-endian format.
@author Ronny Bangsund
@see iff_new(), iff_writechunkdata()
*/
int iff_newchunk(IFFHandle *handle, ulong id)
{
	IFFChunk	*chunk;
	ulong	header[2];

	if((!handle) || (!id)) return FALSE;

	if(!iff_goodID(id)) return FALSE;

	chunk = calloc(1, sizeof(IFFChunk));
	if(!chunk) return FALSE;

	chunk->id = id;
	chunk->size = 0;
	if(chunk->pos % 2) iff_correctchunk(handle, chunk->prev);
	chunk->pos = handle->pos;
	list_add(&handle->chunks, (Node *)chunk);

	header[0] = _SwapBE32(id);
	header[1] = 0;
	handle->size += 8;
	handle->pos += 8;
	fwrite(&header, 4, 2, handle->iff);
	return TRUE;
}
