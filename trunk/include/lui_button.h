/*
 * Button structure, tags and functions.
 */

#if !defined(_LUI_BUTTONS_H)
#define _LUI_BUTTONS_H

#include "iarea.h"
#include "views.h"
#include "xitags.h"
#include "xitiles.h"
#include "lui.h"

#if defined(__cplusplus)
extern "C" {
#endif


typedef struct lui_bt {
	/* Node structure */
	struct lui_bt	*next;
	struct lui_bt	*prev;
	char	*name;

	/* LUI elements common to all gadgets */
	void	(*add)(List *, List *, struct lui_bt *);
	void	(*rem)(List *, List *, struct lui_bt *);
	void	(*destroy)(struct lui_bt *);
	void	(*activate)(struct lui_bt *);
	void	(*draw)(struct lui_bt *);
	void	(*highlight)(struct lui_bt *);
	void	(*ghost)(struct lui_bt *);

	void	*data;
	ulong	id;
	ulong	type;
	ulong	flags;
	int	w, h, x, y;
	inputarea	*ia;
	SDL_Surface	*screen;

	/* Gadget-specific elements */
	SDL_Surface	*store, *image;
	SDL_Surface	*hlimage, *click, *hlclick;

	xitiles	*tileset;
	ulong	tile, hltile, ctile, hlctile;

	void	(*onactivate)(struct lui_bt *, lui_input *);

	char	state; /* Is the toggle currently pressed in? */
} lui_bt;


/* Tags */
enum {
	LBT_IMAGE=(('B'<<24) | ('U'<<16) | ('T'<<8) | ('N')),
	/* Regular image (SDL_Surface *) */
	LBT_HLIMAGE,	/* Mouseover image (if any) */
	LBT_CLICKIMAGE,	/* Same procedure for clicked images */
	LBT_HLCLICKIMAGE,
	LBT_TOGGLE,	/* Boolean to make it a checkbutton */
	LBT_TILESET,	/* Use a tileset for source images */
	LBT_TILE,
	LBT_HLTILE,
	LBT_CLICKTILE,
	LBT_HLCLICKTILE,
};

/* Image identifiers */
enum {
	BT_IMAGE=1, BT_HLIMAGE, BT_CLICKIMAGE, BT_HLCLICKIMAGE
};

/* Public functions */
/*!
 */
extern lui_bt *bt_new(ulong tag1, ... );
extern lui_bt *bt_newA(tagitem *taglist);
extern void bt_free(lui_bt *bt);
void bt_draw(lui_bt *bt);
void bt_highlight(lui_bt *bt);

/* Internal stuff */
extern void bt_blit(lui_bt *bt, SDL_Surface *surface);
extern void bt_tile(lui_bt *bt, int n);
extern void bt_activate(lui_bt *bt);


#if defined(__cplusplus)
}
#endif

#endif	/* EOF */
