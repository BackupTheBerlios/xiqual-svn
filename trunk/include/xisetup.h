/* Xiqual setup definitions
 * When creating the initial screen and requesting
 * audio mixing, use the tags defined here.
 */

#if !defined(_XIQUAL_SETUP_H)
#define _XIQUAL_SETUP_H

#include "SDL.h"
#include "SDL_image.h"

#include "xios.h"
#include "fonts.h"
#include "lui_msg.h"
#include "prefs.h"
#include "views.h"
#include "xitags.h"
#include "xitiles.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 Global structure for Xiqual's automatic handling of miscellaneous data.
*/
typedef struct xiglob {
	List	*prefs;
/**
 This is the SDL surface returned by SDL_SetVideoMode().
*/
	SDL_Surface	*screen;
/**
 An xiview for the main cursor.
*/
	xiview	*cursor;
/**
 Cursor X offset.
*/
	int	hotx;
/**
 Cursor Y offset.
*/
	int	hoty;
/**
 Default surface for tile blits.
@see xit_setdefaults(), xit_qdraw()
*/
	SDL_Surface	*tscreen;
/**
 Default tileset to blit from.
*/
	struct xitiles	*tileset;

/**
 Sample rate.
@doc
 Measured in samples per second. Default is 44100 Hz.
*/
	int	audio_rate;
/**
 Number of channels to use.
@doc
 Use 1 for mono and 2 for stereo.
*/
	int	audio_chan;
/**
 Maximum number of channels to mix.
@doc
 Default is eight.
*/
	int	numchans;
/**
 Size of audio buffers.
@doc
 This doesn't have to be a particularly large number, but must be
 a factor of two. Defaults to 4096 bytes.
*/
	int	audio_bufsize;
/**
 Audio format.
@doc
 The format definitions are in SDL_mixer.h.
*/
	Uint16	audio_fmt;
/**
 Sound nodes.
@doc
 Contents TBA.
*/
	List	sounds;
/**
 Music nodes.
@doc
 Contents TBA.
*/
	List	music;

/**
 Image nodes.
@doc
 Contents TBA.
*/
	List	images;	/* List of xiobj */
/**
 Sprite nodes.
@doc
 Contents TBA.
*/
	List	sprites;

	/* Hooks */

/**
 User-definable hook, called once each time xi_main() is called.
@param keystate A pointer to an array of Uint8. Currently pressed keys are non-zero.
@param modstate State of modifier keys
@param mousestate Flags for current mouse state (buttons).
@param mx Current X position of mouse cursor within the main SDL surface
@param my Current Y position of mouse cursor within the main SDL surface
@author Ronny Bangsund
@see xi_main(), SDL_input.h
*/
	void	(*mainloop)(Uint8 *keystate, SDLMod modstate, Uint8 mousestate, int mx, int my);

	ulong	flags;

	/* LUI data */
/**
 Mesh overlay for disabled gadgets.
@doc
 When a gadget is "ghosted", this surface is copied over it.
@see lui_ghost()
*/
	SDL_Surface	*mesh;
/**
 Pointer to the currently highlighted gadget.
@doc
 NULL if the cursor is not hovering above a gadget
 that can be highlighted. You are free to abuse the
 highlight method of a gadget for just knowing when
 the cursor is above it.
@see lui_highlight(), gadget
*/
	gadget	*hl;
/**
 List of LUI gadgets.
@doc
 Any gadgets in this list are automatically freed
 when xi_quit()/lui_quit() is called.
*/
	List	gadgets;
/**
 List of input areas.
@doc
 Any inputs in this list are automatically freed
 when xi_quit()/lui_quit() is called.
*/
	List	inputs;
/**
 Currently shown message box.
*/
	lui_msg	*msg;
} xiglob;


/* Like a node, but with slightly more information */
typedef struct xiobj {
	struct xiobj	*next, *prev;
	char	*name;	/* We look up objects by this */
	void	*data;	/* Anything that suits us (sound, image, sprite, script) */
} xiobj;


/* Tag definitions */
enum {XIGLOB_BASE=(('G'<<24) | ('L'<<16) | ('O'<<8) | ('B')),
	XI_WIDTH, XI_HEIGHT, XI_DEPTH,	/* Size & depth of main screen */
	XI_SDLFLAGS,	/* Extra SDL flags - xi_init() will SDL_Init() all subsystems by default,
								i.e. SDL_INIT_EVERYTHING. This tag REPLACES the flags for SDL_Init()! */
	XI_SCRFLAGS,	/* Defaults to SDL_HWSURFACE | SDL_DOUBLEBUF */
	XI_CURSOR,	/* SDL_Surface with cursor - xi_quit() will free this */
	XI_CURSORFILE,	/* ..or a filename, if you prefer */
	XI_CURSORKEY,	/* Colourkey returned by SDL_MapRGB (defaults to R=0, G=0, B=255) */
	XI_CURSOR_HOTX, XI_CURSOR_HOTY,	/* Hotspot (actual point that selects things on the screen) */
	XI_MAINHOOK,	/* Shove a main loop here */
	XI_PREFS,	/* Name of file to look for in default locations */
	XI_AUDIORATE, XI_AUDIOCHAN, XI_AUDIOBUFSIZE, XI_AUDIO_FMT,
	XI_MIXCHAN,
	XI_TTF,
	XI_LUI
};

#define XIF_USE_TTF	1
#define XIF_USE_LUI	2


extern xiglob *xi_init(ulong tag1, ... );
extern xiglob *xi_initA(tagitem *tags);
extern void xi_inittagA(tagitem *tags);
extern void xi_inittag(ulong tag1, ... );
extern void xi_quit();


#ifdef __cplusplus
}
#endif

#endif /* EOF */
