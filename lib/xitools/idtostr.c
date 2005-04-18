/*
 * Convert an IFF ID to a string
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: idtostr.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: idtostr.c,v $
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

#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"

#include "xiendian.h"

char *IDtoStr(ulong id, char *buf)
{
	char	*c;
	int	newid;

	newid = _SwapBE32(id);
	c = (char *)&newid;
	sprintf(buf, "%1c%1c%1c%1c", c[0], c[1], c[2], c[3]);
	return buf;
}
