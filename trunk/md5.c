/**@name md5
 Simple MD5-summing program.
@doc
 <h3>Usage</h3>
 <em>md5</em> [-b] [-v] [-c] | [file...]

 md5 generates MD5 checksums, like md5sum(1), but no wildcards are
 properly supported in the program itself. This should not be a problem
 if using Bash or similar shells. On Win32, getting Unix Utilities is
 recommended for developers, anyway.

 The -v flag prints each filename and status regardless of whether it
 has a checksum or not. Use alongside the -c option.

 If -c appears on the command line, the files specified are expected to
 be md5sum output as generated with normal operation. The filenames to
 verify are gathered from these files, and each invalid sum is printed
 to the standard output. Use -v if you want everything printed.

 The -b flag does nothing but print an asterisk before the filename.

 <h3>TODO</h3>
 Implement -c and -v, plus wildcards.
*/

/* $Author: nny $
 * $Date: 2003/12/11 08:36:36 $
 * $Id: md5.c,v 1.6 2003/12/11 08:36:36 nny Exp $
 *
 * $Log: md5.c,v $
 * Revision 1.6  2003/12/11 08:36:36  nny
 *
 * Modified Files:
 * All.
 *
 *
*/

/**
*/

#include "stdio.h"
#include "stdlib.h"

#include "files.h"
#include "md5sum.h"
#include "xiopts.h"


void my_hook(char *buf, size_t len);

MD5_CTX context;
unsigned char *key1, *key2;
char *binary;


#define BUFSIZE 50000


struct xiopts opt[] = {
	{"b", LOT_FLAG, "Read files in binary mode (doesn't actually do much,\n"
		"\t\t\tbeyond adding an asterisk to the output)"},
	{"c", LOT_FLAG, "Check the following digest file (default is to generate)"},
	{"v", LOT_FLAG, "Verbose - print filenames while checking"},
	{0, 0, 0}
};


void my_hook(char *buf, size_t len)
{
	MD5Update(&context, buf, len);
}


void sumfile(char *name)
{
	autofile *af;

	af = file_new(name);
	if(!af) return;

	if(!af->size) return;

	if(!file_initbuf(af, BUFSIZE)) {
		fprintf(stderr, "Couldn't initialise a read buffer\n");
		return;
	}

	file_readhook(af, my_hook);
	MD5Init(&context);

	while(af->pos < af->size) {
		file_read(af, BUFSIZE);
	}
	MD5Final(key1, &context);
	MD5Ascii(key1, key2);
	printf("%s %s%s\n", key2, binary, af->name);
	file_free(af);
}


void checklist(char *name)
{
	autofile *list, *file;

}


int main(int argc, char *argv[])
{
	OptNode *o;
	Node *n;
	xiargs *args;
	int verbose = 0, check=0;

	binary = " ";

	key1 = (unsigned char *)calloc(1, 40);
	if(!key1) {
		fprintf(stderr, "Memory allocation failed!\n");
		return 2;
	}
	key2 = (unsigned char *)calloc(1, 40);
	if(!key2) {
		free(key1);
		fprintf(stderr, "Memory allocation failed!\n");
		return 2;
	}

	args = xi_getopts(argc, argv, "MD5 summer (Xiqual example program)",
		"[-b] [-v] [-c] | [file...]", opt);
	if(!args) return 0;

	o = (OptNode *)args->opts.head;
	while(o) {
		switch(o->name[0]) {
		case 'b':
			binary = "*";
			break;
		case 'v':
			verbose = 1;
			break;
		case 'c':
			check = 1;
		}
		o = o->next;
	}
	n = args->leftovers.head;
	if(check) {
		while(n) {
			checklist(n->data);
			n = n->next;
		}
	} else {
		while(n) {
			sumfile(n->data);
			n = n->next;
		}
	}
	free(key1);
	free(key2);
	xi_freeopts(args);
	return 0;
}
