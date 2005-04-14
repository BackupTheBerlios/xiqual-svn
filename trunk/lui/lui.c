/*
 * Lame User Interface
 * Core functions.
 */

#include "SDL_image.h"

#include "lui_button.h"
#include "lui_list.h"
#include "lui_msg.h"
#include "xitags.h"
#include "xiqual.h"


/* XPM */
static char * mesh_xpm[] = {
	"4 4 3 1",
	" 	c None",
	".	c #000000",
	"+	c #0000FF",
	".+.+",
	"+.+.",
	".+.+",
	"+.+."
};

/**@name LUI: Core functions
 <h1>LUI:the Lame User Interface</h1>

 <p>To use LUI gadgets, you should pass the XI_LUI tag to xi_init()
 when starting your program. Only buttons and lists are
 possible to create yet, but more is to come in a few days.</p>

 <p>All gadgets created can be added to the global lists for
 automatic handling with lui_addgadget(). lui_remgadget()
 can be used if you want to remove anything before exit
 (LUI frees all gadgets automatically, though). All the
 gadgets have several "methods" that can be called for
 various effects, but these are mostly internal. Of most
 interest are the destroy() and draw() methods.</p>

 <h2>Common tags</h2>
 <p>When creating a gadget, you need to pass many settings
 to get it rendered properly. Here's the list of tags
 which all gadgets support:</p>

 <ul>
 <li>LUI_ACTIVATEHOOK</li>
 <p>Function to call upon click activation (normally left mousebutton).</p>

 <li>LUI_PASSHOOK</li>
 <p>Function called when the cursor hovers above a gadget.
 The hook will already have its highlighting done around
 the time this hook is called.</p>

 <li>LUI_NAME</li>
 <p>Optional name for searching, grouping gadgets or whatever you need.</p>

 <li>LUI_TITLE</li>
 <p>Usable by controls to display a title, if it has any.</p>

 <li>LUI_XPOS, LUI_YPOS, LUI_WIDTH, LUI_HEIGHT</li>
 <p>Position and size of gadget. Size is optional, as it is
 usually derived from the images that make up the gadget.</p>

 <li>LUI_SCREEN</li>
 <p>SDL surface to display to. This may be any valid SDL surface
 that can display the images you've passed in to create
 the gadgets previously. It's sometimes useful to render
 to a view for gadgets that may not always be visible.
 (Special controls will be made available to make disabling
 groups of gadgets easier)</p>
 </ul>

@author Ronny Bangsund
@see bt_new(), lst_new()
*/

/*@{*/

/**
 Initialise the Lame User Interface.
@doc
 <p>Required images are loaded, and any structures
 that need to be set up are prepared. Note: It is
 still possible to use LUI if this function fails,
 but important graphics will be missing.</p>

 <p>Only the "ghosting" mesh is loaded at the moment.
 If you want to replace the mesh with one of your
 own design, you can just load an image and set
 the colourkey/alpha level you want.</p>
@return Non-zero if GUI system is fully usable.
@author Ronny Bangsund
@see gadget, bt_new(), lst_new(), lui_quit()
*/
int lui_init()
{
	SDL_Surface	*tmp;

	tmp = IMG_ReadXPMFromArray(mesh_xpm);
	if(!tmp) return 0;

	xig->mesh = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	if(!xig->mesh) return 0;

	SDL_SetColorKey(xig->mesh, SDL_SRCCOLORKEY|SDL_RLEACCEL,
		SDL_MapRGB(xig->screen->format, 0, 0, 255));
	return 1;
}

/**
 Finish using the Lame User Interface.
@doc
 All gadgets and inputareas will be freed,
 saving you from the work.
@author Ronny Bangsund
@see lui_init(), lui_addgadget(), lui_remgadget()
*/
void lui_quit()
{
	gadget	*g;
	inputarea	*ia;

	if(xig->mesh) {
		SDL_FreeSurface(xig->mesh);
		xig->mesh = 0;
	}
	while(xig->gadgets.head) {
		g = (gadget *)xig->gadgets.head;
		lui_remgadget(g);
		g->destroy(g);
	}
	while(xig->inputs.head) {
		ia = (inputarea *)xig->inputs.head;
		list_remove(&xig->inputs, (Node *)ia);
		iarea_free(ia);
	}
}

/**
 Toggle "ghosted" pattern over a gadget.
@doc
 If the gadget is already disabled, it will
 be reenabled and redrawn.
@author Ronny Bangsund
@see gadget, lui_init()
*/
void lui_ghost(gadget *g)
{
	SDL_Rect	rect;
	int	mw, mh;

	if(g->flags & LUIF_GHOSTED) {
		g->flags ^= LUIF_GHOSTED;
		if(xig->mesh) g->draw(g);
		return;
	}

	g->flags |= LUIF_GHOSTED;

	if(g->ghost) {
		g->ghost(g);
		return;
	}

	if(!xig->mesh) return;

	mw = 0;
	mh = g->h / xig->mesh->h;
	rect.x = g->x;
	rect.y = g->y;
	while(mh) {
		while(mw < (g->w / xig->mesh->w)) {
			SDL_BlitSurface(xig->mesh, NULL, xig->screen, &rect);
			rect.x += xig->mesh->w;
			mw++;
		}
		mw = 0;
		mh--;
		rect.x = g->x;
		rect.y += xig->mesh->h;
	}
}


