This directory contains project files necessary to compile Xiqual
with Visual C++ 6.0 or later. Look for a separate package with compiled
libraries if you're reading this file from the source directory.

(If you manage to get the project files into VC5 or 7, it should compile
just fine there.)

I don't care about xicgi in Win32, although it *compiles*.
Also, xinet has problems from time to time; hopefully a truckload's
worth of typedefs has solved this for now.

Most of the libraries get the same testing on Win32 and Linux.
All libraries rely on xitools (mostly) and xigfx (a little); porting
starts in those. A few OS-specific issues were detected in xinet,
but most #if define(WIN32) blocks were moved into xios.h.

(A chunk of optimisation keywords for VC++ are in this header now;
binaries should be very small as a result. There shouldn't be any loss
of speed, either.)

Xiqual can currently open a screen and display graphics just as well
in the platforms tested so far. My version of SDL is compiled against
DirectX SDK 7.x (just the headers and .lib files :), and nothing
seems unusual. Hopefully someone can help me with a Mac port..

(I have CodeWarrior, but dunno what to do with any resulting files
and forks.)
