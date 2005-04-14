/*
 * Open an existing IFF
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: iff_open.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: iff_open.c,v $
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

IFFHandle *iff_open(char *name)
{
	IFFHandle *handle;

	if(!name)
		return 0;

	handle = (IFFHandle *)calloc(1, sizeof(IFFHandle));
	if(!handle)
		return 0;

	memset(handle, 0, sizeof(IFFHandle));

	handle->iff = fopen(name, "rb");
	if(!handle->iff) {
		free(handle);
		return 0;
	}

	if(!iff_check(handle))
		return 0;

	return handle;
}