/* Call the highlight method of a gadget,
 * and set the global pointer to this.
 * Special handling is needed when a message
 * box is shown - if it's not the message
 * box, ignore. Message boxes handle their
 * own highlighting of "children".
 */
void lui_highlight(gadget *g)
{
	if(xig->hl == g) return;

	xig->hl = g;
	g->flags |= LUIF_HIGHLIGHTED;
	/* We don't want a delay after highlighting actions! */
	g->ia->tick -= g->ia->delay;
	if(g->highlight) g->highlight(g);
}


/**
 Add a gadget to the global lists.
@doc
 When using this, xi_main() will handle GUI input automatically.
 You DON'T want to handle it yourself.
@author Ronny Bangsund
@see xi_main(), lui_remgadget(), gadget
*/
void lui_addgadget(gadget *g)
{
	if(!g) return;

	if(g->add) {
		g->add(&xig->inputs, &xig->gadgets, g);
	} else {
		list_add(&xig->gadgets, (Node *)g);
		list_add(&xig->inputs, (Node *)g->ia);
	}
	g->draw(g);
}


/**
 Remove a gadget from the global lists.
@doc
 This does not free a gadget's resources.
 Call the gadget's destroy() method if
 so is desired.
@author Ronny Bangsund
@see lui_addgadet(), gadget
*/
void lui_remgadget(gadget *g)
{
	if(!g) return;

	if(g->rem) {
		g->rem(&xig->inputs, &xig->gadgets, g);
	} else {
		list_remove(&xig->gadgets, (Node *)g);
		list_remove(&xig->inputs, (Node *)g->ia);
	}
}


/**
 Create gadget, and automatically set certain tags.
@doc
 The LBT_TOGGLE tag is set if type is GAD_TOGGLE.
 LUI_SCREEN is also automatically set to xig->screen,
 but any LUI_SCREEN tags you supply will override this.

 <h3>Known button types</h3>
 <ul>
 <li>GAD_BUTTON</li>
 Ordinary clickable button, with optional highlight images.
 The highlight image for the pressed-in state doesn't really
 make much sense, but it would make you able to change a
 button to toggle state on the fly.

 <li>GAD_TOGGLE</li>
 Like a clickable button, except the button stays pressed
 until you click again. The pressed-in highlight image is
 useful for these.

 <li>GAD_LIST</li>
 A very simple list with a field above and below that
 scrolls its contents up and down, respectively.
 It relies on hooks from the up/down buttons to tell
 what item to show as the first one (just a number),
 the its ondraw() hook gets his number and must update
 the list contents.

 <li>GAD_MENU</li>
 Creates a menu header (walks like a button, quacks like
 a button) which pops up a list of further buttons when
 clicked. No layout management is done yet, so the items
 will just appear below eachother. The menu itself handles
 highlighting of the menuitems. Its inputarea changes size
 temporarily while the menuitems are displayed.

 <li>GAD_MESSAGE</li>
 This is a modal message box with one (confirm) button.
 For those who don't speak GUI, modal means all other
 gadgets are unavailable while this box awaits input.
 It handles its own highlighting of items, and may
 possibly have a scrolling list of text, if the text
 is too large for its displayable area.

 <li>GAD_CONFIRM</li>
 This is another modal message box, but with both a
 confirm and cancel button.
 </ul>
*/
gadget *lui_newgadget(ulong type, ulong tag1, ... )
{
	XI_VARARG(lui_newgadgetA(type, (tagitem *)&tag1), gadget)
}


gadget *lui_newgadgetA(ulong type, tagitem *taglist)
{
	gadget	*g=0;
	tagitem	t[3];

	t[0].tag = LUI_SCREEN;
	t[0].data = (ulong)xig->screen;
	t[1].tag = TAG_MORE;
	t[1].data = (ulong)taglist;
	t[2].tag = TAG_MORE;
	t[2].data = (ulong)taglist;

	switch(type) {
	case GAD_BUTTON:
		g = (gadget *)bt_newA(t);
		break;
	case GAD_TOGGLE:
		t[1].tag = LBT_TOGGLE;
		t[1].data = 1;
		g = (gadget *)bt_newA(t);
		break;
	case GAD_LIST:
		g = (gadget *)lst_newA(t);
		break;
	case GAD_MENU:
	case GAD_MESSAGE:
	case GAD_CONFIRM:
		break;
	}
	return g;
}

void lui_freegadget(gadget *g)
{
	if((g) && (g->destroy)) g->destroy(g);
}


/*@}*/
