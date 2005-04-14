/*
 * Make an IFF fixed-size font definition from a plaintext font definition.
 *
 * Usage:
 * makefont fontdesc.def outfile.font
 *
 * The .fnt description file has this format:
 * Line 1: FONT
 * Line 2: Name of the font
 * Line 3: Image file (PNG, JPEG or other; preferably PNG)
 * Line 4: Author of the font
 * Line 5: <width>x<height> (each letter/symbol's size)
 * Each following line represents a line of letters in the file.
 * NOTE: Space is calculated from the size of letters.
 * This is for blocky graphic fonts, after all.
 *
 * $Author: nny $
 * $Date: 2002/10/04 12:44:40 $
 * $Id: makefont.c,v 1.7 2002/10/04 12:44:40 nny Exp $
 *
 * $Log: makefont.c,v $
 * Revision 1.7  2002/10/04 12:44:40  nny
 * *** empty log message ***
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
#include "stdlib.h"
#include "string.h"

#include "iffparse.h"
#include "files.h"

static autofile	*def;
static char	*name, *file, *author, *letters;
static int	width, height;
static IFFHandle	*font;

int parse_def();
void parse_letters();

int main(int argc, char *argv[])
{

	if(argc < 3) {
		fprintf(stderr, "Usage:\n%s fontdesc.def outfile.iff\n", argv[0]);
		return 0;
	}

	def = file_new(argv[1], 0);
	if(!def) {
		fprintf(stderr, "Can't find font definition '%s'\n", argv[1]);
		return 1;
	}
	file_load(def);
	if(!def->buf) {
		file_free(def);
		fprintf(stderr, "Not enough memory to load %s\n", argv[1]);
		return 2;
	}

	if(!parse_def()) {
		file_free(def);
		return 1;
	}

	font = iff_new(argv[2], MAKE_ID('F','O','N','T'));
	if(!font) {
		file_free(def);
	}

	iff_newchunk(font, MAKE_ID('N','A','M','E'));
	iff_writechunkdata(font, name, strlen(name)+1);
	iff_endchunk(font);

	iff_newchunk(font, MAKE_ID('F','I','L','E'));
	iff_writechunkdata(font, file, strlen(file)+1);
	iff_endchunk(font);

	iff_newchunk(font, MAKE_ID('A','U','T','H'));
	iff_writechunkdata(font, author, strlen(author)+1);
	iff_endchunk(font);

	parse_letters();

	iff_close(font);
	file_free(def);
	return 0;
}

int parse_def()
{
	char *c, *w, *h;
	size_t	pos;

	if(!def->size) {
		fprintf(stderr, "%s is empty.\n", def->name);
		return 0;
	}

	if(strncmp(def->buf, "FONT", 4)) {
		fprintf(stderr, "File %s is not a definition file.\n", def->name);
		return 0;
	}

	pos = 5;
	c = def->buf+pos;
	name = c;
	while((pos<=def->size) && (c[0] != 10)) {
		c++;
		pos++;
	}
	c[0] = 0;
	printf("Fontname is '%s'\n", name);
	c++;
	pos++;

	file = c;
	while((pos<=def->size) && (c[0] != 10)) {
		c++;
		pos++;
	}
	c[0] = 0;
	printf("Image file is '%s'\n", file);
	c++;
	pos++;

	author = c;
	while((pos<=def->size) && (c[0] != 10)) {
		c++;
		pos++;
	}
	c[0] = 0;
	printf("Author is '%s'\n", author);
	c++;
	pos++;

	w = c;
	while((pos<=def->size) && (c[0] != 'x')) {
		c++;
		pos++;
	}
	c[0] = 0;
	c++;
	pos++;
	width = strtol(w, NULL, 10);

	h = c;
	while((pos<=def->size) && (c[0] != 10)) {
		c++;
		pos++;
	}
	c[0] = 0;
	c++;
	pos++;
	height = strtol(h, NULL, 10);
	letters = c;

	printf("Fontsize is %dx%d\n", width, height);
	return 1;
}

void parse_letters()
{
	int	i, max, data;
	int	x=0, y=0;
	ulong	pos[2];

	iff_newchunk(font, MAKE_ID('S','i','z','e'));
	pos[0] = _SwapBE32(width);
	pos[1] = _SwapBE32(height);
	iff_writechunkdata(font, (char *)pos, 8);
	iff_endchunk(font);

	max = strlen(letters);
	for(i=0;i<max;i++) {
		switch(letters[i]) {
		case 0:
			printf("Done.\n");
			break;
		case 10:
			x = 0;
			y += height;
			break;
		default:
			iff_newchunk(font, MAKE_ID('C','H','A','R'));
			data = _SwapBE32(letters[i]);
			iff_writechunkdata(font, (char *)&data, 4);
			iff_endchunk(font);

			iff_newchunk(font, MAKE_ID('G','L','Y','F'));
			pos[0] = _SwapBE32(x);
			pos[1] = _SwapBE32(y);
			iff_writechunkdata(font, (char *)&pos, 8);
			iff_endchunk(font);
			x += width;
		}
	}
}
