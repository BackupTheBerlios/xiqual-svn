#include "lui_msg.h"
#include "xiqual.h"


lui_msg *msg_boxnew(ulong tag1, ...)
{
	XI_VARARG(msg_boxnewA((tagitem *)&tag1), lui_msg)
}


lui_msg *msg_boxnewA(tagitem *taglist)
{
	tagitem	*t;
	lui_msg	*m;
	char	*bg=0;

	m = calloc(1, sizeof(lui_msg));
	if(!m) return NULL;

	m->id = -1;
	m->type = GAD_MESSAGE;
	t = tag_next(&taglist);
	while(t) {
		switch(t->tag) {
		case LUI_NAME:
			m->name = strdup((char *)t->data);
			break;
		case LUI_ID:
			m->id = t->data;
			break;
		case LUI_DATA:
			m->data = (void *)t->data;
			break;
		case LUI_ACTIVATEHOOK:
			m->onactivate = (void *)t->data;
			break;
		case LUI_XPOS:
			m->x = t->data;
			break;
		case LUI_YPOS:
			m->y = t->data;
			break;
		case LUI_WIDTH:
			m->w = t->data;
			break;
		case LUI_HEIGHT:
			m->h = t->data;
			break;
		case LUI_SCREEN:
			m->screen = (SDL_Surface *)t->data;
			break;
		case MSG_MESSAGE:
			m->message = strdup((char *)t->data);
			break;
		case MSG_BACKGROUND:
			bg = (char *)t->data;
			break;
		case MSG_BORDERWIDTH:
			m->bw = t->data;
			break;
		case MSG_BORDERHEIGHT:
			m->bh = t->data;
			break;
		}
		t = tag_next(&taglist);
	}
	if(!bg) {
		msg_free(m);
		return NULL;
	}
	m->bg = view_new(XIVT_SCREEN, m->screen,
		XIVT_XPOS, m->x, XIVT_YPOS, m->y,
		XIVT_WIDTH, m->w, XIVT_HEIGHT, m->h,
		XIVT_IMAGE, bg,
		XIVT_FLAGS, XIV_NOKEY, TAG_END);

	if(!m->bg) {
		msg_free(m);
		return NULL;
	}
	m->w = m->bg->w;
	m->h = m->bg->h;
	m->destroy = msg_free;
	m->draw = msg_draw;
	return m;
}


/* Draw contents of message box.
 * Not sure how this should be handled,
 * but it's probably best to let the user
 * regenerate the contents as needed.
 */
void msg_draw(lui_msg *m)
{
	SDL_Rect rect;

	if(m->fg) {
		rect.x = m->bg->w - m->fg->w;
		if(rect.x) rect.x /= 2;
		rect.y = m->bg->h - m->fg->h;
		if(rect.y) rect.y /= 2;
	}
	/* If we have a text-string, generate the
	 * message on top of the view from a font.
	 */
	if(m->message) {
	}
	msg_show((lui_msg *)m->bg, 1);
}


/**
 Show or hide a message box.
@doc
 if show is TRUE:
 <p>The list will handle its own gadgets, while the
 rest of the GUI is blocked. Other input will
 still be handled.

 If FALSE:
 <p>The box is hidden from view, and normal gadget
 handling is resumed.
@author Ronny Bangsund
@see msg_boxnew()
*/
void msg_show(lui_msg *m, int show)
{
	if(show) {
		if((xig->msg) && (xig->msg != m)) {
			view_hide(xig->msg->bg);
		}
		view_show(m->bg);
		xig->msg = m;
	} else {
		view_hide(m->bg);
		xig->msg = 0;
	}
}


void msg_free(lui_msg *m)
{
	if(!m) return;

	if(m->name) free(m->name);
	if(m->message) free(m->message);
	if(m->bg) view_free(m->bg);
	if(m->fg) SDL_FreeSurface(m->fg);
	if(m->ok) SDL_FreeSurface(m->ok);
	if(m->cancel) SDL_FreeSurface(m->cancel);
	free(m);
}
