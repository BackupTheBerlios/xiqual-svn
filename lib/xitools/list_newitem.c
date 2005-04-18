/*
 * Allocate a new Node.
 * Node->data will be filled in if data is specified
 * The item will be added to the end of a list if that list is specified.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: list_newitem.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: list_newitem.c,v $
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
 * Revision 1.4  2002/03/24 06:00:23  nny
 *
 * Merged the latest batch of diffs from SON.
 *
 * Revision 1.3  2001/12/19 09:01:02  nny
 * Pruned logs.
 *
 */

#include "lists.h"


/**
 Allocates a new Node structure.
@doc
 The node will be initialised with data and inserted into list.
 If <em>list</em> is NULL, it merely creates the new Node and
 returns a pointer to it.
@param list An optional struct List to insert Node item into
@param data Data for the data element in the created node. Optional.
@return A pointer to a Node, with all three fields initialised
@author Ronny Bangsund
@see List, Node, list_new(), list_add(), list_remove(), list_delete()
*/
Node *list_newitem(List *list, void *data)
{
	Node *item;

	item = (Node *)calloc(1, sizeof(Node));
	if (item) {
		item->next = NULL;
		item->prev = NULL;
		if (list) {
			list_add(list, item);
		}
		item->data = data;
	}
	return item;
}
