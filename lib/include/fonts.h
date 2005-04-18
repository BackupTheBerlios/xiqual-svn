/*
 * Simple fixed-size fonts for SDL
 * Distinguishing feature: A whole alphabet isn't required.
 *
 */

#if !defined(_XIQUAL_FONTS)
#define _XIQUAL_FONTS

#include <SDL/SDL.h>

#include "xiendian.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 Xiqual font structure.
@doc
 Xiqual fonts are very simple fonts built from
 one or more equal-sized images. It's possible
 to have from one to 256 different symbols in
 a font, as it's based on the ASCII table.

 For more advanced options, wide character
 support and more than 256 symbols, use a
 Truetype font.
@author Ronny Bangsund
*/
typedef struct xifont {
/**
 Next font pointer for putting this in a List.
*/
	struct xifont	*next;
/**
 Previous font pointer for putting this in a List.
*/
	struct xifont	*prev;
/**
 Optional name for searching in lists.
*/
	char	*name;
/**
 Optional author string.
*/
	char	*author;
/**
 Transparent colour.
*/
	ulong	maskcol;
/**
 Destination rectangle for slightly faster operation.
*/
	SDL_Rect	dst;
/**
 Width of each symbol.
*/
	Uint16	w;
/**
 Height of each symbol.
*/
	Uint16	h;
/**
 Extra space to the right of each glyph when printed.
*/
	Uint16	spacing;
/**
 The table of glyphs.
@doc
 Theoretically, this could be expanded to handle
 wide characters. In practice, I don't think anyone
 could be arsed to actually use it.
*/
	SDL_Surface	*glyph[256];	/* Pointers to SDL surfaces */
} xifont;

extern xifont *xi_loadfont(char *name);
extern void xi_freefont(xifont *font);
extern void xi_puttext(xifont *font, SDL_Surface *scr,
	int x, int y, char *text);

/**
 Calculate how many pixels wide a string printed with a fixed-width xifont will be.
@doc
 This is merely a wrapper for a multiplication of the length
 of the text and the fornt width.
 Use SDL_ttf for nice variable width fonts.
@param font A font in Xiqual's own, peculiar format.
@param text A string you want to find space for.
@return An integer indicating how many pixels wide the text will be on screen.
@author Ronny Bangsund
@see xifont, xi_loadfont(), xi_freefont(), xi_puttext()
*/
#define xi_textwidth(font, text)	(strlen(text) * font->w)

#ifdef __cplusplus
}
#endif

#endif /* EOF */
