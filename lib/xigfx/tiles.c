#include "SDL.h"
#include "SDL_image.h"
#include "zlib.h"

#include "iffparse.h"
#include "xiqual.h"
#include "xitiles.h"

static void xit_newsurface(List *list, xitiles *tiles, char *pixels);

/**@name Loading and handling tilesets
 Two-dimensional games are often tile-based, meaning its display
 is constructed from squares and/or rectangles merged together
 seamlessly (more or less).

 This small collection of routines load and display individual
 tiles. A separate tool, called "tileset", creates the tiles from
 images. Handling of tiles as a map requires extra code, too,
 which will be a part of Xiqual Real Soon Now(tm).
@see tileset
*/

/*@{*/
/**
 Load an IFF with compressed tiles.
@doc
 The tiles have been previously compiled with the tileset
 program. A tileset contains one or more tiles, the size
 of all tiles (same size for all), the colour that is to
 be considered transparent (handled automatically by the
 blitting functions) and a couple of rectangles for more
 efficient blitting.
@author Ronny Bangsund
@see xitiles, xit_draw(), xit_qdraw(), xit_freetiles()
*/
xitiles *xit_loadtiles(char *name)
{
	xitiles	*tiles;
	IFFHandle	*iff;
	Uint16	size[2];
	ulong	*compbuf=0, srclen, destlen, n=0;
	char	*decompbuf=0;
	List	list;

	if(!name) return NULL;

	memset(&list, 0, sizeof(List));
	tiles = calloc(1, sizeof(xitiles));
	if(!tiles) return NULL;

	memset(tiles, 0, sizeof(xitiles));
	iff = iff_open(name);
	if(!iff) {
		free(tiles);
		return NULL;
	}

	/* This loader only accepts one FORM type */
	if(!(iff->type == MAKE_ID('T','I','L','S'))) {
		iff_close(iff);
		free(tiles);
		return NULL;
	}

	while(iff_readchunkheader(iff)) {
		switch(iff->id) {
		case MAKE_ID('N','A','M','E'):
			tiles->name = calloc(1, iff->chunksize);
			if(tiles->name) {
				iff_readchunkdata(iff, tiles->name, iff->chunksize);
			} else {
				iff_skipchunkdata(iff);
			}
			break;
		case MAKE_ID('S','i','z','e'):
			iff_readchunkdata(iff, (char *)&size, 4);
			tiles->w = _SwapBE16(size[0]);
			tiles->h = _SwapBE16(size[1]);
			break;
		case MAKE_ID('M','a','s','k'):
			iff_readchunkdata(iff, (char *)&tiles->maskcol, 4);
			tiles->maskcol = _SwapBE32(tiles->maskcol);
			break;
		case MAKE_ID('T','i','l','e'):
			/* Create a buffer for compressed data */
			if(!compbuf) {
				compbuf = calloc(tiles->w, tiles->h * 3);
				if(compbuf) memset(compbuf, 0, tiles->w * tiles->h * 3);
			}
			if(!decompbuf) {
				decompbuf = calloc(tiles->w, tiles->h * 3);
				if(decompbuf) memset(decompbuf, 0, tiles->w * tiles->h * 3);
			}
			if(compbuf && decompbuf) {
				iff_readchunkdata(iff, (char *)compbuf, iff->chunksize);
				srclen = iff->chunksize -4;
				destlen = _SwapBE32(compbuf[0]);
				if(uncompress((char *)decompbuf, &destlen,
					(char *)compbuf + 4, srclen) == Z_OK) {
					xit_newsurface(&list, tiles, decompbuf);
				}
			}
			break;
		default:
			iff_skipchunkdata(iff);
			break;
		}
	}
	if(compbuf) free(compbuf);
	if(decompbuf) free(decompbuf);
	iff_close(iff);
	/* It's pointless to go on with no tiles! */
	if(!list.size) {
		free(tiles);
		return NULL;
	}
	tiles->count = list.size;
	/* Initialise the quick-blit rectangle */
	tiles->dst.w = tiles->w;
	tiles->dst.h = tiles->h;
	/* Build an array of pointers */
	tiles->t = calloc(sizeof(SDL_Surface *), tiles->count);
	if(!tiles->t) {
		xit_freetiles(tiles);
		return NULL;
	}

	memset(tiles->t, 0, sizeof(SDL_Surface *) * tiles->count);
	while(list.head) {
		tiles->t[n] = list.head->data;
		list_delete(&list, list.head);
		n++;
	}
	return tiles;
}


static void xit_newsurface(List *list, xitiles *tiles, char *pixels)
{
	SDL_Surface	*tmp, *tile;
	Uint32	rmask, gmask, bmask, amask;
	Node	*n;
	int	line, pos;

	/* I *like* big-endian formats */
	rmask = 0xff0000;
	gmask = 0x00ff00;
	bmask = 0x0000ff;
	/* We're using colour-keyed blits only (for now) */
	amask=0;
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY,
		tiles->w, tiles->h, 24, rmask, gmask, bmask, amask);
	if(!tmp) return;

	pos = 0;
	for(line=0; line<tiles->h; line++) {
		memcpy((char *)tmp->pixels + (tmp->pitch * line),
			pixels + pos, 3 * tiles->w);
		pos += 3 * tiles->w;
	}

	SDL_SetColorKey(tmp, SDL_RLEACCEL|SDL_SRCCOLORKEY, tiles->maskcol);
	tile = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	if(!tile) return;

	n = list_newitem(list, tile);
	if(!n) {
		SDL_FreeSurface(tile);
		return;
	}
}


/**
 Free a tileset.
@doc
 You must deallocate each tileset before the program exits,
 of course. This function frees all lists, arrays and surfaces
 allocated in a tileset.
@author Ronny Bangsund
@see xit_loadtiles(), tileset
*/
void xit_freetiles(xitiles *tiles)
{
	ulong	n;

	if(!tiles) return;

	if(tiles->name) free(tiles->name);
	if(tiles->t) {
		for(n=0; n<tiles->count;n++) {
			if(tiles->t[n]) SDL_FreeSurface(tiles->t[n]);
		}
		free(tiles->t);
	}
	free(tiles);
}

/**
 Blit from a tileset into the specified SDL_Surface.
@doc
 This is one of two calls available to display tiles in a set.
 The recommended method is to call xit_setdefaults() and use
 xit_qdraw() for all main blits, and xit_draw() only for blits
 that don't happen very often. Your program's structure may
 of course affect the validity of this method.
@author Ronny Bangsund
@see xit_loadtiles(), tileset
*/
void xit_draw(xitiles *tiles, ulong num, SDL_Surface *dest, Uint16 x, Uint16 y)
{
	if((!tiles) || (!dest) || (num >= tiles->count)) return;

	tiles->dst.x = x;
	tiles->dst.y = y;
	SDL_BlitSurface(tiles->t[num], NULL, dest, &tiles->dst);
}

/**
 Blit to default surface from default tileset
@doc
 Uses the defaults set by the most recent call to xit_setdefaults()
 to blit a specified tile image to an SDL_Surface.
@author Ronny Bangsund
@see xit_loadtiles(), xit_setdefaults(), tileset
*/
void xit_qdraw(ulong num, Uint16 x, Uint16 y)
{
	if(!xig->tileset) return;

	xig->tileset->dst.x = x;
	xig->tileset->dst.y = y;
	SDL_BlitSurface(xig->tileset->t[num], NULL,
		xig->tscreen, &xig->tileset->dst);
}


/*@}*/
