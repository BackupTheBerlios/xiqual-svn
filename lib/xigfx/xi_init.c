/* Hairy setup code - initialise SDL, and possibly the mixer,
 * based on your supplied tags.
 */

#if defined(HAVE_CONFIG_H)
#	include "config.h"
#endif

#if defined(HAVE_SDL_SDL_MIXER_H)
#	include "SDL_mixer.h"
#endif
#if defined(HAVE_SDL_SDL_TTF_H)
#	include "SDL_ttf.h"
#endif

#include "lui.h"
#include "prefs.h"
#include "xios.h"
#include "xiqual.h"
#include "xisetup.h"

/* This is the pointer to the global data.
 * Use xiqual_getglob() to get a pointer to
 * it, or save the result from xiqual_init().
 */
xiglob	*xig=0;

static void xi_release();


/**@name Setting up graphics
 When using Xiqual to initialise SDL, several subsystems can be
 started at once, including a few external libraries.

 The default initialisation start sound, graphics and input.
 If SDL_mixer and SDL_ttf are available, they are also opened.
 When xi_init() returns successfully, you can start straight away
 to draw on the screen in the global structure, make noise with
 Mix_* functions and open TTF fonts (plus other formats supported
 by this library).

 The global structure holds the actual audio format specs.

 If you want to use the LUI interface subsystem, pass the
 tag XI_LUI to xi_init() with any data. If XIF_USE_LUI is
 set in the global structure's flags, full use of LUI is
 available. It is very unlikely to fail, as all lui_init()
 does is to allocate a surface for a 4x4 pixmap.

 Unlike kicking a heroin or tobacco addiction, quitting Xiqual
 is very easy. Just call xi_quit() to have SDL, SDL_ttf, SDL_mixer
 and LUI subsystems deallocate their resources. All gadgets
 and other resources created with Xiqual are freed.
@see xiglob, lui_init()
*/


