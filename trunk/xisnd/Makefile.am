lib_LIBRARIES = libxisnd.a

libxiqualincludedir = $(includedir)/xiqual

libxisnd_a_SOURCES = load.c play.c

CFLAGS = -W -Wall -O3 -funroll-loops -Wpointer-arith \
	-I@top_srcdir@/include \
	`@top_srcdir@/xiqual-config --cflags` `sdl-config --cflags` \
