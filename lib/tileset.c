/**
@name tileset
 Create a tileset from a tileset definition and images.

 Usage:

 <em>tileset</em> name.t

 The definition file can have any name, but .t is the
 postfix I'll be using. A tileset definition refers
 to one or more images (loadable by SDL_image) to cut
 sections from. The resulting tileset contains graphics
 with a masking colour defined (the colour that doesn't
 get copied in blits). It does not define WHAT each
 tile is for (sprites, terrain, icons).


 <h2>Definition file format</h2>
 The file is divided into sections. Descriptions follow here.

 <b>[tileset]</b>: The first section, normally. It can be anywhere
 in the file, but it must be unique.

 <h3>Variables in the [tileset] section</h3>

 Variables are set in the usual way of INI-style files,
 with "name = definition" at most once per line.

 <ul>
 <li>name</li> A string; the name to refer to this set by.

 <li>filename</li> This is a string containing the name you
 want to use for the output tileset. Usually ends in .iff or .tiles
 or some other vaguely descriptive surname.

 <li>size</li> Size in pixels of each tile in this tileset.
 A tileset must have all its tiles the same size. Format: Either
 width followed by a space, then height, or WIDTHxHEIGHT.
 <br>For example:<br>size = 32x32

 <li>maskcol</li> The transparent colour. The format is hexadecimal:
 <br>maskcol = 0xnnnnnn
 <br> The 0x prefix is optional.

 <li>author</li> You may specify the name of the artist/packager
 with this variable. Not required, but the artists will thank you
 for it if they're any good, and hate your guts otherwise.

 <li>comment</li> Another optional variable. Any string can go here.
</ul>

 The next type of section can appear multiple times. It specifies
 files to grab images from. The section name is <b>[source]</b>.

 <h3>Variables in [source] sections</h3>

 <ul>
 <li>image</li> Name of an image file in any format known to
 SDL_image. Suggested formats: XPM, PNG or TGA.
 Non-lossy compression is preferred, as JPEG and similar will
 probably mess up the pixels  in transparent colours.
 Proper transparency support from PNG is planned, though.

 <li>count</li> The number of tiles that the image holds.
 The program starts in the upper left corner, then moves right
 until reaching the rightmost edge, then proceeds to the next
 line of tiles until <b>count</b> tiles have been grabbed.
 This variable can be left out if there is only one tile in
 the source image.
 </ul>

 <h3>Variables usable in all sections</h3>
 <ul>
 <li>xborder, yborder</li> These variables make it possible to set
 the width/height of any borders between blocks in the source image.
 Borders to the right and below are accounted for with this. First
 tile will be grabbed from position 0,0 in the image.

 <li>xoffset, yoffset</li> Like above, except the border is
 considered left/above each tile. With these variables, and some
 experimentation, you can grab tilesets off the net easily.
 </ul>

 <h3>IFF chunk types</h3>
 Here is a list of the different chunks that a tileset contains.
 The FORM type is <b>TILS</b>.

 <ul>
 <li>NAME</li> A name to use when referring to this tileset.
 <li>AUTH</li> Name of the author.
 <li>ANNO</li> File comment.
 <li>Mask</li> The mask colour (24-bit RGB colour, big-endian).
 <li>Size</li> Two Uint16 integers with width and height of the
 tiles in the tileset. Big-endian format.
 <li>Tile</li>A zlib-compressed tile in 24-bit RGB mode.
 The first ulong (big-endian) in the data is the uncompressed size.
 </ul>

 <b>Note:</b> If image names are not specified by full paths,
 the program will attempt to load them relative to the definition
 file. In other words, the working directory is set to
 wherever the definitions are.
@author Ronny Bangsund
*/

/**
*/

#include <SDL/SDL_image.h>
#include <zlib.h>

#include "limits.h"
#include "iffparse.h"
#include "prefs.h"
#include "xios.h"
#include "xistr.h"

#include "config.h"


