#include "xios.h"
#include "lists.h"


/**
 Return a Node with data <em>partially</em> containing the entire specified string.
@doc
 This function is useful to find nodes that contain more than just the
 data you are looking for. Safe to call on a circular chain of nodes.
@param node A Node to start searching at.
@param nodename String that must exist in node->data.
@author Ronny Bangsund
@see List, Node, preferences, variable, statuscleaner.c
*/
Node *node_findbycontents(Node *node, char *nodename)
{
	Node *begin, *n;

	begin = node;

	n = node;
	do {
		if(n->data) {
			if(strstr(n->data, nodename)) return n;
		}

		n = n->next;
	} while((n) && (n != begin));
	return NULL;
}
