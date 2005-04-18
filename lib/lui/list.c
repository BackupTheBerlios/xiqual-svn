/* Generic list-function.
 * A list is a square with two button to scroll up and down.
 * What gets drawn in the box varies, as a hook is required to
 * do any actual actions.
 * This user-supplied hook gets a message structure containing
 * the type of action (UP, DOWN, SELECT), and position, if needed.
 *
 * Note that lists are simplified forms of listviews; the latter
 * will always have a slider to indicate position.
 *
 * Requirements:
 * - Button images, normal, highlighted and clicked
 * - A graphic image for the field to be used as a list.
 *   This determines the size of the list, if nothing else does.
 *
 * Notes:
 * Top button scrolls up, bottom button scrolls down. If these
 * buttons do not fill the entire width of the actual list,
 * there should be an option to write in these areas.
 */

#include "xios.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "xiqual.h"

#include "lui_button.h"
#include "lui_list.h"


/**
 Create a new LUI list gadget.
@doc
 The LUI list is a very simple gadget. It creates a field
 to be used for drawing/blitting, and two buttons above
 and below to scroll through the list. A future version
 will allow sidewise orientation.

 The user must supply graphics for the outline of the view
 and optionally the pushed-in state of the buttons.
 Hooks for the up/down buttons and clicks in the main
 list are required, and finally a draw hook to update
 the gadget with whatever data is necessary.

 This gadget is for the DIY people out there.
 More complete gadgets may be built from this.
@author Ronny Bangsund
@see gadget
*/
lui_list *lst_new(ulong tag1, ... )
{
	XI_VARARG(lst_newA((tagitem *)&tag1), lui_list)
}

lui_list *lst_newA(tagitem *taglist)
{
	tagitem	*t;
	lui_list	*lst;
	Uint32	bgcol=0;

	lst = calloc(1, sizeof(lui_list));
	if(!lst) return NULL;

	lst->id = -1;
	lst->type = GAD_LIST;
	t = tag_next(&taglist);
	while(t) {
		switch(t->tag) {
		case LUI_NAME:
			lst->name = strdup((char *)t->data);
			break;
		case LUI_ID:
			lst->id = t->data;
			break;
		case LUI_DATA:
			lst->data = (void *)t->data;
			break;
		case LUI_ACTIVATEHOOK:
			lst->onactivate = (void *)t->data;
			break;
		case LUI_XPOS:
			lst->x = t->data;
			break;
		case LUI_YPOS:
			lst->y = t->data;
			break;
		case LUI_WIDTH:
			lst->w = t->data;
			break;
		case LUI_HEIGHT:
			lst->h = t->data;
			break;
		case LUI_SCREEN:
			lst->screen = (SDL_Surface *)t->data;
			break;
		case LST_UPHOOK:
			lst->onup = (void *)t->data;
			break;
		case LST_DOWNHOOK:
			lst->ondown = (void *)t->data;
			break;
		case LST_DRAWHOOK:
			lst->ondraw = (void *)t->data;
			break;
		case LST_BORDERWIDTH:
			lst->bw = t->data;
			break;
		case LST_BORDERHEIGHT:
			lst->bh = t->data;
			break;
		case LST_IMAGE:
			lst->bg = IMG_Load((char *)t->data);
			break;
		case LST_UPCLICKIMAGE:
			lst->upclick = view_new(XIVT_SCREEN, lst->screen,
				XIVT_XPOS, lst->x,
				XIVT_YPOS, lst->y,
				XIVT_BGCOL, bgcol,
				XIVT_IMAGE, t->data,
				XIVT_FLAGS, XIV_NOKEY, TAG_END);
			break;
		case LST_DOWNCLICKIMAGE:
			lst->downclick = view_new(XIVT_SCREEN, lst->screen,
				XIVT_XPOS, lst->x,
				XIVT_YPOS, (lst->y + lst->bg->h) - lst->bh,
				XIVT_BGCOL, bgcol,
				XIVT_IMAGE, t->data,
				XIVT_FLAGS, XIV_NOKEY, TAG_END);
			break;
		case LST_BGCOL:
			bgcol = t->data;
		}
		t = tag_next(&taglist);
	}

	if((!lst->screen) || (!lst->bg)) {
		lst_free(lst);
		return NULL;
	}

	if(!lst->w) lst->w = lst->bg->w;
	if(!lst->h) lst->h = lst->bg->h;

	lst->ia = iarea_new(IA_INPUT, IA_CTL_MOUSE,
		IA_MBUTTONS, IA_LMB_MASK,
		IA_XPOS, lst->x + lst->bw,
		IA_YPOS, lst->y + lst->bh,
		IA_WIDTH, lst->w - (lst->bw * 2),
		IA_HEIGHT, lst->h - (lst->bh * 2), 
		IA_CLICK_HOOK, lst_activate,
		IA_PASS_HOOK, lui_highlight,
		IA_DATA, lst, IA_ID, lst->id,
		IA_DELAY, 75, TAG_END);

	lst->up = iarea_new(IA_INPUT, IA_CTL_MOUSE,
		IA_MBUTTONS, IA_LMB_MASK,
		IA_XPOS, lst->x, IA_YPOS, lst->y,
		IA_WIDTH, lst->w, IA_HEIGHT, lst->bh, 
		IA_CLICK_HOOK, lst_upclick,
		IA_PASS_HOOK, lst_uprelease,
		IA_DATA, lst, IA_ID, lst->id,
		IA_DELAY, 75, TAG_END);

	lst->down = iarea_new(IA_INPUT, IA_CTL_MOUSE,
		IA_MBUTTONS, IA_LMB_MASK,
		IA_XPOS, lst->x, IA_YPOS, lst->y + lst->h - lst->bh,
		IA_WIDTH, lst->w, IA_HEIGHT, lst->bh, 
		IA_CLICK_HOOK, lst_downclick,
		IA_PASS_HOOK, lst_downrelease,
		IA_DATA, lst, IA_ID, lst->id,
		IA_DELAY, 75, TAG_END);

	if((!lst->ia) || (!lst->up) || (!lst->down)){
		lst_free(lst);
		return NULL;
	}

	lst->view = view_new(XIVT_SCREEN, lst->screen,
		XIVT_XPOS, lst->x + lst->bw,
		XIVT_YPOS, lst->y + lst->bh,
		XIVT_WIDTH, lst->w - (lst->bw * 2),
		XIVT_HEIGHT, lst->h - (lst->bh * 2),
		XIVT_BGCOL, bgcol,
		XIVT_FLAGS, XIV_FILLBACK | XIV_NOKEY | XIV_NOSTORE, TAG_END);

	if(!lst->view) {
		lst_free(lst);
		return NULL;
	}
	lst->add = lst_add;
	lst->rem = lst_rem;
	lst->destroy = lst_free;
	lst->activate = lst_activate;
	lst->draw = lst_draw;
	lst->highlight = 0;
	return lst;
}

