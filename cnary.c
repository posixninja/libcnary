/*
 * cnary.c
 *
 *  Created on: Mar 9, 2011
 *      Author: posixninja
 */

#include <stdio.h>

#include "node.h"

int main(int argc, char* argv[]) {
	(void) argc;
	(void) argv;
	puts("Creating root node");
	node_t* root = node_create_with_key(NULL, "root");

	puts("Creating child 1 node");
	node_t* one = node_create_with_key(root, "child-1");
	puts("Creating child 2 node");
	node_create_with_key(root, "child-2");

	puts("Creating child 3 node");
	node_create_with_key(one, "grandchild-3");

	puts("Looking up child-2 from root");
	node_t* found = node_find_child(root, "child-2");
	if(found != NULL) {
		printf("Found node \"%s\" at depth %u\n", found->key, found->depth);
	}

	puts("Debugging root node");
	node_debug(root);

	puts("Destroying root node");
	node_destroy(root);
	return 0;
}
