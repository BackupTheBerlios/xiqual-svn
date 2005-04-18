# Makefile.am for the most basic library

lib_LIBRARIES = libxitools.a

libxiqualincludedir = $(includedir)/xiqual

MISC_SRC = file_getsize.c files.c opts.c

IFF_SRC = idtostr.c iff_check.c iff_close.c iff_endchunk.c iff_fixsize.c \
	iff_goodid.c iff_goodtype.c iff_new.c iff_newchunk.c iff_open.c \
	iff_readchunkdata.c iff_readchunkheader.c iff_rewind.c iff_skipchunkdata.c \
	iff_writechunkdata.c iff_correctchunk.c

LIST_SRC = list_add.c list_delete.c list_free.c list_getnode.c list_insert.c list_new.c \
	list_newitem.c node_end.c node_makecircular.c node_start.c list_remove.c \
	list_makecircular.c node_findbyname.c list_addhead.c node_findbycontents.c

PREFS_SRC = cfg_freesection.c cfg_freevar.c cfg_loadprefs.c \
	cfg_newsection.c cfg_newvar.c cfg_saveprefs.c cfg_mapfromenv.c \
	cfg_isbool.c cfg_findsection.c cfg_findnext.c cfg_findvar.c

XML_SRC = xmlread.c xmlwrite.c xmlhooks.c xml_printesc.c

# String functions and replacements
STR_SRC = strfind.c str_getvalue.c strlower.c strsplit.c strupper.c \
	strisnum.c strtokenise.c ftr.c

TAG_SRC = tag_alloclist.c tag_finditem.c tag_freelist.c tag_getdata.c \
	tag_next.c

# Memory functions (more sure to come)
MEM_SRC	= mem_arrayget.c mem_arrayset.c

HASH_SRC = crc32.c hash.c hash_newfromlist.c hash_merge.c

libxitools_a_SOURCES = errors.c md5sum.c \
	$(MISC_SRC) $(IFF_SRC) $(LIST_SRC) $(PREFS_SRC) $(STR_SRC) \
	$(TAG_SRC) $(MEM_SRC) $(XML_SRC) $(HASH_SRC)

CFLAGS = -W -Wall -I@top_srcdir@ -I@top_srcdir@/include -g -ggdb \
	`sdl-config --cflags` -Wpointer-arith -O3 -funroll-loops

LDFLAGS = 
