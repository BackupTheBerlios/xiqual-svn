/*
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: list_makecircular.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: list_makecircular.c,v $
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
 *
*/

#include "xios.h"
#include "lists.h"


/**
 Make a List circular (first and last nodes point to eachother).
@doc
 A circular list is used mostly for efficient looping of animations
 (which means only a typical, lazy programmer could have thought of it).
@param node A Node used as reference point
@author Shane O'Neill, Ronny Bangsund
@see List, Node, list_nodemakecircular()
*/
void list_makecircular(List *list)
{
	if(!list)
		return;

	list->head->prev = list->tail;
	list->tail->next = list->head;
}
