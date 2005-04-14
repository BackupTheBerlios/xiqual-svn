/* Mainloop: Handles cursor, GUI and any input hooks
 * defined by a user.
 */

#include "SDL.h"

#include "xisetup.h"
#include "xiqual.h"

/**@name Input handling
 <h2>Input handling</h2>
 Xiqual has an input loop readymade for your perusal.
 It might not be the best loop in the world, but it
 does save me some work when creating a new SDL application.

 To make use of this, you must pass a main loop of your own
 which merely tests the arguments passed. Xiqual does all
 the dreary work of grabbing mouse, keyboard and joystick state.

 The actual input arguments are documented in the xiglob
 structure (click the link below). They are just the usual
 ones you expect from an SDL application.

 Any LUI gadgets added with lui_addgadget() will also be
 automatically handled in the same thread as xi_main().
 That means highlighting, togglestate, calling activation
 hooks and whatnot.

 Finally, the mouse cursor is handled via xi_main().
 Its view is hidden while processing the mainloop, so
 there is no danger of overlapping anything that way.

 Updates: The current system simply uses SDL_Flip() at the
 end of the loop. I'm not sure how fast this is in the
 average case, but profiling so far has been encouraging.

 Overlapping gadgets: Gadgets are redrawn very rarely.
 If a gadget's alternate (clicked/highlighted) state has
 a very different shape and size from the regular state,
 there is no guarantee the gadget will update if you
 happen to overwrite some of it. The current gadgets
 redraw when switching state, but may try to be more
 "intelligent" in the future.

 So if adding LUI to an application, it's best to keep
 the gui on its own side of the screen, or even draw
 it to a view. The ability to make any SDL surface a
 gadget's main output is part of the reason why no
 updates are happening via SDL_UpdateRect(). At least
 I can brag about how object-oriented this interface
 is.

@author Ronny Bangsund
@see xi_init(), xi_quit(), lui_init(), lui_quit(), xiglob
*/
/*@{*/

/**
 Handle input and GUI.
@doc
 Update mouse cursor (if you supplied a new one), pump the input
 queue to get key/mouse/joystate and update the LUI subsystem's
 gadgets. If a gadget is activated, all those things are handled.

 Use xi_init() or xi_inittag() to set an
 XI_MAINHOOK tag pointing to your input handler.

 The hook will be called with a pointer to a keystate, a modstate and a
 mousestate (Uint8 *, SDLMod and Uint8, respectively), and two integers
 (int) with the current mouse coordinates.

 Note: You'll have to use SDL's joystick functions yourself in this main loop
 to read any joysticks, as Xiqual currently has no code relating to joysticks.
 This might change if anyone ever wants even easier handling.

 Warning: Do not run this function if you haven't initialised
 Xiqual with xi_init() yet.
@author Ronny Bangsund
@see xi_init(), xi_inittag(), xi_quit()
*/
int xi_main()
{
	Uint8	*keystate, mousestate;
	SDLMod	modstate;
	int	mousex, mousey;
	inputarea	*ia;
	ulong	id;

	SDL_PumpEvents();
	keystate = SDL_GetKeyState(NULL);
	modstate = SDL_GetModState();
	mousestate = SDL_GetMouseState(&mousex, &mousey);

	if(xig->cursor) {
		view_hide(xig->cursor);
		xig->cursor->x = mousex - xig->hotx;
		xig->cursor->y = mousey - xig->hoty;
	}

	if(xig->hl) {
		if(iarea_check(xig->hl->ia, mousex, mousey, mousestate, 0)==0) {
			if(xig->hl->highlight) xig->hl->draw(xig->hl);
			xig->hl = 0;
		}
	}

	if(xig->mainloop) {
		xig->mainloop(keystate, modstate, mousestate, mousex, mousey);
	}

	ia = (inputarea *)xig->inputs.head;
	id = 0;
	while((id == 0) && ia) {
		id=iarea_check(ia, mousex, mousey, mousestate, 0);
		if(!id) ia = ia->next;
	}

	if(xig->cursor) view_show(xig->cursor);

	SDL_Flip(xig->screen);
	return 0;
}

// FIXME: Check for SDL_VIDEORESIZE events

/*@}*/
