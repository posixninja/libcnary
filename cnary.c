/*
 * cnary.c
 *
 *  Created on: Mar 9, 2011
 *      Author: posixninja
 */

#include <stdio.h>

#include "node.h"

int main(int argc, char* argv[]) {
	node_t* found = NULL;

	puts("Creating root node");
	node_t* root = node_create(NULL, NULL);

	puts("Creating child 1 node");
	node_t* one = node_create(root, NULL);
	puts("Creating child 2 node");
	node_t* two = node_create(root, NULL);

	puts("Creating child 3 node");
	node_t* three = node_create(one, NULL);

	puts("Debugging root node");
	node_debug(root);

	puts("Creating keyed child nodes");
	node_t* alpha = node_create(root, "alpha");
	node_t* beta  = node_create(root, "beta");
	node_t* gamma = node_create(alpha, "gamma");

	puts("Looking up child nodes by key");
	found = node_lookup(root, "alpha");
	printf("lookup(root, \"alpha\") -> %s\n", found == alpha ? "OK" : "FAIL");

	found = node_lookup(root, "beta");
	printf("lookup(root, \"beta\")  -> %s\n", found == beta ? "OK" : "FAIL");

	found = node_lookup(alpha, "gamma");
	printf("lookup(alpha, \"gamma\") -> %s\n", found == gamma ? "OK" : "FAIL");

	found = node_lookup(root, "missing");
	printf("lookup(root, \"missing\") -> %s\n", found == NULL ? "OK (NULL)" : "FAIL");

	puts("Destroying root node");
	node_destroy(root);
	return 0;
}
