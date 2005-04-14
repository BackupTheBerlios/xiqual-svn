#if !defined(HAVE_XI_ARRAY_H)
#define HAVE_XI_ARRAY_H

//! Array specification structure.
/**
Array specification structure
@doc The structure used by the mem_array* functions.
@author Shane O'Neill
*/

typedef struct aspec {
/** The amount of columns in the array.*/
	long	width;
/** The amount of rows in the array. */
	long	height;
/** This should be pointed to the memory to be accessed. */
	long	*data;
}ASpec;


#ifdef __cplusplus
extern "C" {
#endif

extern long mem_arrayget(int, int, int, ASpec *);
extern long mem_arrayset(int, int, int, ASpec *, long);

#ifdef __cplusplus
}
#endif

#endif
