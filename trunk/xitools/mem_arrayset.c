#include <xiarray.h>

/**
Access memory as a 1-, 2- or 3-dimensional array.
@doc
This sets an element within dynamically allocated memory. <br>
The memory can be accessed as a 1, 2 or 3 dimensional array,
depending in the settings of ASpec. <br>
The data pointer in the ASpec structure is of type 'long', 
which means that the value may have to be 'type cast'. <br>
Setting data will be more complex if a finer resolution is
required. For the moment this will be up to the user to 
sort out.
@param x The x offset in the array.
@param y The y offset in the array.
@param z The z offset in the array.
@param ASpec A pointer to an ASpec structure.
@param val The value to which the array element will be set.
@return Always true
@author Shane O'Neill
@see mem_arrayget()
*/
long mem_arrayset(int x, int y, int z, ASpec *a, long val)
{
	int index;
	index = ((z * (a->width * a->height)) + (a->width * y) + x);
	return (a->data[index] = val);
}
