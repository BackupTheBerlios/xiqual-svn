#include "xios.h"
#include "lists.h"


/**
 Return a Node with data containing the specified string.
@doc
 This function is useful to find parts of preferences structures.
 Safe to call on a circular chain of nodes. NOTE: The search starts
 at the given node.
@param node A Node to start searching at.
@param nodename Name of node you are looking for.
@author Ronny Bangsund, Shane O'Neill
@see List, Node, preferences, variable
*/
Node *node_findbyname(Node *node, char *nodename)
{
	Node *begin, *n;

	if((!node) || (!nodename)) return NULL;

	begin = node;

	n = node;
	do {
		if(n->data) {
			if(strcasecmp(n->data, nodename) == 0) return n;
		}

		n = n->next;
	} while((n) && (n != begin));
	return NULL;
}
