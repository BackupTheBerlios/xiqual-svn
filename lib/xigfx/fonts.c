/*
 * Read IFF font (Xiqual-specific format)
 * Requires SDL, SDL_image & zlib.
 * This is the second version of this loader;
 * the API should still be compatible.
 *
 */
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <zlib.h>

#include "iffparse.h"
#include "fonts.h"
#include "xios.h"

static void font_newsurface(xifont *font, int c, char *pixels);

/**
 Load a Xiqual free-form IFF font.
@doc
 By freeform, I mean that you are not required
 to create symbols for more characters than you
 can be bothered to draw. If you want a fontset
 with only the letters required to print your
 name, go ahead and make it.

 The utility named <em>tilefont</em> is used to create
 these fonts from one or more images.
@param fontname An IFF file of type "XIFN", containing
 one or more glyphs falling within the constraints of
 the ASCII table. Sure, you could have symbols for
 unprintable characters.
@author Ronny Bangsund
@see xi_puttext(), xi_freefont(), tilefont, xifont
*/
xifont *xi_loadfont(char *fontname)
{
	IFFHandle	*iff;
	xifont	*font;
	ulong	c=0, *compbuf=0, srclen, destlen;
	char	*decompbuf=0;
	Uint16	size[2], spacing;
	SDL_Surface	*image=0;
	SDL_Rect	dst;

	iff = iff_open(fontname);
	if(!iff) return 0;

	if(!(iff->type == MAKE_ID('X','I','F','N'))) return 0;

	font = calloc(1, sizeof(xifont));
	if(!font) {
		iff_close(iff);
		return 0;
	}

	memset(font, 0, sizeof(xifont));
	dst.x = dst.y = 0;
	while(iff_readchunkheader(iff)) {
		switch(iff->id) {
		case MAKE_ID('N','A','M','E'):
			font->name = calloc(1, iff->chunksize);
			if(font->name)
				iff_readchunkdata(iff, font->name, iff->chunksize);
			else
				iff_skipchunkdata(iff);
			break;
		case MAKE_ID('A','U','T','H'):
			font->author = calloc(1, iff->chunksize);
			if(font->author)
				iff_readchunkdata(iff, font->author, iff->chunksize);
			else
				iff_skipchunkdata(iff);
			break;
		case MAKE_ID('S','i','z','e'):
			iff_readchunkdata(iff, (char *)&size, 4);
			font->w = _SwapBE16(size[0]);
			font->h = _SwapBE16(size[1]);
			break;
		case MAKE_ID('S','p','c','e'):
			iff_readchunkdata(iff, (char *)&spacing, 2);
			font->spacing = _SwapBE16(spacing);
			break;
		case MAKE_ID('M','a','s','k'):
			iff_readchunkdata(iff, (char *)&font->maskcol, 4);
			font->maskcol = _SwapBE32(font->maskcol);
			break;
		case MAKE_ID('C','h','a','r'):
			iff_readchunkdata(iff, (char *)&c, 2);
			c = _SwapBE16(c);
			break;
		case MAKE_ID('G','l','y','f'):
			/* Glyf chunks must be directly preceded by Char chunks */
			if((!c) || (c > 255)) {
				iff_skipchunkdata(iff);
				break;
			}
			if(!compbuf) {
				compbuf = calloc(font->w, font->h * 3);
				if(compbuf) memset(compbuf, 0, font->w * font->h * 3);
			}
			if(!decompbuf) {
				decompbuf = calloc(font->w, font->h * 3);
				if(decompbuf) memset(decompbuf, 0, font->w * font->h * 3);
			}
			if(compbuf && decompbuf) {
				iff_readchunkdata(iff, (char *)compbuf, iff->chunksize);
				srclen = iff->chunksize -4;
				destlen = _SwapBE32(compbuf[0]);
				if(uncompress((char *)decompbuf, &destlen,
					(char *)compbuf + 4, srclen) == Z_OK) {
					font_newsurface(font, c, decompbuf);
				}
			}
			break;
		default:
			iff_skipchunkdata(iff);
		}
	}
	if(compbuf) free(compbuf);
	if(decompbuf) free(decompbuf);
	SDL_FreeSurface(image);
	iff_close(iff);
	font->dst.w = font->w;
	font->dst.h = font->h;
	return font;
}

static void font_newsurface(xifont *font, int c, char *pixels)
{
	SDL_Surface	*tmp, *glyph;
	Uint32	rmask, gmask, bmask, amask;
	int	line, pos;

	/* I *like* big-endian formats */
	rmask = 0xff0000;
	gmask = 0x00ff00;
	bmask = 0x0000ff;
	/* We're using colour-keyed blits only (for now) */
	amask=0;
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY,
		font->w, font->h, 24, rmask, gmask, bmask, amask);
	if(!tmp) return;

	pos = 0;
	for(line=0; line<font->h; line++) {
		memcpy((char *)tmp->pixels + (tmp->pitch * line),
			pixels + pos, 3 * font->w);
		pos += 3 * font->w;
	}

	SDL_SetColorKey(tmp, SDL_RLEACCEL|SDL_SRCCOLORKEY, font->maskcol);
	glyph = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	if(!glyph) return;

	font->glyph[c] = glyph;
}


/**
 Free a font when done with it.
@doc
 Deallocates all resources, freeing up to
 256 SDL surfaces for glyphs, and any of
 the optional strings. Well, what did you
 <em>expect</em> it to do?
@param font xifont to be freed.
@author Ronny Bangsund
@see xi_loadfont(), xifont
*/
void xi_freefont(xifont *font)
{
	int	i;

	if(!font) return;

	if(font->name) free(font->name);
	if(font->author) free(font->author);

	for(i=0;i<256;i++) {
		if(font->glyph[i])
		SDL_FreeSurface((SDL_Surface *)font->glyph[i]);
	}
	free(font);
}


/**
 Print text using an xifont.
@doc
 Given an xifont, this function will print text going
 from left to right on the desired SDL surface,
 starting at the specified position.

 This is much faster than using SDL_ttf, but lacks
 much flexibility. It is more suited for on-the-fly
 printing to the screen, though.
@author Ronny Bangsund
@see xi_loadfont(), xifont
*/
void xi_puttext(xifont *font, SDL_Surface *scr, int x, int y, char *text)
{
	int	i, max;

	if((!font) || (!scr) || (!text)) return;

	max = strlen(text);
	font->dst.x = x;
	font->dst.y = y;
	for(i=0;i<max;i++) {
		if(font->glyph[(ulong)text[i]]) {
			SDL_BlitSurface(font->glyph[(Uint32)text[i]],
				NULL, scr, &font->dst);
		}
		font->dst.x += font->w + font->spacing;
	}
}
