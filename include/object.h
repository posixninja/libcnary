/*
 * object.h
 *
 *  Created on: Mar 8, 2011
 *      Author: posixninja
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct object_t {
	struct object_t* next;
	struct object_t* prev;
	void* value;
	unsigned int type;
	unsigned int size;
} object_t;

#endif /* OBJECT_H_ */
