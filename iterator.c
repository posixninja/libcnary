/*
 * iterator.c
 *
 *  Created on: Mar 8, 2011
 *      Author: posixninja
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "object.h"
#include "iterator.h"

void iterator_destroy(iterator_t* iterator) {
	if(iterator) {
		free(iterator);
	}
}

iterator_t* iterator_create(list_t* list) {
	iterator_t* iterator = (iterator_t*) malloc(sizeof(iterator_t));
	if(iterator == NULL) {
		return NULL;
	}
	memset(iterator, '\0', sizeof(iterator_t));

	iterator->next = iterator_next;
	iterator->bind = iterator_bind;

	if(list != NULL) {
		// Create and bind to list
		iterator->bind(iterator, list);
	} else {
		// Empty Iterator
	}

	return iterator;
}

object_t* iterator_next(iterator_t* iterator) {
	object_t* next = NULL;

	if(iterator == NULL || iterator->value == NULL) {
		return NULL;
	}

	next = iterator->value->next;
	iterator->value = next;
	if(next != NULL) {
		iterator->position++;
	}
	return next;
}

int iterator_bind(iterator_t* iterator, list_t* list) {
	if(iterator == NULL) {
		return -1;
	}

	iterator->position = 0;
	iterator->count = 0;
	iterator->begin = NULL;
	iterator->end = NULL;
	iterator->value = NULL;
	iterator->list = list;

	if(list == NULL) {
		return -1;
	}

	iterator->count = list->count;
	iterator->begin = (object_t*) list->begin;
	iterator->end = (object_t*) list->end;
	iterator->value = iterator->begin;
	return 0;
}
