/*
 * Remove an item from a list, piecing together the list pointers afterwards
 * Returns a pointer to the next node, if any.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: list_remove.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: list_remove.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.4  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 * Revision 1.3  2001/12/14 08:39:21  nny
 * I discovered that I hadn't isolated the freeing of nodes yet. The work
 * to fix references now begins.
 *
 */

#include "lists.h"


/**
 Remove a Node from a List without freeing any data.
@param list A struct List to remove item from
@param item A Node structure to remove from list
@return A pointer to the next Node after item, or NULL.
@author Ronny Bangsund
@see List, Node, list_new(), list_newitem(), list_add(), list_delete(), list_free()
*/
Node *list_remove(List *list, Node *item)
{
	Node *next, *prev;

	if((!list) || (!item)) return NULL;

	next = item->next;
	prev = item->prev;

	if (prev) {
		prev->next = next;
	} else {
		list->head = next;
	}

	if (next) {
		next->prev = prev;
	} else {
		list->tail =prev;
	}
	list->size--;
	item->next = NULL;
	item->prev = NULL;
	return next;
}
