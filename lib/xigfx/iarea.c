/*
 * Input areas.
 * Clickable areas for GUIs etc.
 * Simple, stupid approach.
 *
 * When a mousebutton is pressed, all inputareas that are set
 * to respond to that button should be called if they encompass
 * the mousepointer's hotspot. Alternative controls may also be
 * through joysticks or joypads. These will mostly work like the
 * mouse does, but with more possible buttons.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: iarea.c,v 1.3 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: iarea.c,v $
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
 * Revision 1.4  2001/12/19 09:01:01  nny
 * Pruned logs.
 *
 * Revision 1.3  2001/12/14 08:42:17  nny
 * Simplified iarea_check() to check only one inputarea at a time. Looping
 * to check more will have to be done in the calling routine. Various
 * changes were made to views and iareas to aid the creation of buttons in
 * the forthcoming button system.
 *
 */

#include "stdlib.h"

#include "SDL.h"
#include "iarea.h"
#include "xitags.h"

/**
 Create a new inputarea.
@doc
 An inputarea is a spot on the screen that will respond
 to mouse/joystick buttons when the cursor is within its
 borders. The LUI system uses this for its gadgets.

 Freeing an inputarea is a simple matter of removing it
 from its list and calling iarea_free().

 <h2>Supported tags</h2>

 <ul>
 <li>IA_DATA</li>
 Data to pass on to the hook functions.

 <li>IA_CLICK_HOOK</li>
 Left mousebutton hook.

 <li>IA_MCLICK_HOOK</li>
 Middle mousebutton hook.

 <li>IA_RCLICK_HOOK</li>
 Right mousebutton hook.

 <li>IA_PASS_HOOK</li>
 Pass-over hook. Called when no mousebuttons are pressed.

 <li>IA_INPUT</li>
 A mask of controllers to respond to. The choices are:
 <ul>
 <li>IA_CTL_MOUSE</li>
 Ordinary mouse, two or three buttons (unless you suffer from a Mac).
 <li>IA_CTL_JOY</li>
 Sticks and pads may break my code. Not implemented yet.
 </ul>

 <li>IA_MBUTTONS</li>
 Mask of mousebuttons to respond to.

 <li>IA_JBUTTONS</li>
 Mask of joybuttons to respond to. Note that only eight bits
 are available. Since joystick support hasn't been written
 yet, this will probably be expanded to include hats, too.
 And we could always hope that Z axis and
 throttle makes its way into the code..

 <li>IA_XPOS, IA_YPOS</li>
 Position of upper left corner of this inputarea.

 <li>IA_WIDTH, IA_HEIGHT</li>
 Size of this inputarea. Clicks/pass-over functions are
 not called outside its limits. Beware of overlapping
 regions. No effort will be made to handle overlaps.

 <li>IA_NAME</li>
 Optional name.

 <li>IA_ID</li>
 Identifier returned by iarea_check() when
 the inputarea matches.

 <li>IA_DELAY</li>
 Milliseconds to delay before allowing an inputarea
 to be clicked again. If less than this number of
 milliseconds have passed since clicking a button,
 iarea_check() returns zero. SDL_GetTicks() is called
 to get current ticks iff this tag has been specified.
 </ul>
 
@param tag1 First of a list of tags to specify settings.
@return A new inputarea with all flags set according to
 the input tags.
@author
@see iarea_check(), iarea_free()
*/
inputarea *iarea_new(ulong tag1, ... )
{
	XI_VARARG(iarea_newA((tagitem *)&tag1), inputarea)
}

