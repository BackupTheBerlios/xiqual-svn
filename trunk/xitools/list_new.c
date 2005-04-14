/*
 * Create a new list and initialise
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: list_new.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: list_new.c,v $
 * Revision 1.4  2003/12/11 08:54:55  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.3  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "lists.h"


/**
 Create a new List and set the destructor.
@doc
 If no destructor is specified, a simple free() will be called on items
 in list_delete().
@param destructor A function used to delete each Node. The only parameter is a Node pointer.
@return If all is well, a pointer to a List
@author Ronny Bangsund
@see List, Node, list_newitem(), list_add(), list_remove(), list_delete(), list_free()
*/
List *list_new(void (*destructor)())
{
	List *list;
	list = (List *)calloc(1, sizeof(List));
	if (list) {
		memset(list, 0, sizeof(List));
		list->destructor = (void *)destructor;
	}
	return list;
}
