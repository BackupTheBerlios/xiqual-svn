/*
 * Add node to tail of list
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: list_add.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: list_add.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.4  2002/03/24 06:00:23  nny
 *
 * Merged the latest batch of diffs from SON.
 *
 * Revision 1.3  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 *
 */

#include "lists.h"


/**
 Add a Node to a List.
@doc
 The Node structure passed in is either just a node with space for 
 pointers to previous and next node, and some data, or it can be
 the head of a larger structure. It will be inserted at the bottom
 of the List, as element list->tail.

 Each time an item is added to a List, its size element is increased by one.
 This can be used to instantly tell how many items are in the list.
@param list A struct List to add item to
@param item A Node structure to add to list
@author Ronny Bangsund
@see List, Node, list_new(), list_newitem(), list_insert(), list_remove(), list_delete(),
 list_free(), list_nodemakecircular()
*/
void list_add(List *list, Node *item)
{
	if((!list) || (!item))
		return;

	if (list->head) {
		/* Do the extra voodoo for circular lists */
		if (list->tail->next) {
			item->next = list->head;
			list->head->prev = item;
		}
		list->tail->next = item;
		item->prev = list->tail;
		list->tail = item;
	} else {
		list->tail = item;
		list->head = item;
	}
	list->size++;
}
