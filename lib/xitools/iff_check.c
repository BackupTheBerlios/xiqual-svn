/*
 * Is this a true EA IFF 85 compliant iff?
 * Boolean return value
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_check.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_check.c,v $
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

int iff_check(IFFHandle *handle)
{
	ulong	form[3];
	size_t	size;

	/* I think I'm paranoid */
	if(!handle)
		return FALSE;

	fseek(handle->iff, 0, SEEK_END);
	size = ftell(handle->iff);
	fseek(handle->iff, 0, SEEK_SET);

	/* We'll acknowledge the existence of these three IFFs only. */
	fread(&form, 4, 3, handle->iff);
	form[0] = _SwapBE32(form[0]);
	form[1] = _SwapBE32(form[1]);
	form[2] = _SwapBE32(form[2]);
	switch(form[0]) {
	case ID_FORM:
	case ID_LIST:
	case ID_CAT:
		break;
	default:
		return FALSE;
	}

	/* The only good size is filesize minus headersize */
	if((size-8) != form[1])
		return FALSE;

	handle->type = form[2]; /* Set the filetype in the header */
	return TRUE;
}
