/*	cx16-vec.h	*/
#ifndef CX16_VEC_H_INCLUDE
#define CX16_VEC_H_INCLUDE

#include <stdio.h>
#include "lw_oopc.h"

CLASS(Vector)
{
	void **pv;
	int index;
	void (*init)(void*, int);
	void (*Add)(void *, void *);
	void (*Remove)(void *);
	int (*GetSize)(void*);
	void* (*Get)(void*, int);
	void (*release)(void *);
};

#endif
