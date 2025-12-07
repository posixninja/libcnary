/*
 * node_list.c
 *
 *  Created on: Mar 8, 2011
 *      Author: posixninja
 */

#include <stdlib.h>
#include <string.h>

#include "hash_map.h"
#include "node.h"
#include "node_list.h"

void node_list_destroy(node_list_t* list) {
	node_t* current = NULL;
	node_t* next = NULL;
	if(list == NULL) {
		return;
	}

	current = list->begin;
	while(current != NULL) {
		next = current->next;
		current->next = NULL;
		current->prev = NULL;
		current = next;
	}

	if(list->lookup != NULL) {
		hash_map_destroy(list->lookup);
		list->lookup = NULL;
	}

	free(list);
}

node_list_t* node_list_create(node_t* node) {
	node_list_t* list = (node_list_t*) malloc(sizeof(node_list_t));
	if(list == NULL) {
		return NULL;
	}
	memset(list, '\0', sizeof(node_list_t));

	// Initialize structure
	list->begin = NULL;
	list->end = NULL;
	list->count = 0;
	list->owner = node;
	list->lookup = hash_map_create(32);
	if(list->lookup == NULL) {
		free(list);
		return NULL;
	}
	return list;
}

int node_list_add(node_list_t* list, node_t* node) {
	node_t* last = NULL;
	if(list == NULL || node == NULL) {
		return -1;
	}

	if(node->key != NULL && hash_map_get(list->lookup, node->key) != NULL) {
		return -1;
	}

	last = list->end;
	node->next = NULL;
	node->prev = last;

	if(last != NULL) {
		last->next = node;
	} else {
		list->begin = node;
	}

	list->end = node;
	list->count++;

	if(node->key != NULL && hash_map_put(list->lookup, node->key, node) != 0) {
		// Rollback on failure to index
		list->count--;
		list->end = node->prev;
		if(node->prev != NULL) {
			node->prev->next = NULL;
		} else {
			list->begin = NULL;
		}
		node->next = NULL;
		node->prev = NULL;
		return -1;
	}

	return 0;
}

int node_list_remove(node_list_t* list, node_t* node) {
	if(list == NULL || node == NULL) {
		return -1;
	}

	if(node->prev != NULL) {
		node->prev->next = node->next;
	} else if(list->begin == node) {
		list->begin = node->next;
	} else {
		return -1;
	}

	if(node->next != NULL) {
		node->next->prev = node->prev;
	} else if(list->end == node) {
		list->end = node->prev;
	} else {
		return -1;
	}

	if(list->count > 0) {
		list->count--;
	}

	if(node->key != NULL) {
		hash_map_remove(list->lookup, node->key);
	}

	node->next = NULL;
	node->prev = NULL;

	return 0;
}

node_t* node_list_find(node_list_t* list, const char* key) {
	if(list == NULL || key == NULL) {
		return NULL;
	}

	return (node_t*) hash_map_get(list->lookup, key);
}

