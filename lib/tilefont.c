/**@name tilefont
 Create a font from a definiion file and images.

 Usage:

 <em>tilefont</em> name.def

 Any name is acceptable for the definition file.
 The definition file consists of two types of
 sections, described below.

 <h2>Definition file format</h2>
 Sections are names in square brackets on their own lines.
 The [font] section must appear once, no more or less,
 while the [source] section can appear once or more.

 <b>{font]</b>: The unique section that defines fontsize,
 name of output file and author/comment strings.

 <h3>Variables in section [font]</h3>
 <ul>
 <li>name</li>
 Unique name to use for this font. This is used while
 search a font list, for instance.

 <li>filename</li>
 This is the name of the created font IFF.
 No special name is required.

 <li>size</li>
 Size in pixels of each glyph (letter/symbol) in the font.
 This is in WxH format, like tilesets.

 <li>spacing</li>
 The number of blank pixels to the right of each font.
 If a font is drawn in a very blocky mode, you might want
 to use this to make it slightly more readable.

 <li>maskcol</li> The transparent colour. Hexadecimal format:
 <br>maskcol = 0xnnnnnn
 <br> The 0x prefix is optional.

 <li>author</li>
 Put the name of the culprit here.

 <li>comment</li>
 Other comments go here. Only one comment section will be made,
 from the first comment= definition found.
 </ul>

 <h3>Variables in [source] sections</h3>
 <ul>
 <li>image</li> Name of an image file in any format known to
 SDL_image. Suggested formats: XPM, PNG or TGA.
 Non-lossy compression is preferred, as JPEG and similar will
 probably mess up the pixels  in transparent colours.

 <li>glyphs</li>
 This is a string of symbols in the exact order they appear
 in the image file. It starts in the upper left corner of
 the specified image. The images can stretch over several
 lines in the image, but there should be glyphs filling the
 width.
 </ul>

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
	List	*def;
	preferences	*section;
	variable	*var;
	char		*filename=0, *name=0, *author=0, *comment=0;
	char		*fontbuf, *fontbuf2, *ch=0;
	strtoken	*token;
	Uint16		size[2], w, h, charbuf[1], spacing=0;
	Uint32		maskcol;
	ulong		destlen, realsize;
	IFFHandle	*iff=0;
	SDL_Surface	*image, *glyph;
	SDL_Rect	src, dst;
	int		line, pos;

	printf("tilefont creator (Xiqual %s)\n", VERSION);
	if(argc < 2) {
		fprintf(stderr, "Usage:\n%s <definition>\n", argv[0]);
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
		fprintf(stderr, "Can't load font definition '%s'\n", argv[1]);
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
	section = cfg_findsection(def, "font");
	if(!section) {
		fprintf(stderr, "[font] section missing!\n");
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

	var = (variable *)node_findbyname(section->variables.head, "spacing");
	if(var) spacing = atoi(var->string);

	var = (variable *)node_findbyname(section->variables.head, "author");
	if(var) author = var->string;

	iff = iff_new(filename, MAKE_ID('X','I','F','N'));
	if(!iff) {
		list_free(def);
		fprintf(stderr, "Can't create the IFF %s\n", filename);
		return 2;
	}

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

	w = size[0]; h = size[1];
	size[0] = _SwapBE16(size[0]);
	size[1] = _SwapBE16(size[1]);
	iff_newchunk(iff, MAKE_ID('S','i','z','e'));
	iff_writechunkdata(iff, (char *)&size, 4);
	iff_endchunk(iff);

	if(spacing) {
		printf("Spacing: %d\n", spacing);
		iff_newchunk(iff, MAKE_ID('S','p','c','e'));
		spacing = _SwapBE16(spacing);
		iff_writechunkdata(iff, (char *)&spacing, 2);
		iff_endchunk(iff);
	} else {
		printf("No spacing.\n");
	}
	if(maskcol) {
		maskcol = _SwapBE32(maskcol);
		iff_newchunk(iff, MAKE_ID('M','a','s','k'));
		iff_writechunkdata(iff, (char *)&maskcol, 4);
		iff_endchunk(iff);
	}

	glyph = SDL_SetVideoMode(w, h, 24, 0);
	fontbuf = calloc(w, h * 3);
	fontbuf2 = calloc(w, h * 3 + (w * h));
	if((!fontbuf) || (!fontbuf2) || (!glyph)) {
		iff_close(iff);
		list_free(def);
		if(fontbuf) free(fontbuf);
		if(fontbuf2) free(fontbuf2);
		fprintf(stderr, "Failed to create graphical workbuffer!\n");
		return 2;
	}
	dst.x = 0; dst.y = 0;
	src.w = w; src.h = h;
	section = cfg_findnext((preferences *)def->head, "source");
	while(section) {
		var = (variable *)node_findbyname(section->variables.head, "glyphs");
		if(var) ch = var->string;
		var = (variable *)node_findbyname(section->variables.head, "image");
		src.x = src.y = 0;
		if(var) {
			image = IMG_Load(var->string);
			if(image) {
				printf("Loaded image %s (%dx%d)\n", var->string,
					image->w, image->h);
			} else {
				printf("Failed to load %s\n", var->string);
				ch = 0;
			}
			while((ch) && (ch[0])) {
				SDL_BlitSurface(image, &src, glyph, &dst);
				SDL_Flip(glyph);
				pos = 0;
				/* Copy scanline by scanline to temporary buffer */
				for(line=0; line<h; line++) {
					memcpy(fontbuf + pos,
						(char *)glyph->pixels + (glyph->pitch * line),
						3 * w);
					pos += 3 * w;
				}
				destlen = w * h * 3;
				compress2(fontbuf2, &destlen, fontbuf, w * h * 3, 9);
				iff_newchunk(iff, MAKE_ID('C','h','a','r'));
				charbuf[0] = _SwapBE16(ch[0]);
				iff_writechunkdata(iff, (char *)&charbuf, 2);
				iff_endchunk(iff);
				iff_newchunk(iff, MAKE_ID('G','l','y','f'));
				realsize = _SwapBE32(w * h *3);
				iff_writechunkdata(iff, (char *)&realsize, 4);
				iff_writechunkdata(iff, fontbuf2, destlen);
				iff_endchunk(iff);
				src.x += w;
				ch++;
				if(src.x >= image->w) {
					src.x = 0;
					src.y += h;
				}
				if(src.y >= image->h) ch = 0;
			}
			if(image) SDL_FreeSurface(image);
		}
		section = section->next;
	}
	iff_close(iff);
	list_free(def);
	return 0;
}
