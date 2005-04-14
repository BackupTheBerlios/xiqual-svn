/*
 * Clean Debian status files.
 *
 * Fecking purge and deinstall stubs are all over the status
 * file for dpkg, even when none of the contents exist. Grr.
 *
 * Any remaining "deinstall" status sections indicate that there
 * are still configuration files remanining. Use dpkg --purge and
 * then run this utility again.
 */

#include "xios.h"
#include "files.h"
#include "lists.h"

void usage()
{
	printf("Usage:\n\tstatuscleaner filename\n\n"
		"The file specified must be a Debian dpkg status file,\n"
		"or results are uncertain. A clean version of the file,\n"
		"without purged entries, will be printed on stdout.\n"
		"The deleted entries will be printed on stderr.\n\n"
		"Note: it looks for Status: flags matching \"purge\" or\n"
		"\"deinstall ok not-installed\". Let me know if this\n"
		"breaks anything.\n");
}

/**
@name statuscleaner
 Clean Debian dpkg status files from purged/not-installed entries.
@doc
 I've had no joy finding a tool to do just this. Here's one that not only removes
 "purged" and "deinstalled ok not-installed" entries from those files, but also
 illustrates some use of Xiqual's basic functions. There are no known side-effects
 yet. If you get to know any of them, let me know, too.

 To use the program, find your megabyte-sized status file (or whatever the bloated
 size is), which usually resides in /var/lib/dpkg/ under the name "status". Become
 the superuser and make a copy of it. Don't skip this step; if this program mangles
 your file, I can laugh at your stupidity.

 Next, run the program on the file, redirecting the standard output to where you want
 your clean status file to go. Example:

 statuscleaner status.backup > status

 Now, if this isn't a week-fresh or younger Debian system, you're going to see an
 awful lot of text printed on the console. This is stderr, which you may also
 redirect to a file, if need be. Last in the output is the number of entries that
 were deemed unnecessary.

 If you go through the status file manually after this, you might still find some
 entries with status "deinstall" (among other things). These probably have a few
 configuration files lying around. It would be wise to check these, if it's anything
 imporant like a highscore list for some game, or you can just dpkg --purge that, too,
 if it's just some server configuration file.

 Tip: Take everything written in any computer documentation with a grain of salt.

 Statuscleaner was written in a matter of minutes without time, motivation or reason
 to read any Debian documents. If you end up screwing your system up beyond the
 healing powers of even dpkg -i, I want the details for my list of dastardly deeds.
@return The program returns non-zero if no file was processed.
@author Ronny Bangsund
*/

/**
*/

int main(int argc, char *argv[])
{
	autofile	*af;
	int	count=0;
	char	*line, *lineptr;
	List	packages, *pkg=0;
	Node	*n, *pkgnode;

	if(argc < 2) {
		usage();
		return 1;
	}

	af = file_new(argv[1]);
	if(!af) {
		fprintf(stderr, "Can't open file %s!\n", argv[1]);
		usage();
		return 1;
	}
	memset(&packages, 0, sizeof(List));
	file_load(af);
	if((af->buf) && (af->size)) {
		line = strtok_r(af->buf, "\n", &lineptr);
		/* Get a list of packages in a messy List/Node clusterfuck */
		while(line) {
			if(strncmp(line, "Package:", 8)==0) {
				pkg = list_new(0);
				pkgnode = list_newitem(&packages, pkg);
				if(pkgnode) {
					if(pkg) n = list_newitem(pkg, line);
				} else {
					free(pkg);
				}
			} else {
				if(pkg) n = list_newitem(pkg, line);
			}
			line = strtok_r(NULL, "\n", &lineptr);
		}
		/* Figure out what to write and what to keep,
		  * and free the data in the process.
		  */
		while((n = packages.head)) {
			pkg = n->data;
			n = node_findbycontents(pkg->head, "Status:");
			if(n) {
				if((strstr(n->data, "purge")) ||
					(strstr(n->data, "deinstall ok not-installed"))) {
					count++;
					fprintf(stderr, "%s purged completely\n",
						(char *)pkg->head->data);
				} else {
					n = pkg->head;
					while(n) {
						printf("%s\n", (char *)n->data);
						n = n->next;
					}
					printf("\n");
				}
			}
			n = packages.head;
			list_remove(&packages, n);
			pkg = n->data;
			free(n);
			while(pkg->head) {
				list_delete(pkg, pkg->head);
			}
			free(pkg);
		}
		fprintf(stderr, "Removed %d purged package references.\n", count);
	}
	file_free(af);
	return 0;
}
