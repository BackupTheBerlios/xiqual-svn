/*
 * Create buttons using views and inputareas.
 */

#include "SDL.h"
#include "SDL_image.h"

#include "xios.h"
#include "xitiles.h"

#include "lui_button.h"


/**
 Create a new LUI button.
@doc
 A normal, clickable button or a toggle button/checkbox
 may be created with this function.
@author Ronny Bangsund
@see lui_addgadget(), lui_remgadget()
*/
lui_bt *bt_new(ulong tag1, ... )
{
	XI_VARARG(bt_newA((tagitem *)&tag1), lui_bt)
}


lui_bt *bt_newA(tagitem *taglist)
{
	tagitem	*t;
	lui_bt	*bt;
	SDL_Rect	src, dst;
	SDL_PixelFormat	*format;

	bt = calloc(1, sizeof(lui_bt));
	if(!bt) return NULL;

	bt->id = -1;
	bt->type = GAD_BUTTON;
	t = tag_next(&taglist);
	while(t) {
		switch(t->tag) {
		case LUI_NAME:
			bt->name = strdup((char *)t->data);
			break;
		case LUI_ID:
			bt->id = t->data;
			break;
		case LUI_DATA:
			bt->data = (void *)t->data;
			break;
		case LUI_ACTIVATEHOOK:
			bt->onactivate = (void *)t->data;
			break;
		case LUI_XPOS:
			bt->x = t->data;
			break;
		case LUI_YPOS:
			bt->y = t->data;
			break;
		case LUI_WIDTH:
			bt->w = t->data;
			break;
		case LUI_HEIGHT:
			bt->h = t->data;
			break;
		case LUI_SCREEN:
			bt->screen = (SDL_Surface *)t->data;
			break;
		case LBT_IMAGE:
			bt->image = IMG_Load((char *)t->data);
			break;
		case LBT_HLIMAGE:
			bt->hlimage = IMG_Load((char *)t->data);
			break;
		case LBT_CLICKIMAGE:
			bt->click = IMG_Load((char *)t->data);
			break;
		case LBT_HLCLICKIMAGE:
			bt->hlclick = IMG_Load((char *)t->data);
			break;
		case LBT_TILESET:
			bt->tileset = (xitiles *)t->data;
			break;
		case LBT_TILE:
			bt->tile = t->data;
			break;
		case LBT_HLTILE:
			bt->hltile = t->data;
			break;
		case LBT_CLICKTILE:
			bt->ctile = t->data;
			break;
		case LBT_HLCLICKTILE:
			bt->hlctile = t->data;
			break;
		case LBT_TOGGLE:
			bt->type = GAD_TOGGLE;
			break;
		}
		t = tag_next(&taglist);
	}

	if(((!bt->image) && (!bt->tileset)) || (!bt->screen)) {
		bt_free(bt);
		return NULL;
	}

	if(!bt->w) {
		if(bt->tileset) {
			bt->w = bt->tileset->w;
		} else {
			bt->w = bt->image->w;
		}
	}

	if(!bt->h) {
		if(bt->tileset) {
			bt->h = bt->tileset->h;
		} else {
			bt->h = bt->image->h;
		}
	}

	format = bt->screen->format;
	bt->store = SDL_CreateRGBSurface(SDL_RLEACCEL, bt->w, bt->h,
		format->BitsPerPixel, format->Rmask, format->Gmask, format->Bmask, format->Amask);

	bt->ia = iarea_new(IA_INPUT, IA_CTL_MOUSE, IA_MBUTTONS, IA_LMB_MASK,
		IA_XPOS, bt->x, IA_YPOS, bt->y,
		IA_WIDTH, bt->w, IA_HEIGHT, bt->h, 
		IA_CLICK_HOOK, bt_activate,
		IA_PASS_HOOK, lui_highlight,
		IA_DATA, bt, IA_ID, bt->id,
		IA_DELAY, 75,
		TAG_END);

	src.x = bt->x;
	src.y = bt->y;
	src.w = bt->w;
	src.h = bt->h;
	dst.x = 0;
	dst.y = 0;
	SDL_BlitSurface(bt->screen, &src, bt->store, &dst);

	if(!bt->ia) {
		bt_free(bt);
		return NULL;
	}
	bt->destroy = bt_free;
	bt->activate = bt_activate;
	bt->draw = bt_draw;
	bt->highlight = bt_highlight;
	return bt;
}


/* Generic activation routine (show pressed image) */
void bt_activate(lui_bt *bt)
{
	lui_input	input;

//	if(bt->flags & LUIF_PUSHED) {
//		bt->flags ^= LUIF_PUSHED;
//		return;
//	}

//	bt->flags |= LUIF_PUSHED;
	if(bt->type == GAD_TOGGLE) {
		bt->state = !bt->state;
		input.type = bt->state;
		if(bt->onactivate) bt->onactivate(bt, &input);
		if(bt->state) {
			if(bt->click) bt_blit(bt, bt->click);
			if(bt->tileset) bt_tile(bt, BT_CLICKIMAGE);
		} else {
			bt->draw(bt);
		}
		return;
	}
	if(bt->click) bt_blit(bt, bt->click);
	if(bt->tileset) bt_tile(bt, BT_CLICKIMAGE);
	if(bt->onactivate) bt->onactivate(bt, NULL);
}

void bt_draw(lui_bt *bt)
{
	if((bt->type == GAD_TOGGLE) && (bt->state)) {
		if(bt->click) bt_blit(bt, bt->click);
		if(bt->tileset) bt_tile(bt, BT_CLICKIMAGE);
		return;
	}
	if(bt->image) {
		bt_blit(bt, bt->image);
		return;
	}
	if(bt->tileset) bt_tile(bt, BT_IMAGE);
}


/* Switch on highlighting */
void bt_highlight(lui_bt *bt)
{
	if((bt->type == GAD_TOGGLE) && (bt->state)) {
		if(bt->hlclick) bt_blit(bt, bt->hlclick);
		if(bt->tileset) bt_tile(bt, BT_HLCLICKIMAGE);
		return;
	}
	if(bt->hlimage) bt_blit(bt, bt->hlimage);
	if(bt->tileset) bt_tile(bt, BT_HLIMAGE);
}

void bt_blit(lui_bt *bt, SDL_Surface *surface)
{
	SDL_Rect	dst;

	dst.x = bt->x;
	dst.y = bt->y;
	SDL_BlitSurface(surface, NULL, bt->screen, &dst);
}


void bt_tile(lui_bt *bt, int n)
{
	int	tile=0;

	switch(n) {
	case BT_IMAGE:
		tile = bt->tile;
		break;
	case BT_HLIMAGE:
		tile = bt->hltile;
		break;
	case BT_CLICKIMAGE:
		tile = bt->ctile;
		break;
	case BT_HLCLICKIMAGE:
		tile = bt->hlctile;
		break;
	}
	xit_draw(bt->tileset, tile, bt->screen, bt->x, bt->y);
}


void bt_free(lui_bt *bt)
{
	if(bt->store) SDL_FreeSurface(bt->store);
	if(bt->ia) iarea_free(bt->ia);
	if(bt->image) SDL_FreeSurface(bt->image);
	if(bt->hlimage) SDL_FreeSurface(bt->hlimage);
	if(bt->click) SDL_FreeSurface(bt->click);
	if(bt->hlclick) SDL_FreeSurface(bt->hlclick);
	if(bt->name) free(bt->name);
	free(bt);
}
