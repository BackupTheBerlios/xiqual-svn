#include "xiopts.h"


static void xi_destroyoptnode(OptNode *n)
{
	if(!n) return;

	if(n->name) free(n->name);
	if(n->arg) free(n->arg);
	free(n);
}


/**
 Find option and return flags.
@doc
 Looks for an option in a user-specified struct xiopts.
 The returned ulong will be set to LOT_FLAG, LOT_KEYWORD,
 LOT_VALUE or LOT_FLOAT.
@param opts User-supplied xiopts.
@param name String containing a word or character used as option.
@return ulong containing type of option, NULL if not found.
@author Ronny Bangsund
@see xi_getopts()
*/
ulong xi_findopt(xiopts *opts, char *name)
{
	int index=0;
	size_t len;

	len = strlen(name);
	while(opts[index].name) {
		if(strncmp(opts[index].name, name, len) == 0) {
			return opts[index].type;
		}
		index++;
	}
	return 0;
}


/**
 Parse command args.
@doc
 This function takes the arguments passed to a program and
 sorts them according to a user-supplied xiopts structure.

 The different types of options supported are LOT_FLAG,
 LOT_KEYWORD, LOT_VALUE and LOT_FLOAT. The OptNode structures
 in the xiargs returned will have this type set, and the
 appropriate data field will contain the argument to the option.

 An option that wasn't specified by the user will not be present
 in the list. Non-option arguments are in the leftovers List.

 See the testopts.c sourcecode for example usage. Note that you
 won't need to supply your own usage display function.
@param argc Argument count from the calling program.
@param argv Actual argument list from the caller.
@param description Brief description of the program.
@param usage One-line usage string. See any manpage for an example.
@param xiopts A structure with option names, types and descriptions.
@return A structure containing lists of options with arguments,
 plus leftover arguments. If no arguments were supplied, NULL is
 supplied after displaying program and option descriptions.
@author Ronny Bangsund
@see xi_freeopts(), xiargs, OptNode, testopts.c
*/
xiargs *xi_getopts(int argc, char *argv[],
 char *description, char *usage, xiopts *opts)
{
	xiargs *args;
	OptNode *n;
	int count=1;
	char *name, *arg;

	if(argc == 1) {
		printf("%s\n\nUsage:\n%s %s\nAvailable options:\n",
			description, argv[0], usage);
		count = 0;
		while(opts[count].name) {
			printf("\t");
			if(strlen(opts[count].name) == 1) {
				printf("-");
			} else {
				printf("--");
			}
			printf("%s\t\t%s\n", opts[count].name, opts[count].description);
			count++;
		}
		printf("\n");
		return NULL;
	}

	args = calloc(1, sizeof(xiargs));
	if(!args) return NULL;

	args->opts.destructor = (void *)xi_destroyoptnode;
	while(count < argc) {
		/* Find long option */
		if(strncmp(argv[count], "--", 2) == 0) {
			name = argv[count];
			name += 2;

			n = calloc(1, sizeof(OptNode));
			if(!n) return args;

			arg = strstr(name, "=");
			if(arg) {
				arg[0] = 0;
				arg++;
			}
			n->type = xi_findopt(opts, name);
			if(!n->type) {
				free(n);
				count++; continue;
			}

			n->name = strdup(name);
			if(!n->name) {
				free(n);
				return args;
			}

			switch(n->type) {
			case LOT_KEYWORD:
				if(arg) {
					n->arg = strdup(arg);
				} else {
					count++;
					n->arg = strdup(argv[count]);
				}
				break;
			case LOT_VALUE:
				if(arg) {
					n->val = atoi(arg);
				} else {
					count++;
					n->val = atoi(argv[count]);
				}
				break;
			case LOT_FLOAT:
				if(arg) {
					sscanf(arg, "%f", &n->fval);
				} else {
					count++;
					sscanf(argv[count], "%f", &n->fval);
				}
				break;
			}
			list_add(&args->opts, (Node *)n);
			count++; continue;
		}
		/* Find short option */
		if(strncmp(argv[count], "-", 1)==0) {
			name = argv[count];
			name++;

			n = calloc(1, sizeof(OptNode));
			if(!n) return args;

			n->type = xi_findopt(opts, name);
			if(!n->type) {
				free(n);
				count++; continue;
			}
			n->name = strdup(name);
			if(!n->name) {
				free(n);
				return args;
			}

			switch(n->type) {
			case LOT_KEYWORD:
				count++;
				n->arg = strdup(argv[count]);
				break;
			case LOT_VALUE:
				count++;
				n->val = atoi(argv[count]);
				break;
			case LOT_FLOAT:
				count++;
				sscanf(argv[count], "%f", &n->fval);
				break;
			}
			list_add(&args->opts, (Node *)n);
			count++; continue;
		}
		/* Not an option keyword; put into leftover list */
		list_newitem(&args->leftovers, strdup(argv[count]));
		count++;
	}
	return args;
}


/**
 Free xiargs structure.
@doc
 This deallocates all nodes and the structure.
 Always call this if manually calling xi_getopts().
@param args Structure returned by xi_getopts()
@author Ronny Bangsund
@see xi_getopts(), xiopts
*/
void xi_freeopts(xiargs *args)
{
	if(!args) return;

	while(args->opts.head) {
		list_delete(&args->opts, args->opts.head);
	}
	while(args->leftovers.head) {
		list_delete(&args->leftovers, args->leftovers.head);
	}
}
