lib_LIBRARIES = libxigfx.a

libxiqualincludedir = $(includedir)/xiqual

libxigfx_a_SOURCES = fonts.c iarea.c tiles.c views.c xi_init.c xi_main.c

CFLAGS = -W -Wall -O3 -funroll-loops -I@top_srcdir@/include \
	`@top_srcdir@/xiqual-config --cflags` `sdl-config --cflags` \
	-Wpointer-arith