/*@{*/
/**
 Initialise Xiqual and all SDL subsystems.
@doc
 The tags supplied allow you to selectively initialise some SDL subsystems,
 create a mousepointer from an SDL_image supported file and more.

 To intialise all supported SDL libraries and create the xiglob structure,
 with an 800x600x16 display:

 xi_init(XI_WIDTH, 800, <BR>XI_HEIGHT, 600, <BR>XI_DEPTH, 16, TAG_END);

 This creates a mouse cursor, and tells Xiqual to also handle the cursor,
 in addition to opening a typical display:

 xi_init(XI_WIDTH, 800,<BR>XI_HEIGHT, 600,<BR>XI_DEPTH, 16,
	<BR>XI_CURSORFILE, "cursor.png",<BR>TAG_END);

 <h2>Supported tags</h2>

 <h3>Xiqual settings and hooks<h3>
 <ul>
 <li>XI_MAINHOOK</li>
 A pointer to a function to be called at each input poll.
 See xisetup.h/xi_main() for information on the arguments passed.

 <li>XI_PREFS</li>
 Load this configuration file. This is currently only for convenience,
 but Xiqual may start looking for a section of its own for various
 settings in the future (type of SDL audio/video drivers to use etc.).

 It will first look for $HOME/.xiqual/<filename>, then it
 will look in the current directory. Win32 will just look
 in the current directory.

 If this file isn't found, Xiqual attempts to load
 $HOME/.xiqual/xiqualrc, followed by xiqualrc in the
 current directory. Win32 once again gets the short
 end of the path.
 </ul>

 <h3>SDL settings</h3>
 <ul>
 <li>XI_WIDTH, XI_HEIGHT</li>
 Screensize. Will be emulated if using an odd resolution, or if
 it's not available according to the XF86Config (for example, 320x240
 is usually not defined as available for 16-bit resolutions).
 This WILL LOOK CRUDDY.

 <li>XI_DEPTH</li>
 Bitplane depth (note that SDL may emulate this).

 <li>XI_SDLFLAGS</li>
 If you need to initialise threading, or only a few SDL subsystems,
 pass the replacement flags here. These will be used instead of
 the defaults, which is SDL_INIT_EVERYTHING.

 <li>XI_SCRFLAGS</li>
 Xiqual will default to opening the screen with flags
 SDL_SWSURFACE | SDL_DOUBLEBUF, so pass a different set of flags
 here if that isn't what you want. Note that many other Xiqual
 functions also rely on software buffers for blits, and these
 can't be changed (yet). NVidia cards blit faster on software, too.

 <li>XI_CURSOR</li>
 You may pass a pointer to an xiview with this tag.
 Xiqual frees this structure on exit.

 <li>XI_CURSORFILE</li>
 This tag is for those who prefer to create a mouse
 cursor image with more colours. A view is created, and will
 be freed on exit, like XI_CURSOR.

 <li>XI_CURSORKEY</li>
 Pass a colourkey (as returned from SDL_MapRGB()) here if you also
 specified a filename to load a cursor from. If specifying a cursor
 directly, that view's key will be used.

 <li>XI_CURSOR_HOTX, XI_CURSOR_HOTY</li>
 This is the location of the "hotspot", the point on the
 cursor image that is considered the active (selecting) part.
 The image will be drawn at mouse coordinates minus these coordinates.
 </ul>

 <h3>SDL_mixer settings</h3>
 <ul>
 <li>XI_MIXCHAN</li>
 Maximum number of total mixing channels. Defaults to eight.

 <li>XI_AUDIORATE</li>
 Bitrate (frequency) to use for mixed audio, if SDL_mixer is available.
 Defaults to 22050Hz.

 <li>XI_AUDIOCHAN</li>
 Defines type of audio for music - 1 for mono, 2 for stereo.
 See XI_MAXCHAN, if you're confused.

 <li>XI_AUDIOBUFSIZE</li>
 Size of audio buffers - too small or too big may cause stuttering or
 unexpected pauses on older systems and bad sound hardware.
 Note: Must be a multiple of 16.

 <li>XI_AUDIO_FMT</li>
 Audio format to use for audio mixing. May be one of:
 <ul>
 <li>AUDIO_U8</li> Unsigned 8-bit.
 <li>AUDIO_S8</li> Signed 8-bit.
 <li>AUDIO_U16LSB</li> 16-bit unsigned (Intel format).
 <li>AUDIO_S16LSB</li> 16-bit signed (Intel format). This is the default for WAV.
 <li>AUDIO_U16, AUDIO_S16</li> Shorthand for the two Intel formats.
 <li>AUDIO_U16MSB</li> Big-endian unsigned 16-bit.
 <li>AUDIO_S16MSB</li> Big-endian signed 16-bit.
 <li>AUDIO_U16SYS</li> 16-bit audio of whatever is the native byte-order.
 <li>AUDIO_S16SYS</li> 16-bit signed audio of native byte-order.
 </ul>
 </ul>

 <h3>SDL TTF settings</h3>
 <ul>
 <li>XI_TTF</li>
 Set data to non-zero to enable SDL_ttf initialisation.
 </ul>

 <h3>LUI settings</h3>
 <ul>
 <li>XI_LUI</li>
 Set data to non-zero to enable LUI initialisation.
 </ul>

@author Ronny Bangsund
@see xi_inittag(), xi_main(), xi_quit(), SDL_Init(), Mix_OpenAudio(),
 lui_init(), xiglob
*/
xiglob *xi_init(ulong tag1, ... )
{
//FIXME: Should really create a .xiqual directory in the user's homedir
	XI_VARARG(xi_initA((tagitem *)&tag1), xiglob)
}


xiglob *xi_initA(tagitem *tags)
{
	/* What, user tried to call this twice?! */
	if(xig)
		return NULL;

	xig = calloc(1, sizeof(xiglob));
	if(!xig) return NULL;

	xi_inittagA(tags);
	return xig;
}


