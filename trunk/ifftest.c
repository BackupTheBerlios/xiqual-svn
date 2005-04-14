/*
 * Test the iff parser
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:26 $
 * $Id: ifftest.c,v 1.14 2003/05/30 08:54:26 nny Exp $
 *
 * $Log: ifftest.c,v $
 * Revision 1.14  2003/05/30 08:54:26  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.11  2001/11/21 20:25:35  nny
 * Big cleanup; only one single warning left on my system.
 *
 * Revision 1.10  2001/11/16 21:24:12  nny
 * Changes to accomodate the new iff_rewind() function.
 *
 * Revision 1.9  2001/11/13 19:26:32  nny
 * Removed tag_inarray.c, as it was just a waste of space.
 * Added CVS variables to most files.
 *
 */

#include "stdio.h"

#include "iffparse.h"

int main(int argc, char *argv[])
{
	IFFHandle *iff;
	char	buf[260];
	short	buf2[2];
	ulong size;

	if(argc<=1)
		return 0;

	iff = iff_open(argv[1]);
	if(!iff) {
		printf("Not a valid IFF file.\n");
		return 0;
	}

	printf("Information about %s of filetype %s:\n", argv[1], IDtoStr(iff->type, buf));
	while(iff_readchunkheader(iff)) {
		printf("Chunk ID:'%s', size:%ld\n", IDtoStr(iff->id, buf), iff->chunksize);
		switch(iff->id) {
		case MAKE_ID('N','A','M','E'):
			iff_readchunkdata(iff, buf, iff->chunksize);
			printf("Name:'%s'\n", buf);
			break;
		case MAKE_ID('A','U','T','H'):
			iff_readchunkdata(iff, buf, iff->chunksize);
			printf("Author: %s\n", buf);
			break;
		case MAKE_ID('A','N','N','O'):
			iff_readchunkdata(iff, buf, iff->chunksize);
			printf("Annotation: '%s'\n", buf);
			break;
		case MAKE_ID('F','I','L','E'):
			iff_readchunkdata(iff, buf, iff->chunksize);
			printf("Filename: '%s'\n", buf);
			break;
		case MAKE_ID('S','i','z','e'):
			iff_readchunkdata(iff, (char *)&buf2, 4);
			printf("Size: %dx%d\n", _SwapBE16(buf2[0]), _SwapBE16(buf2[1]));
			break;
		case MAKE_ID('T','i','l','e'):
			iff_readchunkdata(iff, (char *)&size, 4);
			printf("%d bytes of tile/sprite data\n", _SwapBE32(size));
			fseek(iff->iff, -4, SEEK_CUR);
			iff_skipchunkdata(iff);
			break;
		default:
			iff_skipchunkdata(iff);
			break;
		}
	}

	iff_close(iff);
	return 0;
}
