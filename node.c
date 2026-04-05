/*
 * node.c
 *
 *  Created on: Mar 7, 2011
 *      Author: posixninja
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "node_list.h"
#include "node_iterator.h"

void node_destroy(node_t* node) {
	if(node) {
		if(node->key) {
			free(node->key);
		}
		free(node);
	}
}

node_t* node_create(node_t* parent, const char* key) {
	int error = 0;

	node_t* node = (node_t*) malloc(sizeof(node_t));
	if(node == NULL) {
		return NULL;
	}
	memset(node, '\0', sizeof(node_t));

	node->depth = 0;
	node->next = NULL;
	node->prev = NULL;
	node->key = NULL;
	node->isLeaf = TRUE;
	node->isRoot = TRUE;
	node->parent = NULL;
	node->children = node_list_create(node);

	if(key != NULL) {
		node->key = strdup(key);
		if(node->key == NULL) {
			node_destroy(node);
			return NULL;
		}
	}

	// Pass NULL to create a root node
	if(parent != NULL) {
		// This is a child node so attach it to it's parent
		error = node_attach(parent, node);
		if(error < 0) {
			// Unable to attach nodes
			printf("ERROR: %d \"Unable to attach nodes\"\n", error);
			node_destroy(node);
			return NULL;
		}
	}

	return node;
}

int node_attach(node_t* parent, node_t* child) {
	child->isLeaf = TRUE;
	child->isRoot = FALSE;
	child->parent = parent;
	child->depth = parent->depth + 1;
	if(parent->isLeaf == TRUE) {
		parent->isLeaf = FALSE;
	}
	return node_list_add(parent->children, child);
}

int node_detach(node_t* parent, node_t* child) {
	return 0;
}

node_t* node_lookup(node_t* parent, const char* key) {
	if(parent == NULL || key == NULL) {
		return NULL;
	}
	return node_list_lookup(parent->children, key);
}

void node_debug(node_t* node) {
	int i = 0;
	node_t* current = NULL;
	node_iterator_t* iter = NULL;
	for(i = 0; i < node->depth; i++) {
		printf("\t");
	}
	if(node->isRoot) {
		printf("ROOT\n");
	}

	if(node->isLeaf && !node->isRoot) {
		printf("LEAF\n");

	} else {
		if(!node->isRoot) {
			printf("NODE\n");
		}
		iter = node_iterator_create(node->children);
		for(current = iter->begin; current != NULL; current = iter->next(iter)) {
			node_debug(current);
		}
	}

}
