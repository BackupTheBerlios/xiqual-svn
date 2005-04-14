lib_LIBRARIES = libxicgi.a

libxiqualincludedir = $(includedir)/xiqual

libxicgi_a_SOURCES = cgi_checkform.c cgi_clearcookie.c cgi_end.c cgi_expiry.c \
	cgi_findval.c cgi_form.c cgi_freecookie.c cgi_getcookie.c cgi_html.c cgi_include.c \
	cgi_init.c cgi_setcookie.c cgi_unescape.c cgi_validator.c

CFLAGS = -W -Wall -O3 -funroll-loops -I@top_srcdir@/include \
	`@top_srcdir@/xiqual-config --cflags` -Wpointer-arith
