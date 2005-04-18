#include "hash.h"


static void hash_schlep(HashTable *old, HashTable *new)
{
	ulong index=0;
	HashNode **buckets, *n;

	if(!old) return;

	buckets = old->buckets;
	while(index < old->size) {
		if(buckets[index]) {
			/* Move the whole chain over */
			n = buckets[index];
			/* Clear the old table entry */
			buckets[index] = NULL;
			while(n) {
				hash_addnode(new, n);
				n = n->next;
			}
		}
	}
	hash_free(old);
}

/**
 Merge any number of hash tables.
@doc
 <p>The parameters are HASH_TABLE tags pointing to existing
 tables you want to merge, plus any of the standard tags
 expected by hash_new(). The nodes from the old tables
 are simply moved over to the newly created table. On any
 recent hardware, this should manage to merge many lists
 of a million entries in a second.</p>

 <p>If no HASH_SIZE tag is specified, the new table will be
 sized after adding together the sizes of the existing
 tables you specify. Remember to specify a larger size than
 the total of the old tables if you need to add more nodes
 to it later.</p>

 <p>On success, all the old hash tables will be freed.</p>
@param tag1 First tag of several.
@return A combination of all supplied hash tables.
@author Ronny Bangsund
*/
HashTable *hash_merge(ulong tag1, ... )
{
	XI_VARARG(hash_mergeA((tagitem *)&tag1), HashTable)
}


HashTable *hash_mergeA(tagitem *tags)
{
	tagitem *alltags, *ti;
	ulong size=0, realsize=0, load=2;
	HashTable *h, *newtable;

	alltags = tags;	/* Save taglist for second round */
	ti = tag_next(&tags);
	while(ti) {
		switch(ti->tag) {
		case HASH_SIZE:
			size = ti->data;
			break;
		case HASH_LOAD:
			if(ti->data) load = ti->data;
			break;
		case HASH_TABLE:
			h = (HashTable *)ti->data;
			realsize += h->count;
		}
		ti = tag_next(&tags);
	}
	if(realsize > size) size = realsize;
	newtable = hash_new(HASH_SIZE, size, HASH_LOAD, load, TAG_END);
	if(!newtable) return NULL;

	ti = tag_next(&alltags);
	while(ti) {
		if(ti->tag == HASH_TABLE) {
			hash_schlep((HashTable *)ti->data, newtable);
		}
		ti = tag_next(&alltags);
	}
	return newtable;
}
