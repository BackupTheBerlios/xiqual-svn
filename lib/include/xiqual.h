/**
@name Questions and answers.
@doc
 <h1>Xiqual</h1>

 <ol>
 <li>What is it?</li>
 <p>A collection of libraries that help me writing SDL programs,
 and also do many other things.

 <li>What's in it?</li>
 Several sub-libraries:
 <ol>
	<li>xitools</li>
	<p>This is the core library. All the other sub-libraries require this
	to some extent, particularly for linked lists and all the related
	manipulation, but sometimes also for file-handling.

	<p>If you need to read some configuration files, the cfg_* functions
	will do most of what you need. Hooks are supported to do your own
	processing of each variable, although this bit needs some more
	documentation.

	<p>The iff_* functions help you create EA-IFF85 style files. It's a
	simple standard for binary data, with some requirements that make
	them useful for portable data. The basic FORM types can be used
	for pretty much any purpose. You may know these filetypes from the
	Amiga platform.

	<p>Taglists also have some utility functions. If you want to avoid
	changing the interface every time you feel a new option should be
	supported, this is the way to do it. Using varargs, you can make
	your API permanent, only adding more tags for new features. Highly
	recommended for plugin interfaces and dynamic libraries. If only
	the Gtk+ developers used tags..

	<p>MD5 calculation has been grabbed off the 'net, with one additional
	function by me to create a printable string.

	<p>The string tokeniser in Linux' C library, strtok(), is pretty daft.
	Even the more recent strtok_r() isn't very good, as it's not very
	portable. They both have many bugs, so I had to write a new one.
	Use str_tokenise() to create a list of tokens more portably.
	Unlike strtok(), it should be thread-safe.

	<li>xigfx</li>
	<p>You'll find the initialisation functions here. xi_init(), xi_main()
	and xi_quit() are the important calls. Utility functions for
	graphics are here, too - views and tilesets are the only really
	evolved ones included. A map loader, displayer and scroller is
	in the works, using tilesets as a foundation.

	<li>xicgi</li>
	<p>I started writing some utility functions for CGI creation,
	and couldn't find a better place to put them. If there are other
	pervs out there who like to write everything in C, this could be
	useful. Cookies are supported, too.

	<li>xinet</li>
	<p>This sub-library only has a server-toolkit for now.
	It can create select-servers (non-threaded servers).
	Mostly untested; works well locally, but needs larger scale testing.

	<li>xithings</li>
	<p>Various functions to handle objects, things, thingamajigs.
	You know, players, monsters, inventory objects, magic, whatever.
	Scripting support will somehow be integrated into this later.
 </ol>

 <li>What's needed to use and compile it?</li>
 <p>Development is happening under Linux, as is fashionable these days.
 Several of the libraries have compiled nicely on many common Unix-
 like platforms, even without GNU cc. Unless there is a MacOS X
 port of SDL, that probably won't work, but the other parts worked
 on the SourceForge compile farm.

 <h4>Recommended setup</h4>
 <p>Unix-derivative OS with SDL (http://libsdl.org), SDL_image,
 SDL_mixer and SDL_ttf, at least the GNU C compiler and proper
 headers, plus GDB. DDD is a nice frontend, and Valgrind has helped
 me squash many memory bugs. A profiler would be a nice addition,
 but you'll be distracted by the lack of optimisation in Xiqual itself.

 <li>What about Win32?</li>
 <p>A set of Visual C++ 5.x project files are in the VC5/ directory.
 Still no luck actually linking programs with MingW32 (Linux hosted).

 <li>Right..and C++?</li>
 <p>Much better support there. No Xiqual wrapper class has been written,
 but it should link. C links with nearly anything. The headers have
 definitions to ensure C++ compatibility, thanks to Mr.O'Neill.

 <li>What's the license?</li>
 <p>LGPL. See the file of the same name. If you find any code of use
 to you, feel free to simply use the sourcefile and related headers.
 Just abide by the LICENSE and all is well. If you think I'm doing
 anything uncool, let me know.

 <li>You mentioned SDL..what the heck is that?</li>
 <p>Simple Directmedia Library. The name gives some indication what it
 is..basically, it's a canvas for portable graphics programming.
 It supplies the basic functions to create a 2D-display (and also
 OpenGL with recent versions) and handle input from keyboard, mouse
 and other controllers, timers, sound and threads (on some platforms),
 plus even CD-ROM access. All this works in Win32, Linux and MacOS,
 plus a lot of Unix-derivatives.

 <p>SDL has been used to port many games to Linux, and I'd recommend it
 for input handling with OpenGL. For 2D programming, it has blitting
 functions, image loading (extended a lot with SDL_image) and all sorts
 of low-level screen manipulation. But it is merely a canvas; you need
 to supply the drawing tools. Read up on Bresenham's algorithm ;)

 <p>Get SDL from http://libsdl.org, and grab SDL_image, SDL_mixer and
 SDL_ttf while you're at it.

 <li>What does the name mean?</li>
 <p>It's Ouranian-Barbaric for "manifest". It's a verb.

 <li>How do I pronounce it?</li>
 <p>How would you like to pronounce it?

 </ol>

 <h6>
 Clumsily written in one of the headers..
 Note the funny way DOC++ mangles this document ;)
 </h6>
@author Ronny Bangsund, Shane O'Neill
*/

/* Main header - include this is you use the global
 * structure. Required by xi_init() and xi_quit().
*/

/**
*/

/**@name <div class="keyword">Xiqual tools<span>Tools used to create datafiles</span></div> */ /*@{*/
/*@Include: ../tileset.c */
/*@Include: ../tilefont.c */
/*@Include: ../xiqual_template.c */
/*@}*/

/**@name Xiqual example programs
 These programs exist mainly to show how some elements of this
 toolkit work, but I personally use the statuscleaner program
 to keep my system somewhat cleaner.
*/
/*@{*/
/*@Include: ../statuscleaner.c */
/*@Include: ../md5.c */
/*@Include: ../minimalgui.c */
/*@}*/

/**@name Optargs
 Portable options - no need to look for similar packages,
 download and compile. This is mostly here due to lack of
 decent optarg packages in VC++.
*/
/*@{*/
/*@Include: ../xitools/opts.c */
/*@Include: ../include/xiopts.h */
/*@}*/

/**@name XML functions
 This set of functions, known as xixml, are meant for use with
 XML as a pure data format. Stylesheets of any kind aren't
 directly supported. All function calls are geared towards
 loading a whole document and only accepting the tags an API
 user desires.

 To parse a document, you need to specify hooks for different
 tags or categories. You may also specify contexts they're to
 be found in. Finally, you could simply list the tags and
 contexts allowed, then clean out the undesirables with xml_prune().
*/
/*@{*/
/*@Include: ../xitools/xmlread.c */
/*@Include: ../xitools/xmlwrite.c */
/*@Include: ../xitools/xmlhooks.c */
/*@Include: ../include/xixml.h */
/*@}*/


#if !defined(_XIQUAL_H)
#define _XIQUAL_H

#include "xisetup.h"

#ifdef __cplusplus
extern "C" {
#endif

extern xiglob	*xig;
extern const char	*xi_lasterror;

extern int xi_main();
extern void xi_seterror(char *text);


#ifdef __cplusplus
}
#endif

#endif /* EOF */
