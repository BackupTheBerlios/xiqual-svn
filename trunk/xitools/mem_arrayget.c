#include "xiarray.h"

/**
Access memory as a 1-, 2- or 3-dimensional array.
@doc 
This gets an element from dynamically allocated memory. <br>
The memory can be accessed as a 1, 2 or 3 dimensional array,
depending in the settings of ASpec. <br>
The data pointer in the ASpec structure is of type 'long', 
which means that the memory might have to be 'type cast'. <br>
Also if a finer resolution than 'long' is required that
will be up to the user to mask off the irrelevant bits. <br>
@param x The x offset in the array.
@param y The y offset in the array.
@param z The z offset in the array.
@param ASpec A pointer to an ASpec structure.
@return Element at specified coordinates in array.
@author Shane O'Neill
@see mem_arrayset()
*/
long mem_arrayget(int x, int y, int z, ASpec *a)
{
	int index;
	index = ((z * (a->width * a->height)) + (a->width * y) + x);
	return (a->data[index]);
}
