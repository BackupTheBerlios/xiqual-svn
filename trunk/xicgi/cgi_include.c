/*
 * Load and print a section of text
 * Use this for default headers/footers
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: cgi_include.c,v 1.3 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: cgi_include.c,v $
 * Revision 1.3  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.2  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.2  2001/11/15 22:02:43  nny
 * cgi_include.c:Fixed printf where there was a lacking NULL terminator.
 *
 * Revision 1.1  2001/11/14 14:04:09  nny
 * Extracted file_getsize.c from files.c
 * Added cgi_include.c to make it easy to include bits of HTML.
 *
 */

#include "stdio.h"
#include "stdlib.h"

#include "files.h"

int cgi_include(char *filename)
{
	size_t	s;
	char	*mem;
	FILE	*f;

	s = file_getsize(filename);
	if(!s) return 0;

	mem = calloc(1, s+1);
	if(!mem) return 0;

	f = fopen(filename, "r");
	if(!f) return 0;

	s = fread(mem, 1, s, f);
	printf("%s", mem);
	fclose(f);
	free(mem);

	return s;
}
