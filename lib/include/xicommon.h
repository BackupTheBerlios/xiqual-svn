/*
 * Miscellaneous commonly used structures.
 */

#if !defined(_XIQUAL_COMMON_H)
#define _XIQUAL_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 A variable.
@doc
 This extended node contains the type of variable, currently one of
 <b>V_STRING</b>, <b>V_VALUE</b> and <b>V_BOOL</b> and
 the related data.
@author Ronny Bangsund
@see preferences, cfg_newvar(), xml_newdoc(), xmltag
*/
typedef struct variable {
/**
 Next variable in List.
*/
	struct variable	*next;
/**
 Previous variable in List.
*/
	struct variable	*prev;
/**
 The mandatory name of the variable.
@doc
 cfg_* functions will search by this only. Anything recognised by isalpha() is
 legal as a variable name. Locale settings may affect what is considered an
 alphabetical character.
*/
	char	*name;
/**
 Type of variable, as listed in prefs.h.
*/
	ulong	type;
/**
 A string, if type is <b>PREFS_STRING</b>.
*/
	char	*string;
/**
 An integer (signed) if type is <b>V_VALUE</b> or <b>V_BOOL</b>.
@doc
 If a lone word without an equals sign is encountered in a
 configuration file, it is turned into a boolean variable.
*/
	int	value;
} variable;


typedef struct hook {
	struct hook *next;
	struct hook *prev;
	char	*name;
	char	*id;
	int		(*func)(struct hook *self, void *data1, void *data2, void *data3);
} hook;

/* Type of variable */
#define V_STRING	0  /* Catch-all category for things unparsable */
#define V_VALUE		1  /* integer */
#define V_BOOL		2  /* TRUE or FALSE */


#ifdef __cplusplus
}
#endif

#endif /* _XIQUAL_COMMON_H */
