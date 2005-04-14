/*
 * Write the FORM size at IFF close
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_fixsize.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_fixsize.c,v $
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

void iff_fixsize(IFFHandle *handle)
{
	ulong size[1];

	if(!handle)
		return;

	size[0] = _SwapBE32(handle->size);
	fseek(handle->iff, 4, SEEK_SET);
	fwrite(&size, 4, 1, handle->iff);
	handle->pos = 8;
}