void lst_free(lui_list *lst)
{
	if(!lst) return;

	if(lst->ia) iarea_free(lst->ia);
	if(lst->up) iarea_free(lst->up);
	if(lst->down) iarea_free(lst->down);
	if(lst->view) view_free(lst->view);
	if(lst->bg) SDL_FreeSurface(lst->bg);
	if(lst->upclick) view_free(lst->upclick);
	if(lst->downclick) view_free(lst->downclick);
	if(lst->name) free(lst->name);
	free(lst);
}

void lst_activate(lui_list *lst)
{
	lui_input	input;

	if(lst->flags & LUIF_GHOSTED) return;

	input.x = xig->cursor->x - lst->x - lst->bw + xig->hotx;
	input.y = xig->cursor->y - lst->y -lst->bh + xig->hoty;
	if(lst->onactivate) lst->onactivate(&input, lst);
}


/* The activation hook for the up button will
 * be called if the gadget isn't disabled.
 */
void lst_upclick(lui_list *lst)
{
	if(lst->flags & LUIF_GHOSTED) return;

	view_show(lst->upclick);
	if(lst->onup) {
		view_hide(lst->view);
		lst->first = lst->onup(lst->first, lst->data);
		if(lst->ondraw) lst->ondraw(lst->first,
			lst->view->image, lst->data);
		view_show(lst->view);
	}
}

/* This is the IA_PASS_HOOK.
 * If no buttons are pressed, and the cursor
 * is above this area, remove the clicked image.
 * It's also called when the cursor is moved away.
 */
void lst_uprelease(lui_list *lst)
{
	if(lst->flags & LUIF_GHOSTED) return;

	view_hide(lst->upclick);
}

void lst_downclick(lui_list *lst)
{
	if(lst->flags & LUIF_GHOSTED) return;

	view_show(lst->downclick);
	if(lst->ondown) {
		view_hide(lst->view);
		lst->first = lst->ondown(lst->first, lst->data);
		if(lst->ondraw) lst->ondraw(lst->first,
			lst->view->image, lst->data);
		view_show(lst->view);
	}
}

/* Same as lst_uprelease() */
void lst_downrelease(lui_list *lst)
{
	if(lst->flags & LUIF_GHOSTED) return;

	view_hide(lst->downclick);
}

/* Main draw method, as called by LUI (normally).
 * A custom ondraw() method could fill the view
 * with pictures or text.
 */
void lst_draw(lui_list *lst)
{
	SDL_Rect	rect;

	rect.x = lst->x;
	rect.y = lst->y;
	view_hide(lst->view);
	SDL_BlitSurface(lst->bg, NULL, lst->screen, &rect);
	if(lst->ondraw) lst->ondraw(lst->first, lst->view->image, lst->data);
	if(lst->flags & LUIF_GHOSTED) {
		lst->flags ^= LUIF_GHOSTED;
		lui_ghost((gadget *)lst);
	}
	view_show(lst->view);
}

void lst_add(List *inputs, List *gadgets, lui_list *lst)
{
	lst->draw(lst);
	list_add(gadgets, (Node *)lst);
	list_add(inputs, (Node *)lst->ia);
	list_add(inputs, (Node *)lst->up);
	list_add(inputs, (Node *)lst->down);
}

void lst_rem(List *inputs, List *gadgets, lui_list *lst)
{
	view_hide(lst->view);
	view_hide(lst->upclick);
	view_hide(lst->downclick);
	list_remove(gadgets, (Node *)lst);
	list_remove(inputs, (Node *)lst->ia);
	list_remove(inputs, (Node *)lst->up);
	list_remove(inputs, (Node *)lst->down);
}
