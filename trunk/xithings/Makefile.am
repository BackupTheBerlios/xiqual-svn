lib_LIBRARIES = libxithings.a

libxiqualincludedir = $(includedir)/xiqual

libxithings_a_SOURCES = add_attrib.c alloc_attrib.c alloc_thing.c create_attrib.c \
	create_thing.c dupe_attrib.c dupe_thing.c tag_attrib.c tag_thing.c

CFLAGS = -W -Wall -O3 -funroll-loops -I@top_srcdir@/include \
	`@top_srcdir@/xiqual-config --cflags` -Wpointer-arith

LDFLAGS =
