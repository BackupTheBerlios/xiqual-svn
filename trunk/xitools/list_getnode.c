/*
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: list_getnode.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: list_getnode.c,v $
 * Revision 1.2  2003/05/30 08:54:59  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 * Revision 1.1  2002/03/24 06:00:23  nny
 *
 * Merged the latest batch of diffs from SON.
 *
*/

#include "lists.h"

/**
 Return a Node at a specific index position.
@doc
 This will simply traverse the List until either a Node is found at position index,
 or there are no more items in the List.
@param list A struct List to get a Node from
@param index Node number to get a pointer to
@return A pointer to a Node, or NULL if the index was too high
@author Shane O'Neill
@see List, Node, list_new(), list_newitem(), list_add(), list_insert()
*/
Node * list_getnode(List *list, int index)
{
	int i;
	Node *node;

	if(!list)
		return NULL;

	node = list->head;
	
	for (i = 0; i < index; i++) {
		if (!node->next) return(node);

		node = node->next;
	}
	return(node);
}
