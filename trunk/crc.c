/**@name crc
 Simple CRC32-summing program.
@doc
 <h3>Usage</h3>
 <em>crc</em> file
*/

/* $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: crc.c,v 1.2 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: crc.c,v $
 * Revision 1.2  2003/12/11 08:54:55  nny
 *
 * Modified Files:
 * All.
 *
 *
*/

/**
*/

#include <stdio.h>
#include <stdlib.h>

#include "files.h"
#include "hash.h"

void my_hook(char *buf, size_t len);

#define BUFSIZE 50000

int main(int argc, char *argv[])
{
	autofile *af;
	ulong crc;

	if(argc == 1) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return 0;
	}

	af = file_new(argv[1]);
	if(!af) return 1;

	if(!af->size) return 1;

	if(!file_initbuf(af, BUFSIZE)) {
		fprintf(stderr, "Couldn't initialise a read buffer\n");
		return 2;
	}

	while(af->pos < af->size) {
		file_read(af, BUFSIZE);
	}
	crc = crc32_calc(af->buf, af->size);
	printf("CRC: %lx\n", crc);

	file_free(af);
	return 0;
}
