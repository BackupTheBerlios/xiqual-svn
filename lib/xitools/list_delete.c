/*
 * Remove an item from its list and free the memory used.
 * Returns a pointer to the next node in the list, if any.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: list_delete.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: list_delete.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.3  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 * Revision 1.2  2001/12/18 09:25:06  nny
 * Fixed memory leak in lists.
 *
 */

#include "xios.h"
#include "lists.h"


/**
 Remove a Node from a List and free data.
@doc
 If the List has been supplied with a destructor function, this will be
 called with item as its parameter. Otherwise, a simple call to free() will be made.
@param list A struct List to remove item from, and also get a pointer to the destructor
@param item A Node structure to remove from list
@return A pointer to the next Node after item, or NULL.
@author Ronny Bangsund
@see List, Node, list_new(), list_newitem(), list_add(), list_remove(), list_free()
*/
Node *list_delete(List *list, Node *item)
{
	Node *next;

	if(!list) return NULL;

	next = list_remove(list, item);
	if (list->destructor) {
		list->destructor(item);
	} else {
		free(item);
	}
	return next;
}
