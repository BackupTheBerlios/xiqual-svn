# Makefile.in generated automatically by automake 1.4-p6 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Makefile.am for the most basic library


SHELL = /bin/sh

srcdir = .
top_srcdir = ..

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/Xiqual
pkglibdir = $(libdir)/Xiqual
pkgincludedir = $(includedir)/Xiqual

top_builddir = ..

ACLOCAL = aclocal-1.4
AUTOCONF = autoconf
AUTOMAKE = automake-1.4
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = x86_64-unknown-linux-gnu
CC = gcc
DATADIR = 
HAVE_LIB = @HAVE_LIB@
INETLIB = 
LIB = @LIB@
LTLIB = @LTLIB@
MAJOR_VERSION = 0
MAKEINFO = makeinfo
MICRO_VERSION = 1
MINOR_VERSION = 9
PACKAGE = Xiqual
RANLIB = ranlib
VERSION = 0.9.1

lib_LIBRARIES = libxitools.a

libxiqualincludedir = $(includedir)/xiqual

MISC_SRC = file_getsize.c files.c opts.c

IFF_SRC = idtostr.c iff_check.c iff_close.c iff_endchunk.c iff_fixsize.c 	iff_goodid.c iff_goodtype.c iff_new.c iff_newchunk.c iff_open.c 	iff_readchunkdata.c iff_readchunkheader.c iff_rewind.c iff_skipchunkdata.c 	iff_writechunkdata.c iff_correctchunk.c


LIST_SRC = list_add.c list_delete.c list_free.c list_getnode.c list_insert.c list_new.c 	list_newitem.c node_end.c node_makecircular.c node_start.c list_remove.c 	list_makecircular.c node_findbyname.c list_addhead.c node_findbycontents.c


PREFS_SRC = cfg_freesection.c cfg_freevar.c cfg_loadprefs.c 	cfg_newsection.c cfg_newvar.c cfg_saveprefs.c cfg_mapfromenv.c 	cfg_isbool.c cfg_findsection.c cfg_findnext.c cfg_findvar.c


XML_SRC = xmlread.c xmlwrite.c xmlhooks.c xml_printesc.c

# String functions and replacements
STR_SRC = strfind.c str_getvalue.c strlower.c strsplit.c strupper.c 	strisnum.c strtokenise.c ftr.c


TAG_SRC = tag_alloclist.c tag_finditem.c tag_freelist.c tag_getdata.c 	tag_next.c


# Memory functions (more sure to come)
MEM_SRC = mem_arrayget.c mem_arrayset.c

HASH_SRC = crc32.c hash.c hash_newfromlist.c hash_merge.c

libxitools_a_SOURCES = errors.c md5sum.c 	$(MISC_SRC) $(IFF_SRC) $(LIST_SRC) $(PREFS_SRC) $(STR_SRC) 	$(TAG_SRC) $(MEM_SRC) $(XML_SRC) $(HASH_SRC)


CFLAGS = -W -Wall -I.. -I../include 	`sdl-config --cflags` -Wpointer-arith -O3 -funroll-loops


LDFLAGS = 
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
LIBRARIES =  $(lib_LIBRARIES)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I..
CPPFLAGS = 
LIBS = 
libxitools_a_LIBADD = 
libxitools_a_OBJECTS =  errors.o md5sum.o file_getsize.o files.o opts.o \
idtostr.o iff_check.o iff_close.o iff_endchunk.o iff_fixsize.o \
iff_goodid.o iff_goodtype.o iff_new.o iff_newchunk.o iff_open.o \
iff_readchunkdata.o iff_readchunkheader.o iff_rewind.o \
iff_skipchunkdata.o iff_writechunkdata.o iff_correctchunk.o list_add.o \
list_delete.o list_free.o list_getnode.o list_insert.o list_new.o \
list_newitem.o node_end.o node_makecircular.o node_start.o \
list_remove.o list_makecircular.o node_findbyname.o list_addhead.o \
node_findbycontents.o cfg_freesection.o cfg_freevar.o cfg_loadprefs.o \
cfg_newsection.o cfg_newvar.o cfg_saveprefs.o cfg_mapfromenv.o \
cfg_isbool.o cfg_findsection.o cfg_findnext.o cfg_findvar.o strfind.o \
str_getvalue.o strlower.o strsplit.o strupper.o strisnum.o \
strtokenise.o ftr.o tag_alloclist.o tag_finditem.o tag_freelist.o \
tag_getdata.o tag_next.o mem_arrayget.o mem_arrayset.o xmlread.o \
xmlwrite.o xmlhooks.o xml_printesc.o crc32.o hash.o hash_newfromlist.o \
hash_merge.o
AR = ar
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
SOURCES = $(libxitools_a_SOURCES)
OBJECTS = $(libxitools_a_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --foreign --include-deps xitools/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-libLIBRARIES:

clean-libLIBRARIES:
	-test -z "$(lib_LIBRARIES)" || rm -f $(lib_LIBRARIES)

distclean-libLIBRARIES:

maintainer-clean-libLIBRARIES:

install-libLIBRARIES: $(lib_LIBRARIES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	@list='$(lib_LIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(libdir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done
	@$(POST_INSTALL)
	@list='$(lib_LIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(RANLIB) $(DESTDIR)$(libdir)/$$p"; \
	    $(RANLIB) $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done

uninstall-libLIBRARIES:
	@$(NORMAL_UNINSTALL)
	list='$(lib_LIBRARIES)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(libdir)/$$p; \
	done

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

libxitools.a: $(libxitools_a_OBJECTS) $(libxitools_a_DEPENDENCIES)
	-rm -f libxitools.a
	$(AR) cru libxitools.a $(libxitools_a_OBJECTS) $(libxitools_a_LIBADD)
	$(RANLIB) libxitools.a

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags -o $$here/TAGS $(ETAGS_ARGS) $$tags  $$unique $(LISP))

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = xitools

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-libLIBRARIES
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-libLIBRARIES
uninstall: uninstall-am
all-am: Makefile $(LIBRARIES)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(libdir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-libLIBRARIES mostlyclean-compile \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-libLIBRARIES clean-compile clean-tags clean-generic \
		mostlyclean-am

clean: clean-am

distclean-am:  distclean-libLIBRARIES distclean-compile distclean-tags \
		distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-libLIBRARIES \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-libLIBRARIES distclean-libLIBRARIES \
clean-libLIBRARIES maintainer-clean-libLIBRARIES uninstall-libLIBRARIES \
install-libLIBRARIES mostlyclean-compile distclean-compile \
clean-compile maintainer-clean-compile tags mostlyclean-tags \
distclean-tags clean-tags maintainer-clean-tags distdir info-am info \
dvi-am dvi check check-am installcheck-am installcheck install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
