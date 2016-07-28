/*	cx16-vec.c	*/
#include "cx16-vec.h"

static void initialize(void* t, int n)
{
	Vector* cthis = (Vector*)t;
	cthis->index = 0;
	cthis->pv = (void**)malloc(n*sizeof(void*));
}

static void Add(void* t, void* obj)
{
	Vector* cthis = (Vector*)t ;
	cthis->pv[cthis->index]= obj;
	cthis->index++;
}

static void Remove(void* t)
{
	Vector* cthis = (Vector*)t;
	cthis->index--;
	cthis->pv[cthis->index]= NULL;
}

static int GetSize(void* t)
{
	Vector* cthis = (Vector*) t;
	return cthis->index;
}

static void* Get(void* t, int k)
{
	Vector* cthis = (Vector*) t;
	return cthis->pv[k];
}

static void release(void *t)
{
	Vector* cthis = (Vector*) t;
	free(cthis->pv);
}

CTOR(Vector)
	FUNCTION_SETTING(init, initialize)
	FUNCTION_SETTING(Add, Add)
	FUNCTION_SETTING(Remove, Remove)
	FUNCTION_SETTING(GetSize, GetSize)
	FUNCTION_SETTING(Get, Get)
	FUNCTION_SETTING(release, release)
END_CTOR
