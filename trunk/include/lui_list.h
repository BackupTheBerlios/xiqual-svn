/*
 * Button structure, tags and functions.
 */

#if !defined(_LUI_LIST_H)
#define _LUI_LIST_H

#include "iarea.h"
#include "lists.h"
#include "views.h"
#include "xitags.h"

#include "lui.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct lui_list {
	/* Node structure */
	struct lui_list	*next;
	struct lui_list	*prev;
	char	*name;

	/* LUI elements common to all gadgets */
	void	(*add)(List *, List *, struct lui_list *);
	void	(*rem)(List *, List *, struct lui_list *);
	void	(*destroy)(struct lui_list *);
	void	(*activate)(struct lui_list *);
	void	(*draw)(struct lui_list *);
	void	(*highlight)(struct lui_list *);
	void	(*ghost)(struct lui_list *);

	void	*data;
	ulong	id;
	ulong	type;
	ulong	flags;
	int	w, h, x, y;
	inputarea	*ia;
	SDL_Surface	*screen;

	/* Gadget-specific elements */
	int	bw, bh;
	ulong	first;

	SDL_Surface	*bg;
	xiview	*upclick, *downclick;
	inputarea	*up, *down;

	void	(*onactivate)(lui_input *, struct lui_list *);
	/* Pass first element, surface and user-definable data
	 * to draw hook when drawing the list contents.
	 */
	void	(*ondraw)(int, SDL_Surface *, void *);
	/* Up and down functions are passed the current first
	 * item plus user-defined data, and must return the
	 * number of the new first item.
	 */
	int	(*onup)(ulong, void *);
	int	(*ondown)(ulong, void *);

	xiview	*view;

} lui_list;


/* Tags */
enum {
	LST_DRAWHOOK=(('L'<<24) | ('I'<<16) | ('S'<<8) | ('T')),
	LST_UPHOOK,
	LST_DOWNHOOK,
	LST_BORDERWIDTH,	/* Basically x-offset for inner area */
	LST_BORDERHEIGHT,
	LST_IMAGE,	/* Background (entire list area) */
	LST_UPIMAGE,	/* Up arrow (if a new one is needed) */
	LST_UPCLICKIMAGE,	/* Up arrow when clicked */
	LST_DOWNIMAGE,
	LST_DOWNCLICKIMAGE,
	LST_BGCOL,
	LST_ORIENTATION		/* To be implemented soon */
};


/* Public functions */
extern lui_list *lst_new(ulong tag1, ... );
extern lui_list *lst_newA(tagitem *taglist);
extern void lst_add(List *inputs, List *lists, lui_list *l);
extern void lst_rem(List *inputs, List *lists, lui_list *l);
extern void lst_free(lui_list *list);
extern void lst_activate(lui_list *lst);
void lst_draw(lui_list *list);
void lst_upclick(lui_list *lst);
void lst_uprelease(lui_list *lst);
void lst_downclick(lui_list *lst);
void lst_downrelease(lui_list *lst);

#if defined(__cplusplus)
}
#endif

#endif	/* EOF */
