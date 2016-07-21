/* 
 * date: 2016-07-17
 * file: xiaoxing_media.c
 * xgm
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lw_oopc.h"
#include "xiaoxing_media.h"


static void init(void *t, const char *name)
{
	Media* cthis = (Media*)t;
	cthis->name = (char*)malloc(strlen(name)+1);
	strncpy(cthis->name, name, strlen(name)+1);
}
static void changeName(void *t)
{
	Media* cthis = (Media*)t;
}

static char* getName(void *t)
{
	Media* cthis = (Media*)t;
	return cthis->name;
}

static void release(void *t)
{
	Media* cthis = (Media*)t;
	free(cthis->name);
}

CTOR(Media)
	FUNCTION_SETTING(init, init)
	FUNCTION_SETTING(changeName, changeName)
	FUNCTION_SETTING(getName, getName)
	FUNCTION_SETTING(release, release)
END_CTOR

/* 功能:函数可以将一个字符串指定位置指定长度的子字符串转化为整数
 * example: video19700124105123
 * index: 从0开始
 * n: 从1开始
 */
static int atoi_from_string_locale(char *src, int index, int n)
{
	char str[n+1];

	memset(str, 0, n+1);
	strncpy(str, src + index, n);

	return atoi(str);
}





