/*
 * Get the first node in a (possibly circular) list
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: node_start.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: node_start.c,v $
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

#include "xios.h"
#include "lists.h"


/**
 Return the first Node in a circular List.
@doc
 This function will actually return the first Node in a List,
 whether it's circular or not.
@param node A Node used as reference point
@author Shane O'Neill
@see List, Node, list_add(), list_remove(), list_delete(), list_nodemakecircular(), list_nodeend()
*/
Node * node_start(Node *node)
{
	Node *begin;

	begin = node;

	/* 
	 * If list is not circular and this is the start node then return
	 * this node.
	*/
	if (node->prev == NULL)
		return(node);

	 while((node->prev != NULL) && (node->prev != begin)) {
		node = node->prev;
	}
	return(node);
}
