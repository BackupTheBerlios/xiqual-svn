/*
 * Simple hash tables.
 *
 * Allocates one table that does not change
 * in size at all.
 */

#include "hash.h"
#include "xiqual.h"

/**@name <div class="keyword">Hash table functions<span>Fast lookup</span></div>
 Fast string lookup.
@doc
 <p>When you have many (tens of thousands or millions) of uniquely
 named entries, hash tables are good. In the optimal case, every
 string lookup takes the same amount of (minimal) time. For hash
 tables to be any good, every entry should have a different name
 to be sorted by. The current implementation only allows entries
 to have strings as base for their key.<p>

 <p>When you create a table with hash_new(), you supply a number of
 tags (keyword and data pairs) which define the number of entries
 you expect to top out at.</p>

 <p>If your table reaches this hard limit, the regular node adding
 function, hash_add(), refuses to add anymore nodes. You can
 circumvent this by using the ignorant function hash_add() at the
 cost of performance, or you could allocate a bigger table and
 start shuffling the old data over to this.</p>

*/

/*@{*/
/**
 Create a fixed-size hash table.
@doc
 <h2>Tags</h2>
 <p>Like many other Xiqual functions, hash_new() requires a set of
 tags as its parameters. See the hashtest.c source for an example.
 The recognised tags are as follows:</p>

 <ul>
 <li>HASH_SIZE</li>
 <p>Number of buckets in the hash table. Multiply by load to find
 the maximum number of elements normally allowed.</p>

 <li>HASH_LOAD</li>
 <p>Number of elements per bucket. This is only an average, as some
 buckets might only get one elements, and others get three or more.
 Load multiplied by size is the hard limit.</p>

 <li>HASH_NODESIZE</li>
 <p>If you require more space in each node, you can extend the
 existing node structure and specify the new size here. Your new
 node must start with exactly the same fields as struct HashNode.</p>
 </ul>
@param tag1 First tag.
@return A blank hash table of the specified size, or NULL on failure.
@author Ronny Bangsund
@see hash_free(), hash_find(), hash_add(), hash_newnode(),
 HashTable, HashNode
*/
HashTable *hash_new(ulong tag1, ... )
{
	XI_VARARG(hash_newA((tagitem *)&tag1), HashTable)
}


HashTable *hash_newA(tagitem *tags)
{
	tagitem *ti;
	HashTable *h;
	ulong size=512, load=2, nodesize=sizeof(HashNode);

	ti = tag_next(&tags);
	while(ti) {
		switch(ti->tag) {
		case HASH_SIZE:
			size = ti->data;
			break;
		case HASH_LOAD:
			if(ti->data) load = ti->data;
			break;
		case HASH_NODESIZE:
			nodesize = ti->data;
		}
		ti = tag_next(&tags);
	}

	h = calloc(1, sizeof(HashTable));
	if(!h) return NULL;

	h->size = size;
	h->load = load;
	h->nodesize = nodesize;
	h->buckets = calloc(sizeof(HashNode *), h->size);
	if(!h->buckets) {
		hash_free(h);
		return NULL;
	}
	return h;
}


/**
 Free a hash table.
@doc
 All nodes and the table itself are freed.
@param h struct HashTable to free.
@author Ronny Bangsund
@see hash_new(), HashTable, HashNode
*/
void hash_free(struct HashTable *h)
{
	ulong index=0;
	HashNode *n;

	if(!h) return;

	if(!h->buckets) {
		free(h);
		return;
	}

	while(index < h->size) {
		while(h->buckets[index]) {
			n = h->buckets[index]->next;
			hash_freenode(h->buckets[index]);
			h->buckets[index] = n;
		}
		index++;
	}
	free(h->buckets);
	free(h);
}


/**
 Creates a new node with specified name and data.
@doc
 The name is duplicated with xi_strndup().
@param name String to sort by.
@param data Any data you desire that fits in a void pointer.
@return A new node with a copy of the specified name and data pointer.
@author Ronny Bangsund
@see hash_new(), crc32_calc(), xi_strndup(), HashTable, HashNode
*/
HashNode *hash_newnode(const char *name, void *data, ulong size)
{
	HashNode *n;
	size_t namelen;

	n = calloc(1, size);
	if(!n) return NULL;

	n->data = data;

	namelen = strlen(name);
	n->name = strndup(name, namelen);
	n->crc = crc32_calc(n->name, namelen);
	if(!n->name) {
		free(n);
		return NULL;
	}
	return n;
}


/**
 Deallocate a node.
@doc
 Data is not cleared, only deallocated.
@author Ronny Bangsund
@see hash_newnode(), HashNode
*/
void hash_freenode(HashNode *n)
{
	if(!n) return;
	if(n->name) free(n->name);
	free(n);
}


/**
 Ignorantly add a node to a hash table.
@doc
 This function is blithely ignorant of table limits.
 You may add any number of nodes to a hash table,
 but prepare for decreasing performance as the chains
 grow larger.
@param h Hash table to add node to.
@param n Node returned by hash_newnode().
@author Ronny Bangsund
@see hash_newnode(), hash_merge(), HashNode, HashTable
*/
void hash_addnode(struct HashTable *h, HashNode *n)
{
	ulong key;

	h->count++;
	key = n->crc % h->size;
	if(h->buckets[key]) {
		n->next = h->buckets[key];
	}
	h->buckets[key] = n;
}


/**
 Create node with specified data and add to a hash table.
@doc
 The name will be duplicated. hash_newnode() is used to
 actually allocate the node. If the table is full, no node
 is added.
@param h Hash table to add to.
@param name Name to sort node by.
@param data Any (void *) sized data you desire.
@return The node created, or NULL on error. xi_lasterror may
 have meaningful text describing the problem. Only a table full
 error returns a string yet.
@author Ronny Bangsund
*/
HashNode *hash_add(struct HashTable *h, char *name, void *data)
{
	HashNode *n;
	ulong key;

	n = hash_newnode(name, data, h->nodesize);
	if(!n) return NULL;

	if(h->count == (h->size * h->load)) {
		xi_seterror("Hash table full");
		free(n);
		return NULL;
	}

	h->count++;
	key = n->crc % h->size;
	if(h->buckets[key]) {
		n->next = h->buckets[key];
	}
	h->buckets[key] = n;
	return n;
}


/**
 Look up a node in a hash table.
@doc
 Generates a key from the name and checks the resulting
 bucket for nodes matching the specified name.
@param h Hash table to look in.
@param name A string to search for.
@return Either a hash node, or NULL if not found.
@author Ronny Bangsund
*/
HashNode *hash_find(HashTable *h, char *name)
{
	ulong crc, key;
	HashNode *n;

	crc = crc32_calc(name, strlen(name));
	key = crc % h->size;
	if(h->buckets[key] == 0) return NULL;

	n = h->buckets[key];
	while(n) {
		if(n->crc == crc) {
			if(strcmp(n->name, name)==0) return n;
		}
		n = n->next;
	}
	return NULL;
}

/*@}*/
