/* $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: views.c,v 1.3 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: views.c,v $
 * Revision 1.3  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.2  2003/05/30 08:54:58  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.18  2001/12/19 09:01:01  nny
 * Pruned logs.
 *
 * Revision 1.15  2001/12/11 18:46:45  nny
 * Forgot to make altimage same format as main surface. Fixed.
 *
 * Revision 1.14  2001/12/09 20:32:47  nny
 * Added two new tags to view_new(): XIV_IMAGE and XIV_ALTIMAGE, to
 * directly load appropriate images.
 *
 */

#include <stdarg.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "views.h"
#include "xitags.h"

xiview *view_newA(tagitem *taglist)
{
	xiview	*view;
	tagitem	*t;

	view = calloc(1, sizeof(xiview));
	if(!view) return NULL;

	t = tag_next(&taglist);
	while(t) {
		switch(t->tag) {
		case XIVT_FLAGS:
			view->flags = t->data;
			break;
		case XIVT_SCREEN:
			view->screen = (SDL_Surface *)t->data;
			break;
		case XIVT_XPOS:
			view->x = t->data;
			break;
		case XIVT_YPOS:
			view->y = t->data;
			break;
		case XIVT_WIDTH:
			view->w = t->data;
			break;
		case XIVT_HEIGHT:
			view->h = t->data;
			break;
		case XIVT_BGCOL:
			view->bgcol = t->data;
			break;
		case XIVT_IMAGE:
			if(t->data)
				view->image = IMG_Load((char *)t->data);
			break;
		}
		t = tag_next(&taglist);
	}
	if(!view->screen) {
		return NULL;
	}
	return view;
}

/**@name Views
 Views are rectangular screen-areas with backing-store.
 This is more or less a general-purpose "windowing"
 system for SDL-based program, or the foundation of one.
 It can be used for the mouse pointer, sprites, requesters,
 message boxes and heaps of other things.
*/

/*@{*/
/**
 Allocate a small chunk of display memory with back store.
@doc
 This structure can be used for sprites, cursors, message boxes or anything
 else that might need to display and keep its background for re-blitting later.

 Accepted tags:

 <B>XIVT_FLAGS</B>: The flags are <B>XIV_FILLBACK</B> to fill the created rectangle
 in view->image  with the specified background colour, <B>XIV_NOSTORE</B> to not create
 any storage for the background area and <B>XIV_NOKEY</B> to avoid colourkey blitting with
 the background colour as key (entire surface is copied instead of background showing
 through).

 <B>XIVT_SCREEN</B>: SDL surface to blit onto (need not be main screen).
 
 <B>XIVT_XPOS</B>, <B>XIVT_YPOS</B>: Position in suface to be blitted to.

 <B>XIVT_WIDTH</B>, <B>XIVT_HEIGHT</B>: Size of view and background storage, unless
 an image to be loaded is specified.

 <B>XIVT_BGCOL</B>: It's recommended that you use SDL_MapRGB() from the
 destination surface's format to set this, if you need it.

 <B>XIVT_IMAGE</B>: An SDL_image supported file to load and use as view->image.

@author Ronny Bangsund
@see view_free(), view_show(), view_hide()
*/
xiview *view_new(ulong tag1, ... )
{
	va_list	t;
	xiview	*view;
	SDL_PixelFormat	*format;
	SDL_Surface	*tmp;
	Uint32	flags;

	va_start(t, tag1);
	view = view_newA((tagitem *)&tag1);
	va_end(t);

	if(!view)
		return NULL;

	if(view->flags & XIV_NOKEY) {
		flags = SDL_RLEACCEL;
	} else {
		flags = SDL_SRCCOLORKEY | SDL_RLEACCEL;
	}
	format = view->screen->format;
	/* Convert any loaded image to the main surface's format
	 * to speed up all blits. */
	if(view->image) {
		tmp = view->image;
		view->image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		view->w = view->image->w;
		view->h = view->image->h;
		SDL_SetColorKey(view->image, flags, view->bgcol);
	} else {
		view->image = SDL_CreateRGBSurface(SDL_RLEACCEL, view->w, view->h,
			format->BitsPerPixel, format->Rmask, format->Gmask, format->Bmask, format->Amask);
		SDL_SetColorKey(view->image,  flags, view->bgcol);
		if(view->flags & XIV_FILLBACK) {
			SDL_FillRect(view->image, NULL, view->bgcol);
		}
	}

	if(!view->image) {
		SDL_FreeSurface(view->store);
		view_free(view);
		return NULL;
	}

	/* Create a buffer for background regions in the same format as the main surface */
	if(!(view->flags & XIV_NOSTORE))
		view->store = SDL_CreateRGBSurface(SDL_RLEACCEL, view->w, view->h,
			format->BitsPerPixel, format->Rmask, format->Gmask, format->Bmask, format->Amask);

	view->hidden = 1;
	return view;
}


/**
 Deallocate an xiview and its surface(s).
@doc
 The view will be hidden upon destruction, restoring any stored
 background. Note that this also means its screen element must
 be valid until after destruction.
@param view A view structure created with view_new()
@author Ronny Bangsund
@see view_new(), view_show(), view_hide()
*/
void view_free(xiview *view)
{
	if(!view) {
		return;
	}

	if(view->store) {
		view_hide(view);
		SDL_FreeSurface(view->store);
	}

	if(view->image) SDL_FreeSurface(view->image);

	free(view);
}

/**
 Blit view image to surface it is created for.
@doc
 If there is backing store allocated, the area at the view's position is copied
 to this backing store. If the view is already set as displayed through a
 previous call to this function, nothing happens at all.
@param view A view structure created with view_new()
@author Ronny Bangsund
@see view_new(), view_free(), view_hide()
*/
void view_show(xiview *view)
{
	SDL_Rect	rect;

	if(!view)
		return;

	if(!view->hidden)
		return;

	view->hidden = 0;
	rect.x = view->x;
	rect.y = view->y;
	rect.w = view->w;
	rect.h = view->h;
	if(!(view->flags & XIV_NOSTORE)) {
		SDL_BlitSurface(view->screen, &rect, view->store, NULL);
	}
	SDL_BlitSurface(view->image, NULL, view->screen, &rect);
}

/**
 Restore the area behind a view from its backing store.
@doc
 If the view flags has the XIV_NOSTORE bit set, nothing happens.
@param view A view structure created with view_new()
@author Ronny Bangsund
@see view_new(), view_free(), view_show()
*/
void view_hide(xiview *view)
{
	SDL_Rect	rect;

	if(!view)
		return;

	if(view->hidden)
		return;

	view->hidden = 1;
	rect.x = view->x;
	rect.y = view->y;

	if(!(view->flags & XIV_NOSTORE))
		SDL_BlitSurface(view->store, NULL, view->screen, &rect);
}

/*@}*/
