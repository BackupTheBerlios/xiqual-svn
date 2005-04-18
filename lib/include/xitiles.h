/*
 * Both maps and tilesets are in the same sources for now.
 * Maybe it's a good idea to separate them later..
 */
#if !defined(_XIQUAL_TILES_H)
#define _XIQUAL_TILES_H

#include <SDL/SDL.h>

#include "lists.h"
#include "xiendian.h"
#include "xisetup.h"

#if defined(__cplusplus)
extern "C" {
#endif


/**
 A tileset in-memory representation.
@doc
 This is the structure given to each tileset
 when loaded from an IFF. See the tileset program
 for a description of the file formats used.
@see tileset, xit_loadtiles(), xit_draw(), xit_qdraw()
*/
typedef struct xitiles {
	struct xitiles *next, *prev;
	char	*name;
	ulong	count; /* Number of tiles in this set */
	Uint16		w, h; /* Size of each tile */
	Uint32	maskcol; /* Transparent colour (RGB format) */
	SDL_Rect	dst; /* For quicker operation */
	SDL_Surface	**t; /* Quick list of tiles */
} xitiles;


extern xitiles *xit_loadtiles(char *name);
extern void xit_freetiles(xitiles *tiles);
extern void xit_draw(xitiles *tiles, ulong num, SDL_Surface *dest, Uint16 x, Uint16 y);
extern void xit_qdraw(ulong num, Uint16 x, Uint16 y);


/**
 Set default screen and tileset
@doc
 This call just puts the supplied pointers into the appropriate
 places in the global structure. It's currently just a macro.
@param screen An SDL_Surface to blit to by default.
@param tiles Tileset to use for quick blits.
@author Ronny Bangsund
@see xit_loadtiles(), xit_qdraw(), tileset
*/
#define xit_setdefaults(screen, tiles) { \
	xig->tscreen = screen; \
	xig->tileset = tiles; }


#if defined(__cplusplus)
}
#endif

#endif	/* EOF */
