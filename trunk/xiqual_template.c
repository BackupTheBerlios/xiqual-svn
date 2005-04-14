/*
 * Generate a basic main.c that creates an SDL screen with Xiqual.
 */

/* Being terribly lazy, I've made it so that this
 * header includes everything I need for simple
 * command line tools.
 */
#include "xiqual.h"
#include "fcntl.h"
#include "sys/stat.h"

autofile *header, *footer, *body;

/* Shutdown function */
void free_all()
{
	file_free(header);
	file_free(body);
	file_free(footer);
}

int main(int argc, char *argv[])
{
	char *templatedir, *bodyname, *dirname;
	char filename[1000];
	FILE *newfile;

	if(argc < 2) {
		fprintf(stderr, "Usage:\n%s <directory to create>\n", argv[0]);
		return 0;
	}
	dirname = argv[1];
	templatedir = "templates";
	bodyname = "lui.c";

	snprintf(filename, 1000, "%s/header.c", templatedir);
	header = file_new(filename);
	snprintf(filename, 1000, "%s/footer.c", templatedir);
	footer = file_new(filename);
	snprintf(filename, 1000, "%s/%s", templatedir, bodyname);
	body = file_new(filename);
	atexit(free_all);
	if(!(header && body && footer)) {
		fprintf(stderr, "Couldn't open templates in %s/\n", templatedir);
		return 2;
	}
	if(!(file_load(header) && (file_load(body)) && (file_load(footer)))) {
		fprintf(stderr, "Couldn't load templates into memory\n");
		return 2;
	}
	if(mkdir(dirname, 0755)) {
		fprintf(stderr, "Couldn't create directory %s\n", dirname);
		return 2;
	}
	/* Write main.c from template files */
	snprintf(filename, 1000, "%s/main.c", dirname);
	newfile = fopen(filename, "w+");
	if(!newfile) {
		fprintf(stderr, "Couldn't create %s\n", filename);
		return 2;
	}
	printf("Created %s\n", filename);
	fprintf(newfile, "%s%s%s\n", header->buf, body->buf, footer->buf);
	fclose(newfile);
	/* Write Makefile */
	return 0;
}
