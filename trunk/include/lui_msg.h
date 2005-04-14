/*
 * LUI message boxes.
 * Square area (may be alpha/colourblitted to look
 * more shapely) with one or more buttons.
 */

#if !defined(_LUI_MSG_H)
#define _LUI_MSG_H

#if defined(HAVE_SDL_SDL_TTF)
#	include "SDL_ttf.h"
#endif

#include "SDL.h"

#include "fonts.h"
#include "iarea.h"
#include "views.h"

#include "lui.h"

#if defined(__cplusplus)
extern "C" {
#endif


/**
 Message box.
@doc
 A box with an optional message in it,
 and one or more buttons to allow
 confirmation and cancellation of actions.
@see msg_boxnew()
*/
typedef struct lui_msg {
	struct lui_msg	*next, *prev;
	char	*name;

	/* LUI elements common to all gadgets */
	void	(*add)(List *, List *, struct lui_msg *); /* Unused */
	void	(*rem)(List *, List *, struct lui_msg *); /* Unused */
	void	(*destroy)(struct lui_msg *);
	void	(*activate)(struct lui_msg *);
	void	(*draw)(struct lui_msg *);
	void	(*highlight)(struct lui_msg *);
	void	(*ghost)(struct lui_msg *); /* Unused */

	void	*data;
	ulong	id;
	ulong	type;
	ulong	flags;
	int		w, h, x, y, bw, bh;
	inputarea	*ia;
	SDL_Surface	*screen;

	/* The privates */
	SDL_Surface	*fg, *ok, *cancel;
	xiview		*bg;

	char		*message;
	struct gadget	*btok, *btcancel;
#if defined(HAVE_SDL_SDL_TTF)
	TTF_Font	*ttf;
#endif
	xifont		*font;

	void	(*onactivate)(lui_input *, struct lui_msg *);
} lui_msg;


/* Tags */
enum {
	MSG_BACKGROUND=(('M'<<24) | ('s'<<16) | ('g'<<8) | ('B')),
	MSG_MESSAGE,	/* String */
	MSG_TTF,	/* Truetype font to print message with */
	MSG_XIFONT,	/* Xiqual font to print message with (faster) */
	MSG_IMAGE,	/* Surface with message if no string */
	MSG_BORDERWIDTH, MSG_BORDERHEIGHT,
	MSG_OK,	MSG_OKHL, MSG_OKCLICK,	/* OK/confirm button images*/
	MSG_CANCEL, MSG_CANCELHL, MSG_CANCELCLICK
};


extern lui_msg *msg_boxnew(ulong tag1, ...);
extern lui_msg *msg_boxnewA(tagitem *taglist);
extern void msg_show(lui_msg *m, int show);
extern void msg_set(lui_msg *m, ulong tag1, ... );
extern void msg_setA(lui_msg *m, tagitem *taglist);
extern void msg_free(lui_msg *m);
void msg_draw(lui_msg *m);


#if defined(__cplusplus)
}
#endif

#endif /* EOF */
