/*
 * Lame user interface
 * Hoping to one day become likable
 *
 */

#if !defined(_LUI_H)
#define _LUI_H

#include <SDL/SDL.h>

#include "iarea.h"
#include "views.h"

#if defined(__cplusplus)
extern "C" {
#endif


typedef struct lui_input {
	ulong	type;
	int	x, y;
	void	*data;
} lui_input;

/**
 Generic gadget.
@doc
 Every button, checkbox, listview etc. in LUI has this structure,
 with its own private data in an extended part of the structure.
@see iarea_check()
*/
typedef struct gadget {
	/* Node structure */
/**
 Next gadget in List.
*/
	struct gadget	*next;
/**
 Previous gadget in List.
*/
	struct gadget	*prev;
/**
 Optional string for searching.
*/
	char	*name;

	/* LUI elements common to all gadgets */
/**
 Add gadget to lists.
@doc
 Gadgets which require addition of more inputareas
 than the default at creation time use this method.
@param inputs Pointer to list of inputareas.
@param gadgets Pointer to list of gadgets.
@param gadget This gadget.
*/
	void	(*add)(List *inputs, List *gadgets, struct gadget *);
/**
 Remove gadget from lists.
@doc
 Gadgets which added more inputareas than
 the default at creation time use this method.
@param inputs Pointer to list of inputareas.
@param gadgets Pointer to list of gadgets.
@param gadget This gadget.
*/
	void	(*rem)(List *inputs, List *gadgets, struct gadget *);
/**
 LUI destructor.
@doc
 This frees the gadget and all child gadgets (if any),
 along with all their data. The gadget must already
 have been removed from any lists before this is called.
*/
	void	(*destroy)(struct gadget *);
/**
 LUI activation method.
@doc
 This draws a gadget's active state,
 and calls any onactivate() hooks it may have installed.
*/
	void	(*activate)(struct gadget *);
/**
 LUI draw method.
@doc
 Used internally by LUI to call upon the correct method(s)
 to draw a gadget and also perform any cleanup related
 to highlight state, hiding and showing other gadgets etc.
*/
	void	(*draw)(struct gadget *);
/**
 LUI highlight method.
@doc
 If a gadget has installed a hook for mouseover actions,
 it will be called from this. This function is normally
 handled from within LUI when the cursor passes over
 the related gadget.
*/
	void	(*highlight)(struct gadget *);
/**
 Ghosting method.
@doc
 If a gadget has special requirements for making itself
 look disabled, this method gets installed here.
 Used by gadgets that change size (menus).
*/
	void	(*ghost)(struct gadget *);

/**
 User-definable data.
@doc
 This pointer is passed along to user-defined hooks.
*/
	void	*data;
/**
 Optional identifier.
@doc
 Returned by iarea_check(), so you can check for activation in other ways
 than a callback hook, or for other forms of record keeping.
 Defaults to -1; iarea_check() returns 0 when a check fails!
*/
	ulong	id;
/**
 Type of gadget.
@doc
 See lui.h for a list of types.
*/
	ulong	type;
/**
 LUI flags.
@doc
 User-readable flags go here. None are implemented yet.
*/
	ulong	flags;
/**
 Width.
@doc
 Read-only size of entire gadget.
*/
	int	w;
/**
 Height.
@doc
 Read-only size of entire gadget.
*/
	int	h;
/**
 X position in screen.
@doc
 Read-only position of entire gadget.
*/
	int	x;
/**
 Y position in screen.
@doc
 Read-only size of entire gadget.
*/
	int	y;
/**
 Main input area.
@doc
 This may or may not cover the entire surface
 of the gadget. Some gadgets create more input areas
 for sub-components, and some may even have some
 of these areas within its own ia's borders.
*/
	inputarea	*ia;
/**
 Screen.
@doc
 This is the SDL surface this gadget is drawn on
 when its draw() method is called.
*/
	SDL_Surface	*screen;
} gadget;


/* Common tags shared by all controls */
enum {
	LUI_ID = (('L'<<24) | ('U'<<16) | ('I'<<8)),
	LUI_ACTIVATEHOOK,	/* Called on a click */
	LUI_PASSHOOK,	/* Called when cursor hovers over a control */
	LUI_NAME,	/* A name, if wanted (for lookup or just because you can) */
	LUI_DATA,	/* Control-specific data (user-defined) */
	LUI_XPOS,	/* Control placement in surface */
	LUI_YPOS,
	LUI_WIDTH,	/* Size of entire control */
	LUI_HEIGHT,	/* (Controls like listviews consist of multiple smaller controls */
	LUI_SCREEN,	/* SDL surface to display control in */
	LUI_TITLE	/* For controls that have need/ability of a title */
};

/* Gadget types */
enum {
	GAD_BUTTON=1,
	GAD_TOGGLE,
	GAD_LIST,
	GAD_MENU,		/* Also for popup menus */
	GAD_MESSAGE,	/* Plain message box with one button */
	GAD_CONFIRM		/* Confirmation box; two buttons */
};

/* Gadget flags */
#define LUIF_GHOSTED 1
/* Some gadgets may not always be visible
 * (popup menus, menus, message boxes)
 */
#define LUIF_HIDDEN LUIF_GHOSTED

/* Avoids flickering */
#define LUIF_HIGHLIGHTED 2

/* Left-right orientation */
#define LUIF_HORIZONTAL	3

/* Top-bottom (or bottom-top) orientation */
#define LUIF_VERTICAL	4

/* This function loads required images
 * for the core GUI system.
 */
extern int lui_init();
extern void lui_quit();

extern void lui_ghost(gadget *g);
extern void lui_highlight(gadget *g);

extern gadget *lui_newgadget(ulong type, ulong tag1, ... );
extern gadget *lui_newgadgetA(ulong gadtype, tagitem *taglist);
extern void lui_freegadget(gadget *g);

extern void lui_addgadget(gadget *g);
extern void lui_remgadget(gadget *g);
extern void lui_freegadgets(List *l);

#if defined(__cplusplus)
}
#endif

#endif /* EOF */
