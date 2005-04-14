#include "hash.h"
#include "xiqual.h"


/* Create and add a node without copying name string.
 * We don't check if the table is full, either.
 */
HashNode *hash_addnocopy(struct HashTable *h, char *name, void *data)
{
	HashNode *n;
	ulong key;

	n = calloc(1, sizeof(HashNode));
	if(!n) return NULL;

	n->name = name;
	n->data = data;
	n->crc = crc32_calc(name, strlen(name));

	h->count++;
	key = n->crc % h->size;
	if(h->buckets[key]) {
		n->next = h->buckets[key];
	}
	h->buckets[key] = n;
	return n;
}


/**
 Turn a struct List into a struct HashTable.
@doc
 <p>Every node from the list is converted into a hash node,
 then the list is freed. This function assumes that the
 nodes in the list all have data pointers pointing to a
 string. This pointer is copied and cleared to save a
 few allocations.</p>

 <p>If no size is specified, twice the number of list elements
 will be assumed. Default load is two elements.</p>

 <p>hash_new() is used to allocate the list. If you have need
 of larger nodes, you should iterate through your lists and
 do this yourself.</p>

 <p>This function is useful after collecting the contents
 of a dictionary.</p>
@param l A list with nodes pointing their data elements to
 allocated spaces with strings.
@param size Zero for default of twice the list size, or any number
 at least as large as the list. The size will be set to same as
 the list if you specify anything lower.
@param
@return A hash table. If any allocations failed, they're silently ignored.
 You should check whether the count is the same as the list size. NULL if
 memory allocation of the table itself failed.
@author Ronny Bangsund
@see hash_new(), HashTable, List
*/
HashTable *hash_newfromlist(List *l, ulong size, ulong load)
{
	HashTable *h;
	ulong hsize;
	Node *n;

	if(!l) return NULL;

	if(size) {
		hsize = size;
		if(hsize < l->size) hsize = l->size;
	} else {
		hsize = l->size * 2;
	}

	h = hash_new(HASH_SIZE, hsize, HASH_LOAD, load, TAG_END);
	if(!h) return NULL;

	n = l->head;
	while(n) {
		hash_addnocopy(h, n->data, NULL);
		n->data = NULL;
		n = n->next;
	}
	list_free(l);
	return h;
}
