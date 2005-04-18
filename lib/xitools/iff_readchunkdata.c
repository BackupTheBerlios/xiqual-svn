/*
 * Read the actual chunk data into the buffer
 * Possible return values:
 * Number of bytes read, or zero for nothing.
 * It's up to the user to keep reading if the buffer is smaller than the chunk.
 * If return value and handle->id is zero, an error has occured.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_readchunkdata.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_readchunkdata.c,v $
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

size_t iff_readchunkdata(IFFHandle *handle, char *buffer, size_t size)
{
	size_t	read;

	if(!(handle && buffer && size))
		return 0;

	if(size >= handle->chunksize) {
		read = handle->chunksize;
	} else {
		read = size;
	}

	read = fread(buffer, 1, read, handle->iff);
	handle->pos += read;
	return read;
}
