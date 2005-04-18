/*
 * Tagtest
 * Demonstrates use of tags and varargs
 *
 */

#include "stdarg.h"
#include "stdio.h"

#include "xiendian.h"
#include "xitags.h"

void ScanlistA(tagitem *tags)
{
	tagitem	*t, *saved;

t=tags;
printf("%ld, %ld\n", t->tag, t->data);
	saved = tags;
	t = tag_next(&tags);	/* tag_next() modifies the pointer passed */
	while(t) {
		printf("Tag: %ld, data:%ld\n", t->tag, t->data);
		t = tag_next(&tags);
	}
	/* Test the tag_finditem() function */
	printf("Looking for 3000...\n");
	/* The functions modify the pointer they are passed! */
	t = saved;
	t = tag_finditem(3000, &t);
	if(t)printf("Found it! Data is %ld\n", t->data);
	/* Now check that the first item in the list can be found
	 * (sanity check)
	 */
	printf("Looking for 1000 (first in list)...\n");
	t = saved;
	t = tag_finditem(1000, &t);
	if(t)printf("Found it! Data is %ld\n", t->data);
}

void Scanlist(unsigned long tag1,  ... )
{
	XI_VOIDARG(ScanlistA((tagitem *)&tag1))
}

int main()
{
//	Scanlist(1000, 1, 2000, 2, 3000, 3, 4000, 4, TAG_END);
	tagitem
	return 0;
}