int main(int argc, char *argv[])
{
#if !defined(PATH_MAX)
#	define PATH_MAX 4096
#endif
#if defined(WIN32)
	char	drive[PATH_MAX], dir[PATH_MAX];
	char	fname[PATH_MAX], ext[PATH_MAX];
#endif
	char	path[PATH_MAX];
	List *def;
	preferences	*section;
	variable	*var;
	char	*filename=0, *name=0, *author=0, *comment=0;
	char	*tilebuf, *tilebuf2;
	strtoken	*token;
	Uint16	size[2], tw, th;
	Uint32	maskcol;
	ulong	count, destlen, realsize;
	IFFHandle *iff=0;
	SDL_Surface	*image, *tile;
	SDL_Rect	src, dst;
	int	line, pos, xborder=0, yborder=0, xoffset=0, yoffset=0;

	printf("tileset creator (Xiqual %s)\n", VERSION);

	if(argc < 2) {
		fprintf(stderr, "Usage:\n%s <tileset>\n", argv[0]);
		return 0;
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to initialise SDL!\n");
		return 2;
	}
	atexit(SDL_Quit);

	def = cfg_loadprefs(PREFS_FILENAME, argv[1],
		PREFS_STRINGS, 1, TAG_END);

	if(!def) {
		fprintf(stderr, "Can't load tileset definition '%s'\n", argv[1]);
		return 1;
	}

	/* Change path to where the definition file is */
#if defined(WIN32)
	/* No realpath() on Win32 */
	_splitpath(argv[1], drive, dir, fname, ext);
	_makepath(path, drive,  dir, NULL, NULL);
#else
	realpath(argv[1], path);
	filename = strrchr(path, '/');
	if(filename) {
		filename[0] = 0;
	}
#endif
	chdir(path);

	printf("Loaded tileset definition \"%s\"\n", argv[1]);
	section = cfg_findsection(def, "tileset");
	if(!section) {
		fprintf(stderr, "[tileset] section missing!\n");
		return 1;
	}
	var = (variable *)node_findbyname(section->variables.head, "filename");
	if(var) filename = var->string;

	var = (variable *)node_findbyname(section->variables.head, "name");
	if(var) name = var->string;

	var = (variable *)node_findbyname(section->variables.head, "comment");
	if(var) comment = var->string;

	var = (variable *)node_findbyname(section->variables.head, "size");
	if(var) {
		token = str_tokenise(var->string, " xX");
		if(token) {
			if(token->tokens.head) size[0] = atoi(token->tokens.head->data);
			if(token->tokens.head->next)
				size[1] = atoi(token->tokens.head->next->data);
			str_freetoken(token);
			printf("Size is %d by %d\n", size[0], size[1]);
		}
	}

	var = (variable *)node_findbyname(section->variables.head, "maskcol");
	if(var) maskcol = strtol(var->string, NULL, 16);

	var = (variable *)node_findbyname(section->variables.head, "author");
	if(var) author = var->string;

	var = (variable *)node_findbyname(section->variables.head, "xborder");
	if(var) xborder = atoi(var->string);

	var = (variable *)node_findbyname(section->variables.head, "yborder");
	if(var) yborder = atoi(var->string);

	var = (variable *)node_findbyname(section->variables.head, "xoffset");
	if(var) xoffset = atoi(var->string);

	var = (variable *)node_findbyname(section->variables.head, "yoffset");
	if(var) yoffset = atoi(var->string);

	iff = iff_new(filename, MAKE_ID('T','I','L','S'));
	if(!iff) {
		list_free(def);
		fprintf(stderr, "Can't create the IFF %s\n", filename);
		return 2;
	}

	printf("Creating tileset with filename %s\n", filename);

	if(name) {
		printf("Internal name is %s\n", name);
		iff_newchunk(iff, MAKE_ID('N','A','M','E'));
		iff_writechunkdata(iff, name, strlen(name)+1);
		iff_endchunk(iff);
	}

	if(comment) {
		printf("Comment:%s\n", comment);
		iff_newchunk(iff, MAKE_ID('A','N','N','O'));
		iff_writechunkdata(iff, comment, strlen(comment)+1);
		iff_endchunk(iff);
	}

	if(author) {
		printf("Author(s):%s\n", author);
		iff_newchunk(iff, MAKE_ID('A','U','T','H'));
		iff_writechunkdata(iff, author, strlen(author)+1);
		iff_endchunk(iff);
	}

	tw = size[0]; th = size[1];
	size[0] = _SwapBE16(size[0]);
	size[1] = _SwapBE16(size[1]);
	iff_newchunk(iff, MAKE_ID('S','i','z','e'));
	iff_writechunkdata(iff, (char *)&size, 4);
	iff_endchunk(iff);

	if(maskcol) {
		maskcol = _SwapBE32(maskcol);
		iff_newchunk(iff, MAKE_ID('M','a','s','k'));
		iff_writechunkdata(iff, (char *)&maskcol, 4);
		iff_endchunk(iff);
	}

	tile = SDL_SetVideoMode(tw, th, 24, 0);
	tilebuf = calloc(tw, th * 3);
	tilebuf2 = calloc(tw, th * 3 + (tw * th));
	if((!tilebuf) || (!tilebuf2) || (!tile)) {
		iff_close(iff);
		list_free(def);
		if(tilebuf) free(tilebuf);
		if(tilebuf2) free(tilebuf2);
		fprintf(stderr, "Failed to create graphical workbuffer!\n");
		return 2;
	}
	dst.x = 0; dst.y = 0;
	src.w = tw; src.h = tw;
	section = cfg_findnext((preferences *)def->head, "source");
	while(section) {
		var = (variable *)node_findbyname(section->variables.head, "xborder");
		if(var) xborder = atoi(var->string);

		var = (variable *)node_findbyname(section->variables.head, "yborder");
		if(var) yborder = atoi(var->string);

		var = (variable *)node_findbyname(section->variables.head, "xoffset");
		if(var) xoffset = atoi(var->string);

		var = (variable *)node_findbyname(section->variables.head, "yoffset");
		if(var) yoffset = atoi(var->string);

		var = (variable *)node_findbyname(section->variables.head, "count");
		if(var) {
			count = atol(var->string);
		} else {
			count = 1;
		}
		src.x = src.y = 0;
		var = (variable *)node_findbyname(section->variables.head, "image");
		if(var) {
			image = IMG_Load(var->string);
			if(image) {
				printf("Loaded image %s (%dx%d)", var->string,
					image->w, image->h);
				if(count > 1) {
					printf(" with %ld images\n", count);
				} else {
					printf(" with only one tile\n");
				}
			} else {
				printf("Failed to load %s\n", var->string);
				count = 0;
			}
			while(count) {
				src.x += xoffset;
				src.y += yoffset;
				SDL_BlitSurface(image, &src, tile, &dst);
				SDL_Flip(tile);
				pos = 0;
				/* Copy scanline by scanline to temporary buffer */
				for(line=0; line<th; line++) {
					memcpy(tilebuf + pos,
						(char *)tile->pixels + (tile->pitch * line),
						3 * tw);
					pos += 3 * tw;
				}
				count--;
				destlen = tw * th * 3;
				compress2(tilebuf2, &destlen, tilebuf, tw * th * 3, 9);
				iff_newchunk(iff, MAKE_ID('T','i','l','e'));
				realsize = _SwapBE32(tw * th *3);
				iff_writechunkdata(iff, (char *)&realsize, 4);
				iff_writechunkdata(iff, tilebuf2, destlen);
				iff_endchunk(iff);
				src.x += tw + xborder;
				if(src.x >= image->w) {
					src.x = 0;
					src.y += th + yborder;
				}
				if(src.y >= image->h) count = 0;
			}
			if(image) SDL_FreeSurface(image);
		}
		section = section->next;
		xborder = yborder = xoffset = yoffset = 0;
	}

	free(tilebuf);
	free(tilebuf2);
	iff_close(iff);
	list_free(def);
	return 0;
}
