/*
 * Allocate a handle and open a file for writing
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: iff_new.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: iff_new.c,v $
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
#include "stdlib.h"

#include "iffparse.h"
#include "xiendian.h"


/**
 Creates a new IFF.
@doc
 Any existing file with the same name will be deleted.
 Note that header information is written in big-endian format.
 It's recommended to convert all numeric data to big-endian
 before saving if the data will be available on several
 different platforms. Intel x86 is little-endian, and PPC is
 big-endian.
@param name Name of file to create.
@param type IFF type.
@author Ronny Bangsund
@see iff_newchunk(), iff_writechunk(), iff_endchunk(),
 iff_close(), iff_goodtype(), IFFHandle, _SwapBE32()
*/
IFFHandle *iff_new(char *name, ulong type)
{
	IFFHandle	*handle;
	ulong	form[3];

	if((!name) || (!type)) return NULL;

	if(!iff_goodtype(type)) return NULL;

	handle = (IFFHandle *)calloc(1, sizeof(IFFHandle));
	if(!handle) return NULL;

	memset(handle, 0, sizeof(IFFHandle));
	handle->iff = fopen(name, "wb");
	if(!handle->iff) {
		free(handle);
		return NULL;
	}

	form[0] = _SwapBE32(ID_FORM);
	form[1] = 0;
	form[2] = _SwapBE32(type);
	if(!(fwrite(&form, 4, 3, handle->iff) == 3)) {
		fclose(handle->iff);
		free(handle);
		return NULL;
	}

	handle->write = TRUE;
	handle->size = 4;
	handle->pos = 12;
	return handle;
}
