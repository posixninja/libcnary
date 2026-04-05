/*
 * hashmap.h
 *
 *  Created on: Apr 4, 2026
 *      Author: posixninja
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_

#define HASHMAP_NUM_BUCKETS 64

typedef struct hashmap_entry_t {
	char* key;
	void* value;
	struct hashmap_entry_t* next;
} hashmap_entry_t;

typedef struct hashmap_t {
	hashmap_entry_t** buckets;
	unsigned int count;
	unsigned int num_buckets;
} hashmap_t;

hashmap_t* hashmap_create(void);
void hashmap_destroy(hashmap_t* map);

int hashmap_set(hashmap_t* map, const char* key, void* value);
void* hashmap_get(hashmap_t* map, const char* key);
int hashmap_remove(hashmap_t* map, const char* key);

#endif /* HASHMAP_H_ */
