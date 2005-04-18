/*
 * Views structures and definitions
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: views.h,v 1.13 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: views.h,v $
 * Revision 1.13  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.10  2001/12/14 08:42:17  nny
 * Simplified iarea_check() to check only one inputarea at a time. Looping
 * to check more will have to be done in the calling routine. Various
 * changes were made to views and iareas to aid the creation of buttons in
 * the forthcoming button system.
 *
 * Revision 1.9  2001/12/09 20:32:46  nny
 * Added two new tags to view_new(): XIV_IMAGE and XIV_ALTIMAGE, to
 * directly load appropriate images.
 *
 * Revision 1.8  2001/12/09 16:49:40  nny
 * Changed view_show() back to original style, and added an alt flag in the
 * view structure instead. Set it to use the altimage during view_show().
 *
 * Revision 1.7  2001/12/09 16:47:06  nny
 * Changed the API a bit.
 *
 * Revision 1.6  2001/12/07 23:18:36  nny
 * Changed some datatypes because of sign clash.
 *
 */

#if !defined(_XIQUAL_VIEWS_H)
#define _XIQUAL_VIEWS_H

#include "SDL.h"

#include "lists.h"
#include "xiendian.h"
#include "xitags.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 This is a view, for use as sprites, message boxes, or anything
 else that might need to store the rectangle it is blitted over.
@author Ronny Bangsund
@see view_new(), view_show(), view_hide()
*/
typedef struct xiview {
/**
 Next view, if used in a List.
*/
	struct xiview	*next;
/**
 Previous view, if used in a List.
*/
	struct xiview	*prev;
/**
 Optional name for this view.
*/
	char	*name;
/**
 Rectangle we blitted this view over (available by default).
*/
	SDL_Surface	*store;
/**
 The image to blit onto screen.
*/
	SDL_Surface	*image;
/**
 This is the SDL_Surface that this view is tied to.
@doc
 Note that there is nothing stopping you from using any old surface
 to blit onto.
*/
	SDL_Surface	*screen;
/**
 Various xiview flags, as documented in view_new().
*/
	ulong	flags;				/* See below */
/**
 This will be used as fill colour if XIV_FILLBACK is defined in flags,
 or a colourkey unless XIV_NOKEY is defined.
*/
	Uint32	bgcol;			/* Background colour for fills */
/**
 Width of area to blit onto.
*/
	int	w;
/**
 Height of area to blit onto.
*/
	int	h;
/**
 X position in screen.
*/
	int	x;
/**
 Y position in screen.
*/
	int	y;
/**
 TRUE if hidden by view_hide().
*/
	char	hidden;
} xiview;

extern xiview *view_new(ulong tag1, ... );
extern void view_free(xiview *view);
extern void view_show(xiview *view);
extern void view_hide(xiview *view);

/* Tags */
enum {
	XIV_BASE = ( ('V'<<24) | ('I'<<16) | ('E'<<8) | 'W' ),
	XIVT_FLAGS,	/* Set the below flags with this */
	XIVT_SCREEN,
	XIVT_XPOS,
	XIVT_YPOS,
	XIVT_WIDTH,
	XIVT_HEIGHT,
	XIVT_BGCOL,	/* Set background colour - defaults to black */
	XIVT_IMAGE	/* Name of image to load */
};

/* Flags */
#define	XIV_FILLBACK	1	/* Fill back with bgcol */
#define	XIV_NOKEY		2	/* Don't colourkey the blits */
#define	XIV_NOSTORE	4	/* We don't want to store images before showing a view */

#ifdef __cplusplus
}
#endif

#endif /* EOF */
