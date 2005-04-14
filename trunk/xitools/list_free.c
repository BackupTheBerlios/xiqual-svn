/*
 * Free all data, items and the list itself.
 * Note:If you don't  specify a destructor, you'll have to manually
 * free any data allocated for each node!
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: list_free.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: list_free.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.4  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 * Revision 1.3  2001/12/18 09:25:06  nny
 * Fixed memory leak in lists.
 *
 */

#include "lists.h"


/**
 Free a List and all its attached Node structures.
@doc
 Since list_delete() does the actual freeing of each Node, destructors will be
 called if available.
@param list A struct List to remove all items from
@author Ronny Bangsund
@see List, Node, list_remove(), list_delete()
*/
void list_free(List *list)
{
	Node *next;

	if(!list) return;

	while (list->head) {
		next = list_delete(list, list->head);
	}
	free(list);
}
