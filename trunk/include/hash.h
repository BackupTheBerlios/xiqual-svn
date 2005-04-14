/* 
 * Hashing and CRC32
 */

#if !defined(XIQUAL_HASH_H)
#define XIQUAL_HASH_H

#include "xios.h"
#include "lists.h"
#include "xitags.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 Basic hash node.
@doc
 The structure retains its CRC for faster lookup
 and recalculation when merging hash tables.
@see hash_newnode(), crc32_calc(), HashTable
*/
typedef struct HashNode {
/**
 Next node in chain.
@doc
 There is no pointer to the next node in
 the hash table; you must iterate through
 the buckets manually. Sorry.
*/
	struct HashNode *next;
/**
 Name of node.
@doc
 This is the string used to generate the node's key,
 which in turn determines what bucket to stick it in.
*/
	char *name;
/**
 CRC of the name.
@doc
 Used internally for faster comparison in hash_find().
 Also used to quickly recalculate position when the
 node moves to a new hash table.
*/
	ulong crc;
/**
 User-definable data.
@doc
 Hash nodes would be pretty useless without this field.
*/
	void *data;
} HashNode;


/**
 Non-resizing hash table.
@doc
 Better make sure you allocate enough space for all
 the data you need, or use the forthcoming extension
 called "hash carousels".
@see hash_new(), HashNode
*/
typedef struct HashTable {
/**
 Number of buckets.
@doc
 A bucket holds approximately the number of nodes
 specified by load on creation. This is not a real
 guarantee, though, as the hashing algorithm can't
 guarantee complete useless in hashes.
*/
	ulong	size;
/**
 Maximum number of elements per bucket.
@doc
 When the average elements per bucket reaches
 this number, the hash table is definitely full.

 The current implementation refuses to add nodes
 beyond this point.
*/
	ulong	load;
/**
 Number of elements right now
*/
	ulong	count;
/**
 Size of nodes.
@doc
 This allows you to have custom nodes - the only
 requirement is that the first fields are the same
 as the HashNode structure.
*/
	ulong	nodesize;
/**
 The actual hash array
@doc
 Each bucket is a pointer to one or more hash nodes.
 Specify load to set an approximate maximum.
*/
	struct HashNode **buckets;
} HashTable;


extern ulong crc32_update(unsigned char *octet, ulong crc);
extern ulong crc32_calc(char *buf, size_t len);

extern HashTable *hash_new(ulong tag1, ... );
extern HashTable *hash_newA(tagitem *tags);
extern void hash_free(struct HashTable *hash);

extern HashNode *hash_newnode(const char *name, void *data, ulong size);
extern void hash_freenode(HashNode *n);
extern HashNode *hash_add(struct HashTable *hash, char *name, void *data);
extern HashNode *hash_find(HashTable *hash, char *name);
extern void hash_addnode(struct HashTable *h, HashNode *n);
extern HashTable *hash_newfromlist(List *l, ulong size, ulong load);

extern HashTable *hash_merge(ulong tag1, ... );
extern HashTable *hash_mergeA(tagitem *tags);


/* Tag IDs */
enum {
	HASH_SIZE =			/* Buckets */
	 (('H'<<24) | ('A'<<16) | ('S'<<8) | 'H'),
	HASH_LOAD,			/* Elements per bucket limit (average) */
	HASH_NODESIZE,		/* For custom nodes */
	HASH_TABLE			/* Used in hash_merge() */
};

#ifdef __cplusplus
}
#endif

#endif /* EOF */
