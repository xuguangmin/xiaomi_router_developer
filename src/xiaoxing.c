/* 
 * date: 2016-07-17 晚
 * file: xiaoxing.c
 * xgm
 */

#include <stdio.h>
#include "xiaoxing.h"
#include "xiaoxing_media.h"
#include "cx16-vec.h"

/* 向量最大长度 */
#define MEDIA_LIST_MAX	(100)

void init(void *t)
{
	Xiao* cthis = (Xiao*)t;
	Vector* pv = cthis->medialist;
	pv = (Vector*)Vector_new();
	pv->init(pv, MEDIA_LIST_MAX);
	cthis->medialist = pv;
}

static void changeAllFileName(void *t)
{
	;
}

static void addFile(void *t, const char *obj)
{
	Xiao* cthis = (Xiao*)t;
	Vector* list = (Vector*)cthis->medialist;
	Media* media = (Media*)Media_new();
	media->init(media, obj);
	list->Add(list, media);
}

static void removeFile(void *t)
{
	Xiao* cthis = (Xiao*)t;
	Vector* pv = (Vector*)cthis->medialist;
	Media* media = (Media*)pv->Get(pv, pv->GetSize(pv)-1);
	media->release(media);
	pv->Remove(pv);
}

static int  getFileCount(void *t)
{
	Xiao* cthis = (Xiao*)t;
	Vector* pv = (Vector*)cthis->medialist;
	return pv->GetSize(pv);
}

static void release(void *t)
{
	Xiao* cthis = (Xiao*)t;
	Vector* pv = (Vector*)cthis->medialist;
	pv->release(pv);
}

static void print_xiao(void *t)
{
	int i;
	Media* media;
	Xiao* cthis = (Xiao*)t;
	Vector* pv = (Vector*)cthis->medialist;

	for(i = 0; i < pv->GetSize(pv); i++) {
		media = (Media*)pv->Get(pv, i);
		printf("%s\n", media->getName(media));
	}
}

CTOR(Xiao)
	FUNCTION_SETTING(init, init)
	FUNCTION_SETTING(changeAllFileName, changeAllFileName)
	FUNCTION_SETTING(addFile, addFile)
	FUNCTION_SETTING(removeFile, removeFile)
	FUNCTION_SETTING(getFileCount, getFileCount)
	FUNCTION_SETTING(release, release)
	FUNCTION_SETTING(print_xiao, print_xiao)
END_CTOR

