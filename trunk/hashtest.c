/*
 * Test Xiqual hashtables.
 * (Very fast lookup of strings)
 */

#include "hash.h"
#include "xiqual.h"
#include "time.h"


int main()
{
	HashNode *node;
	HashTable *hash;
	ulong i;
	char name[200];
	time_t t, t2;

	/* The lower the load, the faster the lookup.
	 * Optimal load would be 2 (the default), since
	 * You can't hope to have exactly one entry per
	 * available bucket - some buckets will have two
	 * or more entries, while using load = 1 would
	 * almost guarantee unused buckets. It would be
	 * as fast as it can be, although not by much.
	 */
	hash = hash_new(HASH_SIZE, 200000, HASH_LOAD, 2, TAG_END);
	if(!hash) {
		fprintf(stderr, "Allocation of hash table failed.\n");
		return 2;
	}

	for(i=0; i<400000; i++) {
		snprintf(name, 200, "Node #%ld", i+1);
		node = hash_add(hash, name, NULL);
		if(!node) {
			if(xi_lasterror	) {
				printf("Failed at #%ld: %s\n", i+1, xi_lasterror);
			} else {
				printf("Failed due to unknown error at allocation #%ld\n", i+1);
			}
		}
	}

	node = hash_find(hash, "Node #5");
	if(node) {
		printf("Found %s at 0x%p\n", node->name, node);
	}

	node = hash_find(hash, "Node #13");
	if(node) {
		printf("Found %s at 0x%p\n", node->name, node);
	} else {
		printf("Node #13 not in table\n");
	}

	node = hash_find(hash, "Node #65000");
	if(node) {
		printf("Found %s at 0x%p\n", node->name, node);
	} else {
		printf("Node #65000 not in table\n");
	}

	node = hash_find(hash, "Node #380000");
	if(node) {
		printf("Found %s at 0x%p\n", node->name, node);
	} else {
		printf("Node #380000 not in table\n");
	}

	t = time(NULL);
	printf("Doing 12000000 lookups..");
	for(i=0; i<12000000; i++) {
		node = hash_find(hash, "Node #65000");
	}
	t2 = time(NULL);
	printf("done in %ld seconds.\n", t2 - t);

	hash_free(hash);
	return 0;
}
