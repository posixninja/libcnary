/*
 * list.c
 *
 *  Created on: Mar 8, 2011
 *      Author: posixninja
 */

#include <stdlib.h>

#include "list.h"

void list_init(list_t* list) {
	if(list == NULL) {
		return;
	}
	list->begin = NULL;
	list->end = NULL;
	list->count = 0;
}


void list_destroy(list_t* list) {
	object_t* current = NULL;
	object_t* next = NULL;
	if(list == NULL) {
		return;
	}

	current = (object_t*) list->begin;
	while(current != NULL) {
		next = current->next;
		current->next = NULL;
		current->prev = NULL;
		current = next;
	}

	free(list);
}

int list_add(list_t* list, object_t* object) {
	object_t* tail = NULL;
	if(list == NULL || object == NULL) {
		return -1;
	}

	object->next = NULL;
	tail = (object_t*) list->end;
	object->prev = tail;

	if(tail != NULL) {
		tail->next = object;
	} else {
		list->begin = object;
	}

	list->end = object;
	list->count++;
	return 0;
}

int list_remove(list_t* list, object_t* object) {
	if(list == NULL || object == NULL) {
		return -1;
	}

	if(object->prev != NULL) {
		object->prev->next = object->next;
	} else if(list->begin == object) {
		list->begin = object->next;
	} else {
		return -1;
	}

	if(object->next != NULL) {
		object->next->prev = object->prev;
	} else if(list->end == object) {
		list->end = object->prev;
	} else {
		return -1;
	}

	object->next = NULL;
	object->prev = NULL;
	if(list->count > 0) {
		list->count--;
	}
	return 0;
}
