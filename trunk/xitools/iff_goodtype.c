/*
 * Check if a FORM type complies - stricter than chunk IDs.
 * See the EA IFF 85 specification.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: iff_goodtype.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: iff_goodtype.c,v $
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

#include "ctype.h"
#include "stdio.h"

#include "iffparse.h"
#include "xiendian.h"

int iff_goodtype(ulong type)
{
	char	*c;
	int	i;

	/* Types have all the restrictions of IDs and then some! */
	if(!iff_goodID(type))
		return FALSE;

	c = (char *)&type;
	for(i=0;i<4;i++) {
		if(isalpha(c[0]) && (islower(c[0])))
			return FALSE;

		if(c[i] > 'Z')
			return FALSE;

		if( (c[i] <'A') && ((c[i] > '9') || (c[i] < '0')) && (c[0] != ' '))
			return FALSE;
	}
	/* Phew! If you get here, your FORM type is hoopy */
	return TRUE;
}
