/* 
 * date: 2016-07-17
 * file: xiaoxing_media.h
 * xgm
 */

#ifndef XIAOXING_MEDIA_H_INCLUDE_
#define XIAOXING_MEDIA_H_INCLUDE_

#include <stdio.h>
#include "lw_oopc.h"

CLASS(Media)
{
	void (*init)(void *, const char *);
	void (*changeName)(void *);
	char* (*getName)(void *);
	void (*release)(void *);
	char *name;
};

#endif
