/* Lame-o options!
 *
 * Tested on Debian Linux and Win32.
 * It seems to work :)
 */

#if !defined(XIQUAL_OPTS_H)
#define XIQUAL_OPTS_H


#include "xios.h"
#include "lists.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 Option node.
@see xi_getopts()
*/
typedef struct OptNode {
	struct OptNode *next;
	struct OptNode *prev;
/**
 Name of option, a character or keyword.
*/
	char	*name;
/**
 The argument, if the option required a string arg.
*/
	char	*arg;
/**
 Type of option indicates what field to check.
*/
	ulong	type;
/**
 signed integer value, if the option required that.
*/
	long	val;
/**
 Float, if the option required a floating point number.
*/
	float	fval;
} OptNode;


/**
 The argument structure.
@see xi_getopts()
*/
typedef struct xiargs {
/**
 A list of OptNode structures.
*/
	List	opts;
/**
 Plain Node structs, if there were non-option args.
@doc
 The data element will contain a pointer to the string.
*/
	List	leftovers;
} xiargs;


/**
 User-supplied structure for xi_getopts().
@doc
 You fill in any number of these in an array.
@see xi_getopts(), testopts.c
*/
typedef struct xiopts {
/**
 Name of option.
@doc
 One character for single-dash options,
 a keyword for double-dash. For example,
 'h' for '-h' or 'help' for '--help'.
*/
	char	*name;
/**
 Type of argument.
@doc
 LOT_FLAG, LOT_KEYWORD, LOT_VALUE or LOT_FLOAT.
*/
	ulong	type;
/**
 A description of the option.
@doc
 Shown when no arguments are given to the program.
*/
	char	*description;
} xiopts;


enum {
	LOT_FLAG = 1,	/* No argument */
	LOT_KEYWORD,	/* Has a string argument */
	LOT_VALUE,		/* Has an integer argument */
	LOT_FLOAT		/* Has a floating point argument */
};


extern ulong xi_findopt(xiopts *opts, char *name);
extern xiargs *xi_getopts(int argc, char *argv[], char *description,
	char *usage, xiopts *opts);
extern void xi_freeopts(xiargs *args);


#ifdef __cplusplus
}
#endif

#endif /* XIQUAL_OPTS_H */
