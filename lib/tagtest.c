/*
 * Tagtest
 * Demonstrates use of tags and varargs
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:36 $
 * $Id: tagtest.c,v 1.7 2003/12/11 08:36:36 nny Exp $
 *
 * $Log: tagtest.c,v $
 * Revision 1.7  2003/12/11 08:36:36  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.6  2003/05/30 08:54:27  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.3  2001/12/07 14:16:01  nny
 * Forgot to make a long-standing bugfix (use tag_next() always).
 *
 * Revision 1.2  2001/11/13 19:26:32  nny
 * Removed tag_inarray.c, as it was just a waste of space.
 * Added CVS variables to most files.
 *
 */

#include "stdarg.h"
#include "stdio.h"

#include "xiendian.h"
#include "xitags.h"

void ScanlistA(tagitem *tags)
{
	tagitem	*t, *saved;

	saved = tags;
	t = tag_next(&tags);	/* tag_next() modifies the pointer passed */
	while(t) {
		printf("Tag: %ld, data:%ld\n", t->tag, t->data);
		t = tag_next(&tags);
	}
	/* Test the tag_finditem() function */
	printf("Looking for 3000...\n");
	/* The functions modify the pointer they are passed! */
	t = saved;
	t = tag_finditem(3000, &t);
	if(t)printf("Found it! Data is %ld\n", t->data);
	/* Now check that the first item in the list can be found
	 * (sanity check)
	 */
	printf("Looking for 1000 (first in list)...\n");
	t = saved;
	t = tag_finditem(1000, &t);
	if(t)printf("Found it! Data is %ld\n", t->data);
}

void Scanlist(ulong tag1,  ... )
{
	XI_VOIDARG(ScanlistA((tagitem *)&tag1))
}

int main()
{
	Scanlist(1000, 1, 2000, 2, 3000, 3, 4000, 4, TAG_END);
	return 0;
}
