/*
 * Step back to the beginning of previous chunk.
 * Note: Only use this after a chunk has been read
 * or written fully.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_rewind.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_rewind.c,v $
 * Revision 1.2  2003/05/30 08:54:58  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.1  2001/11/16 21:22:48  nny
 * New function to skip to the beginning of the previous IFF chunk.
 *
 */

#include "stdio.h"

#include "iffparse.h"
#include "xiendian.h"

void iff_rewind(IFFHandle *handle)
{
	int	span;

	span = handle->chunksize + handle->prevchunksize;
	span = fseek(handle->iff, -span, SEEK_CUR);
	handle->pos = span;	/* Hopefully this works.. */
}
