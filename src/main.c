/* 
 * date: 2016-07-17 晚
 * file: xiaoxing.h
 * xgm
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cx16-vec.h"
#include "xiaoxing.h"

/* test */
#include <dirent.h>

static int filter(const struct dirent *dirent);

/* HN1A005G1C04495/1970/01/19700312/video19700312104522.mp4 */
/* 文件名长度，不包括.后面的字符 */
#define NAME_LEN_PREFIX 20
/* 后缀 */
#define MSC_SUFFIX	"msc"

#define XIAOXING_CAMERA_DIR		"/home/gm/xiaomi/file"
#define MODIFYED_CAMERA_DIR		"/home/gm/xiaomi/dest"
#define CURRENT_TEST_DIR		"/home/gm/xiaomi/HN1A005G1C04495/1970"

Xiao* xiao; 

int main()
{
	xiao = (Xiao*)Xiao_new();
	xiao->init(xiao);

	struct dirent **namelist;
	int n;

	n = scandir(CURRENT_TEST_DIR, &namelist, filter, alphasort);
	if (n < 0)
		perror("scandir");
		/*
	else {
		while (n--) {
			Media* me = (Media*)Media_new();		
			me->init(me, namelist[n]->d_name);
			pv->Add(pv, me);
			
			free(namelist[n]);
		}
		free(namelist);
	}

	int i;
	Media* me2 = (Media*)Media_new();
	printf("%d\n", pv->GetSize(pv));
	for(i = 0; i < pv->GetSize(pv); i++) {
		me2 = pv->Get(pv, i);
		me2->changeName(me2);
		printf("file: %s\n", me2->name);
	}
	*/

	return 0;
}

static int filter(const struct dirent *dirent)
{
	static char buf[255] = {0};


	/* . and .. 文件夹排除*/
	if(dirent->d_name[0] == '.')
		return  0;

	/* 过滤msc文件 */
	if(strncmp(dirent->d_name + NAME_LEN_PREFIX, "msc", 3) == 0) {
		if(strncmp(buf, dirent->d_name, NAME_LEN_PREFIX) == 0) {
			xiao->removeFile(xiao);
			goto out;
		} else 
			goto out;
	} else {
		if(strncmp(buf, dirent->d_name, NAME_LEN_PREFIX) == 0) 
			goto out;
		 else {
			printf("Will add  %s\n", dirent->d_name);
			xiao->addFile(xiao, dirent->d_name);
			goto out;
		}
	}
	return 1;

out:
	strncpy(buf, dirent->d_name, strlen(dirent->d_name));
	return 0;
}