inputarea *iarea_newA(tagitem *taglist)
{
	inputarea	*ia;
	tagitem	*t;

	ia = calloc(1, sizeof(inputarea));
	if(!ia)
		return NULL;

	memset(ia, 0, sizeof(inputarea));
	t = tag_next(&taglist);
	while(t) {
		switch(t->tag) {
		case IA_DATA:
			ia->data = (void *)t->data;
			break;
		case IA_CLICK_HOOK:
			ia->lmbfunc = (void *)t->data;
			break;
		case IA_MCLICK_HOOK:
			ia->mmbfunc = (void *)t->data;
			break;
		case IA_RCLICK_HOOK:
			ia->rmbfunc = (void *)t->data;
			break;
		case IA_PASS_HOOK:
			ia->pfunc = (void *)t->data;
			break;
		case IA_XPOS:
			ia->x = t->data;
			break;
		case IA_YPOS:
			ia->y = t->data;
			break;
		case IA_WIDTH:
			ia->w = t->data;
			break;
		case IA_HEIGHT:
			ia->h = t->data;
			break;
		case IA_INPUT:
			ia->inputs = t->data;
			break;
		case IA_MBUTTONS:
			ia->mb = (Uint8)t->data;
			break;
		case IA_JBUTTONS:
			ia->jb = (Uint8)t->data;
			break;
		case IA_NAME:
			ia->name = strdup((char *)t->data);
			break;
		case IA_ID:
			ia->id = t->data;
			break;
		case IA_DELAY:
			ia->delay = t->data;
			break;
		}
		t = tag_next(&taglist);
	}
	return ia;
}

/**
 Checks an inputarea against cursor position and controller buttons.
@doc
 If the supplied inputarea accepts the mouse/joybuttons currently
 pressed, the corresponding hook is called. An inputarea can respond
 to left, right and middle mousebutton for now.

 If no buttons are pressed, the IA_PASS_HOOK function
 is called instead.

 The ID is still returned if there is a delay
 that hasn't been finished yet.

 Big fat FIXME: Xiqual doesn't currently actually initialise other
 controllers (joypads and sticks). Furthermore, the parameters
 should be expanded to allow for the hat.
@param ia Inputarea to compare against cursor position
@param curx Current cursor x coordinate
@param cury Current cursor y coordinate
@param mousestate Mask of currently pressed mousebuttons
@param joystate Mask of currently pressed stick/pad buttons
@return The inputarea's ID if a match was found.
@author Ronny Bangsund
@see iarea_new()
*/
ulong iarea_check(inputarea *ia, int curx, int cury, Uint8 mousestate, Uint8 joystate)
{
	Uint32	now;

	if((ia->inputs & IA_CTL_MOUSE)) {
		if((curx >= ia->x) && (cury >= ia->y) && (curx <= ia->x+ia->w) && (cury <= ia->y+ia->h)) {
			/* If we're not done with the delay, return.
			 * This should work for zero-value ia->tick, too.
			 */
			now = SDL_GetTicks();
			if((ia->delay) && (now - ia->tick < ia->delay)) return ia->id;

			if(mousestate & IA_LMB) {
				if(ia->lmbfunc) {
					ia->lmbfunc(ia->data);
				}
			}
			if(mousestate & IA_MMB) {
				if(ia->mmbfunc) {
					ia->mmbfunc(ia->data);
				}
			}
			if(mousestate & IA_RMB) {
				if(ia->rmbfunc) {
					ia->rmbfunc(ia->data);
				}
			}
			if(!(mousestate & (IA_LMB | IA_MMB | IA_RMB)))  {
				if(ia->pfunc) {
					ia->pfunc(ia->data);
				}
			}
			ia->tick = SDL_GetTicks();
			return ia->id;
		}
	}
	if((ia->inputs & IA_CTL_JOY)) {
		if((curx >= ia->x) && (cury >= ia->y) && (curx <= ia->x+ia->w) && (cury <= ia->y+ia->h)) {
			printf("Joybutton: 0x%x\n", joystate);
			return ia->id;
		}
	}
	return 0;
}

/**
 Free an inputarea.
@doc
 Since some inputareas may have a name,
 you should call this to free all data.
@author Ronny Bangsund
@see iarea_new()
*/
void iarea_free(inputarea *ia)
{
	if(!ia) return;

	if(ia->name) free(ia->name);
	free(ia);
}
