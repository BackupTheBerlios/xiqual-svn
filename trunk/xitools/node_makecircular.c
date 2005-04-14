/*
 * $Author: nny $
 * $Date: 2003/05/30 08:54:59 $
 * $Id: node_makecircular.c,v 1.2 2003/05/30 08:54:59 nny Exp $
 *
 * $Log: node_makecircular.c,v $
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
 Make a chain of nodes circular (first and last Node points to eachother).
@doc
 A circular list is used mostly for efficient looping of animations
 (which means only a typical, lazy programmer could have thought of it).
 This function does not necessarily require a List structure.

 Do NOT call on a Node in an already circular chain. That's a waste of time ;)
@param node A Node used as reference point.
@author Shane O'Neill
@see List, Node, list_add(), list_remove(), list_delete(), list_nodeend(), list_nodestart()
*/
int node_makecircular(Node *node)
{
	Node *nodestart;
	Node *nodeend;

	nodestart = list_nodestart(node);
	nodeend = list_nodeend(node);

	nodestart->prev = nodeend;
	nodeend->next = nodestart;

	return 0;
}
