/*
 * Test the iff writer
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:26 $
 * $Id: iffwrite.c,v 1.8 2003/05/30 08:54:26 nny Exp $
 *
 * $Log: iffwrite.c,v $
 * Revision 1.8  2003/05/30 08:54:26  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.5  2001/11/21 20:25:35  nny
 * Big cleanup; only one single warning left on my system.
 *
 * Revision 1.4  2001/11/13 19:26:32  nny
 * Removed tag_inarray.c, as it was just a waste of space.
 * Added CVS variables to most files.
 *
 */

#include "stdio.h"

#include "iffparse.h"

int main()
{
	IFFHandle *iff;

	iff = iff_new("test.iff", MAKE_ID('X','I','Q','U'));
	if(!iff) {
		printf("Can't create IFF file.\n");
		return 0;
	}

	printf("Created file test.iff\n");

	iff_newchunk(iff, MAKE_ID('N','A','M','E'));
	if(iff_writechunkdata(iff, "test.iff", 9))
		printf("Wrote name chunk.\n");
	iff_endchunk(iff);

	iff_newchunk(iff, MAKE_ID('A','U','T','H'));
	if(iff_writechunkdata(iff, "O- RB", 6))
		printf("Wrote author chunk.\n");
	iff_endchunk(iff);

	iff_newchunk(iff, MAKE_ID('A','N','N','O'));
	if(iff_writechunkdata(iff, "Testing libxitools", 19))
		printf("Wrote annotation chunk.\n");
	iff_endchunk(iff);

	printf("Wrote %ld bytes to test.iff\n", iff->size);
	iff_close(iff);
	return 0;
}
