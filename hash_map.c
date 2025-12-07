#include <stdlib.h>
#include <string.h>

#include "hash_map.h"

#define DEFAULT_BUCKET_COUNT 32

static size_t hash_map_index(const hash_map_t* map, const char* key) {
	size_t hash = 5381;
	const unsigned char* walker = (const unsigned char*) key;
	while(*walker != '\0') {
		hash = ((hash << 5) + hash) ^ *walker;
		walker++;
	}
	return hash % map->bucket_count;
}

static char* hash_map_strdup(const char* source) {
	size_t length = 0;
	char* copy = NULL;

	if(source == NULL) {
		return NULL;
	}

	length = strlen(source) + 1;
	copy = (char*) malloc(length);
	if(copy != NULL) {
		memcpy(copy, source, length);
	}
	return copy;
}

hash_map_t* hash_map_create(size_t bucket_count) {
	hash_map_t* map = NULL;
	size_t actual = bucket_count == 0 ? DEFAULT_BUCKET_COUNT : bucket_count;

	map = (hash_map_t*) malloc(sizeof(hash_map_t));
	if(map == NULL) {
		return NULL;
	}

	map->buckets = (hash_map_entry_t**) calloc(actual, sizeof(hash_map_entry_t*));
	if(map->buckets == NULL) {
		free(map);
		return NULL;
	}

	map->bucket_count = actual;
	map->size = 0;
	return map;
}

void hash_map_destroy(hash_map_t* map) {
	size_t i = 0;
	hash_map_entry_t* entry = NULL;
	hash_map_entry_t* next = NULL;

	if(map == NULL) {
		return;
	}

	for(i = 0; i < map->bucket_count; i++) {
		entry = map->buckets[i];
		while(entry != NULL) {
			next = entry->next;
			free(entry->key);
			free(entry);
			entry = next;
		}
	}

	free(map->buckets);
	free(map);
}

int hash_map_put(hash_map_t* map, const char* key, void* value) {
	size_t index = 0;
	hash_map_entry_t* entry = NULL;

	if(map == NULL || key == NULL) {
		return -1;
	}

	index = hash_map_index(map, key);
	entry = map->buckets[index];
	while(entry != NULL) {
		if(strcmp(entry->key, key) == 0) {
			return -1;
		}
		entry = entry->next;
	}

	entry = (hash_map_entry_t*) malloc(sizeof(hash_map_entry_t));
	if(entry == NULL) {
		return -1;
	}

	entry->key = hash_map_strdup(key);
	if(entry->key == NULL) {
		free(entry);
		return -1;
	}
	entry->value = value;
	entry->next = map->buckets[index];
	map->buckets[index] = entry;
	map->size++;
	return 0;
}

void* hash_map_get(hash_map_t* map, const char* key) {
	size_t index = 0;
	hash_map_entry_t* entry = NULL;

	if(map == NULL || key == NULL) {
		return NULL;
	}

	index = hash_map_index(map, key);
	entry = map->buckets[index];
	while(entry != NULL) {
		if(strcmp(entry->key, key) == 0) {
			return entry->value;
		}
		entry = entry->next;
	}

	return NULL;
}

int hash_map_remove(hash_map_t* map, const char* key) {
	size_t index = 0;
	hash_map_entry_t* entry = NULL;
	hash_map_entry_t* prev = NULL;

	if(map == NULL || key == NULL) {
		return -1;
	}

	index = hash_map_index(map, key);
	entry = map->buckets[index];
	prev = NULL;
	while(entry != NULL) {
		if(strcmp(entry->key, key) == 0) {
			if(prev == NULL) {
				map->buckets[index] = entry->next;
			} else {
				prev->next = entry->next;
			}
			free(entry->key);
			free(entry);
			if(map->size > 0) {
				map->size--;
			}
			return 0;
		}
		prev = entry;
		entry = entry->next;
	}

	return -1;
}
