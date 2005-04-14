/*
 * Insert node after specified node in list
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: list_insert.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: list_insert.c,v $
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
 Insert one Node after another given Node.
@doc
 The Node <em>item</em> will be inserted after the Node prev in <em>list</em>.
 It will not fail, unless you pass NULL pointers. The size of <em>list</em>
 will also be incremented.
@param list A struct List to insert Node item
@param prev Insert item after this
@param item The Node to be inserted into list
@author Ronny Bangsund
@see List, Node, list_new(), list_newitem(), list_add(), list_remove(), list_delete()
*/
void list_insert(List *list, Node *prev, Node *item)
{
	if((!list) || (!item) || (!prev))
		return;

	item->prev = prev;
	item->next = prev->next;
	prev->next = item;
	list->size++;
}
