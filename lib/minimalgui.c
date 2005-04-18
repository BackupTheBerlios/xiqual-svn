/**@name minimalgui
 A very minimal example of LUI use.
@doc
 This example shows how to start and end a Xiqual program
 with LUI gadgets. The user interface runs itself once
 the gadgets are created, thanks to xi_main(). One button
 is created with a hook to set an exit flag. Xiqual has
 no way to know when to quit otherwise.
@author Ronny Bangsund
*/

/**
*/

#include "lui_button.h"
#include "lui_list.h"
#include "xiqual.h"

static ulong running=1;
static xitiles *tiles;

void clickquit(gadget *g)
{
	printf("Gadget id %ld\n", g->id);
	running = 0;
}

void drawlist(int first, SDL_Surface *s, xitiles *tileset)
{
	int	x=0, y=0;
	ulong	count;

	SDL_FillRect(s, NULL, 0);
	count = first;
	while(count < tileset->count) {
		xit_draw(tileset, count, s, x, y);
		count++;
		x += tileset->w;
		if(x >= s->w) {
			x = 0;
			y += tileset->h;
		}
		if(y >= s->h) return;
	}
}

/* When the example list is clicked within its
 * borders, this hook/method is called.
 * The first pointer is a small structure that
 * contains the coordinates of the mouse
 * relative to the gadget, adjusted by borders.
 * This means that 0,0 is in the upper left
 * inside the border.
 */
void clicklist(lui_input *input, lui_list *lst)
{
	ulong	tilenum;
	xitiles	*t;

	t = (xitiles *)lst->data;
	if(!t) return;

	/* Calculate what tile the position corresponds to.
	 * The formula expects the displayable width inside
	 * the borders to be exactly the width of a whole
	 * number of gadgets. If you add, say, 16 pixels to
	 * this particular gadget, you'll find half-tiles
	 * blitted in this extra bit. But for this example,
	 * there's no need to fix it. The fix is easy enough,
	 * if you want to do it.
	 */
	tilenum = (lst->view->w / t->w) * (input->y / t->h) +
		(input->x / t->w) + lst->first;
	if(tilenum < t->count) {
		t->dst.x = 0;
		t->dst.y = 0;
		SDL_FillRect(xig->screen, &t->dst, 0);
		xit_draw(t, tilenum, xig->screen, 0, 0);
	}
}

/* For the basic LUI list:
 * Inputs are the number of the topmost/leftmost
 * currently displayed item and some data (in this
 * particular case a tileset pointer).
 * Calculate whether clicking up/down should
 * actually change anything, and return the new
 * number.
 */
int uplist(ulong first, xitiles *tileset)
{
	if((first > 1) && (tileset->count)) return first-2;
	return 0;
}

int downlist(ulong first, xitiles *tileset)
{
	if(first + 2 < tileset->count) return first+2;
	return tileset->count-1;
}

/* Passing this to xi_main() with the XI_MAINHOOK
 * gives you somewhat better control of input.
 * This is also where you check the keyboard.
 * xi_main() does the bare minimum to pump events,
 * get state, update mouse and GUI, then call this.
 * During the custom main function, the mouse is
 * hidden from view, but gadgets are still active.
 */
void realmain(Uint8 *keystate, SDLMod modstate,
	Uint8 mousestate, int mousex, int mousey)
{
	if(keystate[SDLK_ESCAPE]) running = 0;

	if(modstate) printf("Modstate: 0x%x\n", modstate);

	/* Right mousebutton will disable the list */
	if(mousestate & IA_RMB_MASK) {
		lui_ghost((gadget *)xig->gadgets.head->next);
		SDL_Delay(75); /* Artificial delay till I fix timing.. */
	}
}

int main(int argc, char *argv[])
{
	if(argc || argv) argc = 1;

	if(!xi_init(XI_WIDTH, 800, XI_HEIGHT, 600, XI_DEPTH, 16,
		XI_CURSORFILE, "gfx/cursor.png", XI_CURSOR_HOTX, 2, XI_CURSOR_HOTY, 1,
		XI_LUI, 1, XI_MAINHOOK, realmain, TAG_END)) {
		fprintf(stderr, "Error (graphics/audio subsystem): %s\n", xi_lasterror);
		return 2;
	}
	atexit(xi_quit);

	tiles = xit_loadtiles("sprite/narya.iff");
	if(!tiles) {
		fprintf(stderr, "Can't find sprite/narya.iff\n"
			"Use tileset to create it.\n");
		return 1;
	}
	/* Make a clickable button appear near the
	 * middle of the screen. The function clickquit()
	 * will be called when it's clicked.
	 * Three images are used - normal state,
	 * mouseover (highlighted) state and clicked state.
	 */
	lui_addgadget(lui_newgadget(GAD_BUTTON,
		LUI_XPOS, 400, LUI_YPOS, 300,
		LBT_IMAGE, "gfx/quit.png",
		LBT_HLIMAGE, "gfx/quit_hl.png",
		LBT_CLICKIMAGE, "gfx/quit_b.png",
		LUI_ID, 42, LUI_ACTIVATEHOOK, clickquit,
		TAG_END));

	/* A list of icons in the upper right corner */
	lui_addgadget((gadget *)lst_new(LUI_SCREEN, xig->screen,
		LUI_XPOS, 700, LUI_YPOS, 20,
		/* The bordersize of tilebox.png */
		LST_BORDERWIDTH, 8, LST_BORDERHEIGHT, 8,
		/* The list will be as large as this image */
		LST_IMAGE, "gfx/tilebox.png",
		/* This function selects items */
		LUI_ACTIVATEHOOK, clicklist,
		/* This fills the list with images */
		LST_DRAWHOOK, drawlist,
		/* These functions calculate what to display topmost */
		LST_UPHOOK, uplist, LST_DOWNHOOK, downlist,
		/* The list shows these tiles (two per row) */
		LUI_DATA, tiles,
		/* Only a clickimage is supported */
		LST_UPCLICKIMAGE, "gfx/tileboxup.png",
		LST_DOWNCLICKIMAGE, "gfx/tileboxdown.png",
		LUI_ID, 23, TAG_END));

	while(running) {
		xi_main();
	}
	xit_freetiles(tiles);
	return 0;
}
