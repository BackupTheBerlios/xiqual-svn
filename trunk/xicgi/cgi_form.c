/*
 * Decode a form passed by either GET or POST.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: cgi_form.c,v 1.3 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: cgi_form.c,v $
 * Revision 1.3  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.2  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.11  2001/12/19 09:00:57  nny
 * Pruned logs.
 *
 *
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "lists.h"
#include "xicgi.h"

/*
 * This call returns a List structure, as defined in lists.h from libxitools.
 * Simply calling list_free() on the list will free all data.
 */
static void cgi_delete(cgivar *var)
{
	if(!var)
		return;

	free(var->name);
	free(var->value);
	free(var);
}

/* Use cgi_checkform() to fill in method/query details */
List *cgi_form()
{
	cgivar	*var;
	List	*list;
	long	count=0;
	char	*nameptr, *valptr=0;

	list = list_new(cgi_delete);
	if(!list)
		return NULL;

	/* If the method is POST, copy the input to cgi->query */
	if(strcmp(cgi->method, "POST")==0) {
		cgi->query = calloc(1, cgi->contentlength+1);
		if(!cgi->query) {
			return 0;
		}
		cgi->contentlength = fread(cgi->query, 1, cgi->contentlength, stdin);
	}

	/* Now process it the same whether it's GET or POST */
	nameptr = cgi->query;
	while(count < cgi->contentlength+1) {
		switch(cgi->query[count]) {
		case '=':
			cgi->query[count] = 0;
			count++;
			valptr = cgi->query+count;
			break;

		case 0:
		case '&':
		case ';':		/* The O'Reilly tome on the subject of HTML encourages semicolons as valid separators */
			cgi->query[count] = 0;
			var = calloc(1, sizeof(cgivar));
			if(!var) {
				return list;
			}
			var->name = calloc(1, strlen(nameptr)+1);
			if(!var->name) {
				free(var);
				return list;
			}
			if(valptr) {
				var->value = calloc(1, strlen(valptr)+1);
				if(!var->value) {
					free(var->name);
					free(var);
					return list;
				}
			}
			cgi_unescape(nameptr, var->name);
			count++;
			nameptr = cgi->query+count;
			if(valptr) {
				cgi_unescape(valptr, var->value);
			} else {
				var->value = 0;
			}
			valptr = 0;
			list_add(list, (Node *)var);
			break;
		default:
			count++;
		}
	}
	return list;
}
