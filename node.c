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

static char* node_strdup(const char* source) {
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

void node_destroy(node_t* node) {
	node_t* child = NULL;
	node_t* next = NULL;

	if(node == NULL) {
		return;
	}

	if(node->children != NULL) {
		child = node->children->begin;
		while(child != NULL) {
			next = child->next;
			node_destroy(child);
			child = next;
		}
	}

	if(node->parent != NULL) {
		node_detach(node->parent, node);
	}

	if(node->children != NULL) {
		node_list_destroy(node->children);
		node->children = NULL;
	}

	if(node->key != NULL) {
		free(node->key);
		node->key = NULL;
	}

	free(node);
}

node_t* node_create(node_t* parent) {
	return node_create_with_key(parent, NULL);
}

node_t* node_create_with_key(node_t* parent, const char* key) {
	int error = 0;
	node_t* node = (node_t*) malloc(sizeof(node_t));
	if(node == NULL) {
		return NULL;
	}
	memset(node, '\0', sizeof(node_t));

	node->depth = 0;
	node->next = NULL;
	node->prev = NULL;
	node->isLeaf = TRUE;
	node->isRoot = TRUE;
	node->parent = NULL;
	node->count = 0;
	node->attach = node_attach;
	node->detach = node_detach;

	if(key != NULL) {
		node->key = node_strdup(key);
		if(node->key == NULL) {
			free(node);
			return NULL;
		}
	}

	node->children = node_list_create(node);
	if(node->children == NULL) {
		free(node->key);
		free(node);
		return NULL;
	}

	// Pass NULL to create a root node
	if(parent != NULL) {
		// This is a child node so attach it to it's parent
		error = node_attach(parent, node);
		if(error < 0) {
			// Unable to attach nodes
			printf("ERROR: %d \"Unable to attach nodes\"\n", error);
			node_list_destroy(node->children);
			free(node->key);
			free(node);
			return NULL;
		}
	}

	return node;
}

int node_attach(node_t* parent, node_t* child) {
	int status = 0;

	if(parent == NULL || child == NULL) {
		return -1;
	}

	if(child->parent != NULL && child->parent != parent) {
		node_detach(child->parent, child);
	}

	child->isRoot = FALSE;
	child->parent = parent;
	child->depth = parent->depth + 1;
	child->isLeaf = (child->children == NULL || child->children->count == 0);

	status = node_list_add(parent->children, child);
	if(status != 0) {
		child->parent = NULL;
		child->depth = 0;
		child->isRoot = TRUE;
		return status;
	}

	parent->isLeaf = FALSE;
	parent->count = parent->children->count;
	return status;
}

int node_detach(node_t* parent, node_t* child) {
	int status = 0;

	if(parent == NULL || child == NULL || child->parent != parent) {
		return -1;
	}

	status = node_list_remove(parent->children, child);
	if(status != 0) {
		return status;
	}

	child->parent = NULL;
	child->depth = 0;
	child->isRoot = TRUE;

	if(child->children == NULL || child->children->count == 0) {
		child->isLeaf = TRUE;
	}

	if(parent->children->count == 0) {
		parent->isLeaf = TRUE;
	}
	parent->count = parent->children->count;
	return 0;
}

node_t* node_find_child(node_t* parent, const char* key) {
	if(parent == NULL || key == NULL || parent->children == NULL) {
		return NULL;
	}
	return node_list_find(parent->children, key);
}

void node_debug(node_t* node) {
	int i = 0;
	node_t* current = NULL;
	node_iterator_t* iter = NULL;

	if(node == NULL) {
		return;
	}

	for(i = 0; i < (int) node->depth; i++) {
		printf("\t");
	}

	if(node->isRoot) {
		printf("ROOT");
	} else if(node->isLeaf) {
		printf("LEAF");
	} else {
		printf("NODE");
	}

	if(node->key != NULL) {
		printf(" [%s]", node->key);
	}
	printf("\n");

	if(node->children == NULL || node->children->count == 0) {
		return;
	}

	iter = node_iterator_create(node->children);
	if(iter == NULL) {
		return;
	}

	for(current = iter->begin; current != NULL; current = iter->next(iter)) {
		node_debug(current);
	}

	node_iterator_destroy(iter);
}
