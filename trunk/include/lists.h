/*
 * List functions: specification.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: lists.h,v 1.11 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: lists.h,v $
 * Revision 1.11  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.10  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.7  2002/03/24 06:00:23  nny
 *
 * Merged the latest batch of diffs from SON.
 *
 */

#if !defined(_XIQUAL_LISTS_H)
#define _XIQUAL_LISTS_H

#include "xiendian.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 A simple Node.
@doc
 Every programmer probably knows about lists and nodes, so I won't elaborate.

 Xiqual List functions make no assumptions about the contents and size of a
 Node structure passed to them. If you have larger structures with data to be
 freed, the destructor callback is useful.
@author Ronny Bangsund
@see List
*/
typedef struct Node {
/**
 Next node in List.
@doc
 It can be any structure starting with a Node structure, as Xiqual does not
 assume anything about data beyond the Node header.
*/
	struct Node	*next;
/**
 Previous Node in List.
@doc
 It can be any structure starting with a Node structure, as Xiqual does not
 assume anything about data beyond the Node header.
*/
	struct Node	*prev;
/**
 Node-specific data.
@doc
 Xiqual routines use this pointer as a string pointer to a name,
 or a pointer to sound/graphics data.
*/
	void	*data;
} Node;

/**
 Linked list structure.
@doc
 The first Node will not have a previous Node pointer,
 and the last will have no next Node pointer. Use the
 list_makecircular() function to make it loop.
@author Ronny Bangsund
@see Node, list_new(), list_add(),  list_insert()
*/
typedef struct List {
/**
 First Node in List.
*/
	Node	*head;
/**
 Last Node in List.
*/
	Node	*tail;
/**
 Number of items in List.
*/
	ulong	size;
/**
 A destructor callback for items (optional).
*/
	void	(*destructor)(void *);
} List;

extern List *list_new(void (*destructor)() );
extern void list_free(List *list);

extern Node *list_newitem(List *list, void *data);
extern void list_insert(List *list, Node *prev, Node *item);
extern void list_add(List *list, Node *item);
extern void list_addhead(List *list, Node *item);
extern Node *list_remove(List *list, Node *item);
extern Node *list_delete(List *list, Node *item);
extern void list_makecircular(List *list);
extern Node * list_getnode(List *list, int index);

extern int node_makecircular(Node *);
extern Node *node_start(Node *);
extern Node *node_end(Node *);

extern Node *node_findbyname(Node *node, char *nodename);
extern Node *node_findbycontents(Node *node, char *nodename);

/**
 Wrapper for node_makecircular().
@see node_makecircular()
*/
#define list_nodemakecircular(n)	node_makecircular(n)
/**
 Wrapper for node_start().
@see node_start()
*/
#define list_nodestart(n)	node_start(n)
/**
 Wrapper for node_end().
@see node_end()
*/
#define list_nodeend(n)	node_end(n)

#ifdef __cplusplus
}
#endif

#endif /* EOF */
