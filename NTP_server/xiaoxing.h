/* 
 * date: 2016-07-17 晚
 * file: xiaoxing.h
 * xgm
 */


#ifndef XIAOXING_H_INCLUDE_
#define XIAOXING_H_INCLUDE_

#include <stdio.h>
#include "lw_oopc.h"
#include "cx16-vec.h"
#include "xiaoxing_media.h"


INTERFACE(CAMERA)
{
	void (*init)(void *);
	void (*changeAllFileName)(void *);
	void (*addFile)(void *, const char *);
	void (*removeFile)(void *);
	int  (*getFileCount)(void *);
	void (*print_xiao)(void *);
	void (*release)(void *);
};


#endif
