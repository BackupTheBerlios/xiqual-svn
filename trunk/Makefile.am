# Makefile.am for the Xiqual Tools root directory
AUTOMAKE_OPTIONS = 1.4
SUBDIRS = xitools xinet xigfx xisnd xicgi lui include

bin_PROGRAMS = ifftest realpath statuscleaner tileset tilefont \
	xiqual_template

libxiqualincludedir = $(includedir)/xiqual

TEXTFILES = README CREDITS AUTHORS TODO LGPL Changes MANIFEST BUGS API \
	library.xml sink.xml hello.xhtml
GRAPHICS = gfx/cursor.png

EXTRA_DIST = acconfig.h config.h autogen.sh $(TEXTFILES) \
	stest html/* sprite/* VisualC/*.dsp VisualC/*.dsw xiqualrc \
	xiqualrc.x gfx/* VisualC/*.txt

bin_SCRIPTS = xiqual-config

noinst_PROGRAMS = iffwrite md5 cgi tagtest prefs tokenise minimalgui \
	xmltest crc hashtest testopts

iffwrite_SOURCES = iffwrite.c
iffwrite_LDADD = -L@top_srcdir@/xitools -lxitools
ifftest_SOURCES = ifftest.c
ifftest_LDADD = -L@top_srcdir@/xitools -lxitools
md5_SOURCES = md5.c
md5_LDADD = -L@top_srcdir@/xitools -lxitools
crc_SOURCES = crc.c
crc_LDADD = -L@top_srcdir@/xitools -lxitools
hashtest_SOURCES = hashtest.c
hashtest_LDADD = -L@top_srcdir@/xitools -lxitools
cgi_SOURCES = cgi.c
cgi_LDADD =  -L@top_srcdir@/xitools -L@top_srcdir@/xicgi -lxicgi -lxitools
realpath_SOURCES = realpath.c
tagtest_SOURCES = tagtest.c
tagtest_LDADD = -L@top_srcdir@/xitools -lxitools
prefs_SOURCES = prefs.c
prefs_LDADD = -L@top_srcdir@/xitools -lxitools
statuscleaner_SOURCES = statuscleaner.c
statuscleaner_LDADD = -L@top_srcdir@/xitools -lxitools
testopts_SOURCES = testopts.c
testopts_LDADD = -L@top_srcdir@/xitools -lxitools
tokenise_SOURCES = tokenise.c
tokenise_LDADD = -L@top_srcdir@/xitools -lxitools
tileset_SOURCES = tileset.c
tileset_LDADD = -L@top_srcdir@/xitools -L@libdir@ -lxitools -lSDL_image -lz
tilefont_SOURCES = tilefont.c
tilefont_LDADD = -L@top_srcdir@/xitools -L@libdir@ -lxitools -lSDL_image -lz
minimalgui_SOURCES = minimalgui.c
minimalgui_LDADD = -L@top_srcdir@/lui -L@top_srcdir@/xigfx \
	-L@top_srcdir@/xitools -L@libdir@ -llui -lxigfx -llui -lxitools \
	-lSDL_image -lSDL_mixer -lSDL_ttf -lSDL -lz
xmltest_SOURCES = xmltest.c
xmltest_LDADD = -L@top_srcdir@/xitools -lxitools
xiqual_template_SOURCES = xiqual_template.c
xiqual_template_LDADD = -L@top_srcdir@/xitools -lxitools

DEFS = @DEFS@
CFLAGS = -I@top_srcdir@/include `sdl-config --cflags` -O3 \
	-funroll-loops -W -Wall

LDFLAGS =

dist-hook:
	rm -f MANIFEST
	touch MANIFEST
	touch $(PACKAGE)-$(VERSION)/MANIFEST
	(cd $(PACKAGE)-$(VERSION); ls -1 * | grep [!CVS] >>../MANIFEST)
	cp MANIFEST $(PACKAGE)-$(VERSION)
	tar chojf $(PACKAGE)-$(VERSION).tar.bz2 $(PACKAGE)-$(VERSION)

install-data-hook:
	sed -e "s\\DATADIR\$(pkgdatadir)\g" < xiqualrc.x > xiqualrc
	mkdirhier $(prefix)/doc/$(PACKAGE)
	cp -r html $(prefix)/doc/$(PACKAGE)
	install -m644 $(TEXTFILES) $(prefix)/doc/$(PACKAGE)
	mkdirhier $(pkgdatadir)/gfx
	install -m644 $(GRAPHICS) $(pkgdatadir)/gfx
	install -m644 xiqualrc $(pkgdatadir)

deb:
	./configure --prefix=/usr
	make
	make doc
	debuild binary

html:
	rm -rf html
	doc++ -H -f --dir html xi*/*.c lui/*.c include/*.h

doc-check:
	make html
	install -m644 xiqual.css html
	tidy -utf8 -asxhtml -m -c html/*.html

strip:
	strip $(noinst_PROGRAMS) $(bin_PROGRAMS)
