/*
 * Check if a chunk ID complies.
 * Used when creating a new IFF.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_goodid.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_goodid.c,v $
 * Revision 1.2  2003/05/30 08:54:58  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.4  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"

#include "iffparse.h"
#include "xiendian.h"

int iff_goodID(ulong id)
{
	char	*c;
	int	i;

	c = (char *)&id;
	if((c[0] == ' ') && (id != ID_NULL))
		return FALSE; /* Can't start with space */

	for(i=0;i<4;i++) {
		if((c[i] < 0x20) || (c[i] > 0x7e))	/* The only allowable range of characters */
			return FALSE;
	}
	/* That's it! Pretty loose restrictions, really */
	return TRUE;
}
