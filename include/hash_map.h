#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#include <stddef.h>

typedef struct hash_map_entry_t {
	char* key;
	void* value;
	struct hash_map_entry_t* next;
} hash_map_entry_t;

typedef struct hash_map_t {
	hash_map_entry_t** buckets;
	size_t bucket_count;
	size_t size;
} hash_map_t;

hash_map_t* hash_map_create(size_t bucket_count);
void hash_map_destroy(hash_map_t* map);

int hash_map_put(hash_map_t* map, const char* key, void* value);
void* hash_map_get(hash_map_t* map, const char* key);
int hash_map_remove(hash_map_t* map, const char* key);

#endif /* HASH_MAP_H_ */
