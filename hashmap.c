/*
 * hashmap.c
 *
 *  Created on: Apr 4, 2026
 *      Author: posixninja
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

static unsigned int hashmap_hash(const char* key, unsigned int num_buckets) {
	unsigned int hash = 5381;
	int c;
	while ((c = (unsigned char)*key++)) {
		hash = ((hash << 5) + hash) + c; /* djb2: hash * 33 + c */
	}
	return hash % num_buckets;
}

hashmap_t* hashmap_create(void) {
	hashmap_t* map = (hashmap_t*) malloc(sizeof(hashmap_t));
	if (map == NULL) {
		return NULL;
	}
	map->num_buckets = HASHMAP_NUM_BUCKETS;
	map->count = 0;
	map->buckets = (hashmap_entry_t**) calloc(map->num_buckets, sizeof(hashmap_entry_t*));
	if (map->buckets == NULL) {
		free(map);
		return NULL;
	}
	return map;
}

void hashmap_destroy(hashmap_t* map) {
	unsigned int i;
	hashmap_entry_t* entry;
	hashmap_entry_t* next;

	if (map == NULL) {
		return;
	}
	for (i = 0; i < map->num_buckets; i++) {
		entry = map->buckets[i];
		while (entry != NULL) {
			next = entry->next;
			free(entry->key);
			free(entry);
			entry = next;
		}
	}
	free(map->buckets);
	free(map);
}

int hashmap_set(hashmap_t* map, const char* key, void* value) {
	unsigned int index;
	hashmap_entry_t* entry;
	hashmap_entry_t* new_entry;

	if (map == NULL || key == NULL) {
		return -1;
	}
	index = hashmap_hash(key, map->num_buckets);
	entry = map->buckets[index];

	/* Update existing entry if key already present */
	while (entry != NULL) {
		if (strcmp(entry->key, key) == 0) {
			entry->value = value;
			return 0;
		}
		entry = entry->next;
	}

	/* Insert new entry at head of bucket chain */
	new_entry = (hashmap_entry_t*) malloc(sizeof(hashmap_entry_t));
	if (new_entry == NULL) {
		return -1;
	}
	new_entry->key = strdup(key);
	if (new_entry->key == NULL) {
		free(new_entry);
		return -1;
	}
	new_entry->value = value;
	new_entry->next = map->buckets[index];
	map->buckets[index] = new_entry;
	map->count++;
	return 0;
}

void* hashmap_get(hashmap_t* map, const char* key) {
	unsigned int index;
	hashmap_entry_t* entry;

	if (map == NULL || key == NULL) {
		return NULL;
	}
	index = hashmap_hash(key, map->num_buckets);
	entry = map->buckets[index];
	while (entry != NULL) {
		if (strcmp(entry->key, key) == 0) {
			return entry->value;
		}
		entry = entry->next;
	}
	return NULL;
}

int hashmap_remove(hashmap_t* map, const char* key) {
	unsigned int index;
	hashmap_entry_t* entry;
	hashmap_entry_t* prev;

	if (map == NULL || key == NULL) {
		return -1;
	}
	index = hashmap_hash(key, map->num_buckets);
	entry = map->buckets[index];
	prev = NULL;

	while (entry != NULL) {
		if (strcmp(entry->key, key) == 0) {
			if (prev != NULL) {
				prev->next = entry->next;
			} else {
				map->buckets[index] = entry->next;
			}
			free(entry->key);
			free(entry);
			map->count--;
			return 0;
		}
		prev = entry;
		entry = entry->next;
	}
	return -1;
}