void xi_inittagA(tagitem *tags)
{
	tagitem	*t;
	int	w=0, h=0, d=0;	/* Screen settings */
	Uint32	sdlflags, scrflags, cursorkey=0;
	char	*cursorname=0;
	char	home_prefs[2000];

	/* Just some precautions */
	if(!(xig || tags)) {
		return;
	}

	sdlflags = SDL_INIT_EVERYTHING; /* All subsystems, no threads, parachute ON */
	 /* FIXME: Check for availability of hardware surfaces?
	  * Someone on #SDL noted that NVidia cards do software surfaces
	  * lots faster. Well, I have NVidia, so let's try this..
	  */
	scrflags = SDL_SWSURFACE | SDL_DOUBLEBUF;
//FIXME: Check for SDL_OPENGL and SDL_OPENGLBLIT in user-defined
// settings - set up a nice default GL screen if present
#if defined(HAVE_SDL_SDL_MIXER_H)
	/* Set defaults to the most common format (WAV-style, little-endian signed 16-bit) */
	xig->audio_rate = 44100;	/* We opt for hiqh quality as a default..tags are your friends */
	xig->audio_chan = 2;
	xig->audio_bufsize = 4096;	/* Must be a power of 2 */
	xig->audio_fmt = AUDIO_S16;
#endif

	t = tag_next(&tags);
	while(t) {
		switch(t->tag) {
		case XI_WIDTH:
			w = t->data;
			break;
		case XI_HEIGHT:
			h = t->data;
			break;
		case XI_DEPTH:
			d = t->data;
			break;
		case XI_SDLFLAGS:
			sdlflags = t->data;
			break;
		case XI_SCRFLAGS:
			scrflags = t->data;
			break;
		case XI_CURSOR:
			xig->cursor = (xiview *)t->data;
			break;
		case XI_CURSORFILE:
			cursorname = (char *)t->data;
			break;
		case XI_CURSOR_HOTX:
			xig->hotx = t->data;
			break;
		case XI_CURSOR_HOTY:
			xig->hoty = t->data;
			break;
		case XI_CURSORKEY:
			cursorkey = t->data;
		case XI_MAINHOOK:
			xig->mainloop = (void *)t->data;
			break;
		case XI_PREFS:
#if defined(WIN32)
			/* Cheap-ass solution for now */
			snprintf(home_prefs, 2000, "%s", (char *)t->data);
#else
			snprintf(home_prefs, 2000, "%s/.xiqual/%s",
				getenv("HOME"), (char *)t->data);
#endif
			if(file_getsize(home_prefs)) {
				xig->prefs = cfg_loadprefs(PREFS_NAME, home_prefs, TAG_END);
			} else {
				xig->prefs = cfg_loadprefs(PREFS_NAME, (char *)t->data, TAG_END);
			}
			break;
#if defined(HAVE_SDL_SDL_MIXER_H)
		case XI_AUDIORATE:
			xig->audio_rate = t->data;	/* Bitrate (frequency) */
			break;
		case XI_AUDIOCHAN:
			xig->audio_chan = t->data;	/* 1 for mono, 2 for stereo? */
			break;
		case XI_AUDIOBUFSIZE:
			xig->audio_bufsize = t->data;
			break;
		case XI_AUDIO_FMT:
			xig->audio_fmt = (Uint16)t->data;	/* See SDL_mixer.h */
			break;
		case XI_MIXCHAN:
			xig->numchans = t->data;
			break;
#endif
#if defined(HAVE_SDL_SDL_TTF_H)
		case XI_TTF:
			if(t->data) xig->flags |= XIF_USE_TTF;
			break;
#endif
		case XI_LUI:
			if(t->data) xig->flags |= XIF_USE_LUI;
			break;
		}
		t = tag_next(&tags);
	}
	if(!xig->prefs) {
#if defined(WIN32)
			/* Cheap-ass solution for now */
		snprintf(home_prefs, 2000, "xiqualrc");
#else
		snprintf(home_prefs, 2000, "%s/.xiqual/xiqualrc",
			getenv("HOME"));
#endif
		if(file_getsize(home_prefs)) {
			xig->prefs = cfg_loadprefs(PREFS_NAME, home_prefs, TAG_END);
		/* ..while Unix users have proper home directories guaranteed.
		 * WinNT 4 and 5 have them, in a way, but I can't be arsed.
		 */
		} else {
			xig->prefs = cfg_loadprefs(PREFS_NAME, "xiqualrc", TAG_END);
		}
	}
	/* If this is a second call, to adjust parameters, we need to
	    check what to create anew */

	/* Are we going to create a new main screen? */
	if(w && h && d && xig->screen) {
		/* Yep, so shut down SDL before re-initialising */
		SDL_Quit();
		xig->screen = 0;
	}
	if(!xig->screen) {
		switch(d) {
		case 8:
		case 15:
		case 16:
		case 24:
		case 32:
			break;
		default:
			d = 16; /* In case some odd depth makes its way in..
						or it is forgotten */
			break;
		}
		if(!w) w = 640;

		if(!h) h = 480;

		/* We already have a screen - kill off the old one */
		if(SDL_Init(sdlflags) == -1) {
			xi_release();
			return;
		}
		xig->screen = SDL_SetVideoMode(w, h, d, scrflags);
	}

	if(cursorname) {
		if(!cursorkey)
			cursorkey = SDL_MapRGB(xig->screen->format, 0, 0, 255);

		if(xig->cursor)
			view_free(xig->cursor);

		xig->cursor = view_new(XIVT_SCREEN, xig->screen, XIVT_IMAGE, cursorname,
			XIVT_BGCOL, cursorkey,
			TAG_END);
	}
#if defined(HAVE_SDL_SDL_MIXER_H)
	if(Mix_OpenAudio(xig->audio_rate, xig->audio_fmt,
		xig->audio_chan, xig->audio_bufsize)) {
		xi_lasterror = SDL_GetError();
		xi_release();
		return;
	}
	if(xig->numchans) Mix_AllocateChannels(xig->numchans);
	/* Get the actual audio settings */
	Mix_QuerySpec(&xig->audio_rate, &xig->audio_fmt, &xig->audio_chan);
#endif
#if defined(HAVE_SDL_SDL_TTF_H)
	if(xig->flags & XIF_USE_TTF) {
		if(TTF_Init() == -1) xig->flags ^= XIF_USE_TTF;
	}
#endif
	SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	if(xig->flags & XIF_USE_LUI) {
		if(!lui_init()) xig->flags ^= XIF_USE_LUI;
	}
	if(xig->cursor) SDL_ShowCursor(0);
}


