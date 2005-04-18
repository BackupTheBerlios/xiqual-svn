lib_LIBRARIES = libxinet.a

libxiqualincludedir = $(includedir)/xiqual

libxinet_a_SOURCES = net_addnodes.c net_server.c net_server_free.c \
	net_handle.c net_accept.c net_process.c net_setnonblock.c \
	net_tag_server.c net_remove_client.c net_client.c net_client_free.c \
	net_read.c net_write.c

CFLAGS = -W -Wall -O3 -funroll-loops -I@top_srcdir@/include 
	`@top_srcdir@/xiqual-config --cflags` -Wpointer-arith
