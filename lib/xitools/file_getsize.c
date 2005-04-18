/* Semi-portable way of getting the size of a file.
 * The only completely independent routine.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: file_getsize.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: file_getsize.c,v $
 * Revision 1.2  2003/05/30 08:54:58  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.2  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include <stdio.h>

/**
 Stand-alone routine to get a file's size.
@doc
 If the file specified exists, its size is returned.
 Note that a file may be empty, in which case this
 fails miserably. This is fine for file_new() in
 Xiqual, however. Reading in an empty file is not
 necessary when you can just overwrite.
@author Ronny Bangsund
@see fopen(), fseek(), fclose(), file_new()
*/
size_t file_getsize(char *name)
{
	FILE *f;
	size_t s = 0;

	f = fopen(name, "rb");
	if (f) {
		fseek(f, 0, SEEK_END);
		s = ftell(f);
		fclose(f);
	}
	return s;
}