/**
 This function gets called by xi_init() when initialising.
@doc
 You may pass similar tags as xi_init() to this function after setup
 to change resolution, audio mode or shut down subsystems.
@author Ronny Bangsund
@see xi_init(), xi_main(), SDL_Init(), Mix_OpenAudio(), xi_quit()
*/
void xi_inittag(ulong tag1, ... )
{
	XI_VOIDARG(xi_inittagA((tagitem *)&tag1))
}


/**
 Free the xiglob structure and all its related data, then release all SDL resources.
@doc
 This function replaces SDL_Quit(), so please check your atexit() calls.
@author Ronny Bangsund
@see xi_init(), xi_inittag(), xi_main(), Mix_CloseAudio(), SDL_Quit()
*/
void xi_quit()
{
#if defined(HAVE_SDL_SDL_MIXER_H)
	Mix_CloseAudio();
#endif
#if defined(HAVE_SDL_SDL_TTF_H)
	if(xig->flags & XIF_USE_TTF) TTF_Quit();
#endif
	if(xig->flags & XIF_USE_LUI) lui_quit();
	if(xig) {
		xi_release();
		SDL_Quit();
	}
}


/* This internal function deallocates all xiglob data */
static void xi_release()
{
	if(xig->cursor) view_free(xig->cursor);

	if(xig->prefs) list_free(xig->prefs);

	free(xig);
	xig = 0;
}

/*@}*/
