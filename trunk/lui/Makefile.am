lib_LIBRARIES = liblui.a

libxiqualincludedir = $(includedir)/xiqual

liblui_a_SOURCES = lui.c button.c list.c msgbox.c

CFLAGS = -W -Wall -O3 -funroll-loops -Wpointer-arith \
	-I@top_srcdir@/include \
	`@top_srcdir@/xiqual-config --cflags` `sdl-config --cflags` \
