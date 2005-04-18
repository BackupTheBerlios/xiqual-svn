#include "iffparse.h"


/**
 Correct and pad odd-sized IFF chunks.
@doc
 Used internally when a chunk is ended, or a new
 chunk is started.
@author Ronny Bangsund
@see iff_newchunk(), iff_endchunk()
*/
void iff_correctchunk(IFFHandle *handle, IFFChunk *chunk)
{
	char	null[1];

	if(!chunk) return;

	chunk->size++;
	handle->size++;
	fseek(handle->iff, chunk->pos + chunk->size + 7, SEEK_SET);
	null[0] = 0;
	fwrite(&null, 1, 1, handle->iff);
	handle->pos = ftell(handle->iff);
}
