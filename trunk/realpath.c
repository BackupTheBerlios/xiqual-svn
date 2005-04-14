/*
 * Get base path of a file. Why isn't this in the GNU tools?
 * pwd is not always adequate for this purpose.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[])
{
#if defined(PATH_MAX)
	char	path[PATH_MAX];
#else
	char	path[4096];
#endif

	if(argc>1) {
		if(realpath(argv[1], path))
			printf("%s", path);
	}
	return 0;
}
