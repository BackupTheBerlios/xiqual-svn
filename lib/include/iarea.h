/*
 * Input areas. Button system still sorta slow.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: iarea.h,v 1.6 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: iarea.h,v $
 * Revision 1.6  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.3  2001/12/14 08:42:17  nny
 * Simplified iarea_check() to check only one inputarea at a time. Looping
 * to check more will have to be done in the calling routine. Various
 * changes were made to views and iareas to aid the creation of buttons in
 * the forthcoming button system.
 *
 */

#if !defined( _XIQUAL_IAREA_H)
#define _XIQUAL_IAREA_H

#include <SDL/SDL.h>

#include "xitags.h"
#include "views.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This structure usually specifies the location of a clickable button,
 * but could also be used with zero width and height for a test that
 * will always be performed. This would be useful for joysticks etc.,
 * leaving screen-coordinate based inputareas to the mousepointer.
 */
/**
 An inputarea for testing mouse and other
 controllers in specific screen coordinates.
@doc
 The inputarea gets its function hooks filled from
 the tags supplied by iarea_new().
@author Ronny Bangsund
@see iarea_new(), iarea_free()
*/
typedef struct inputarea {
/**
 Next area if in a list-
*/
	struct inputarea	*next;
/**
 Previous area if in a list.
*/
	struct inputarea	*prev;
/**
 The optional name.
@doc
 Little used, but could allow associating areas
 by name if they're all part of a complex gadget.
*/
	char	*name;
/**
 User-definable data.
@doc
 Passed on to all the activation hooks.
*/
	void	*data;
/**
 X coordinate of upper left corner.
*/
	int	x;
/**
 Y coordinate of upper left corner.
*/
	int	y;
/**
 Width of rectangle.
*/
	int	w;
/**
 Height of rectangle.
*/
	int	h;	/* Size of rectangle */
/**
 Left mousebutton activation.
*/
	void	(*lmbfunc)(void *);	/* Left mousebutton activation */
/**
 Middle mousebutton activation.
*/
	void	(*mmbfunc)(void *);	/* Middle mousebutton activation */
/**
 Right mousebutton activation.
*/
	void	(*rmbfunc)(void *);	/* Right mousebutton activation */
/**
 Mouse passing over without buttons pressed.
*/
	void	(*pfunc)(void *);	/* Activation function */
/**
 Mask of controllers to respond to.
@doc
 IA_CTL_MOUSE and/or IA_CTL_JOY.
*/
	ulong	inputs;
/**
 Mandatory ID.
@doc
 iarea_check() returns this when a check is positive.
 It would be daft to supply a zero value.
 Zero means failure.
*/
	ulong	id;
/**
 Mousebutton mask.
@doc
 IA_LMBMASK, IA_MMB_MASK and/or IA_RMB_MASK.
 These are all aliases for SDL flags with slightly
 longer names, because I am terminally lazy.
*/
	Uint8	mb;
/**
 Mask of joybuttons.
@doc
 These have no presets, as which button is
 the first and which is the last depends
 on the joystick/pad in question. Bit #0
 will most likely be near one of your thumbs, though.
*/
	Uint8	jb;
/**
 Point when this inputarea was last activated.
@doc
 This is the point at which this inputarea was last activated.
 It uses SDL_GetTicks() to get its value, but the way delays
 are calculated shouldn't be affected by the 49-day limit.

 Note, however, that there is something else you are doing
 wrong if you have a game running for 49 days.
*/
	Uint32	tick;
/**
 Delay in milliseconds before allowing another activation.
@doc
 Use the IA_DELAY tag when creating to set this.
*/
	Uint32	delay;
} inputarea;

/* Tags */
enum {
	IA_BASE = (TAG_USER+0x3000),
	IA_DATA, /* Data to be passed to function */
	IA_CLICK_HOOK, /* Function called upon activation (LMB) */
	IA_RCLICK_HOOK, /* Clicking right mousebutton */
	IA_MCLICK_HOOK, /* Clicking middle mousebutton */
	IA_PASS_HOOK,
	IA_INPUT,	/* Controllers to respond to */
	IA_MBUTTONS,	/* Mousebuttons to respond to */
	IA_JBUTTONS,	/* Joybuttons to respond to */
	IA_XPOS,
	IA_YPOS,
	IA_WIDTH,
	IA_HEIGHT,
	IA_NAME,	/* Name of node */
	IA_ID,
	IA_DELAY
};


/* Controller flags */
#define	IA_CTL_MOUSE	0x1
#define	IA_CTL_JOY		0x2

#define	IA_LMB	SDL_BUTTON_LEFT
#define	IA_MMB	SDL_BUTTON_MIDDLE
#define	IA_RMB	SDL_BUTTON_RIGHT
#define	IA_LMB_MASK	SDL_BUTTON_LMASK
#define	IA_MMB_MASK	SDL_BUTTON_MMASK
#define	IA_RMB_MASK	SDL_BUTTON_RMASK

extern inputarea *iarea_new(ulong tag1, ... );
extern inputarea *iarea_newA(tagitem *taglist);
extern ulong iarea_check(inputarea *ia, int curx, int cury, Uint8 mousestate, Uint8 joystate);
extern void iarea_free(inputarea *ia);

#ifdef __cplusplus
}
#endif

#endif	/* EOF */
