#ifndef _IMAGELOOP_H
#define _IMAGELOOP_H

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <type_handle.h>

#define MAX_FILENAME 256
//#define MAX_IMAGEARR 100
#define TYPE_BMP 0x0
#define TYPE_PNG 0x1
#define TYPE_JPG 0x2

struct img_file {
	char f_name[MAX_FILENAME];
	int  type;
	char cache[MAX_FILENAME];
	char resolution[10];// 1234x4321
	void * pspecial;
	struct img_file *next;
	struct img_file *pre;
};

typedef struct {
	int size;
	struct img_file * head;
}FILE_LIST;

enum IMG_TYPE{
	USER_TYPE_BMP = 1,
	USER_TYPE_JPG,
	USER_TYPE_PNG
};




//function of imageloop.c
FILE_LIST * fmanage_init(const char * dir_path);
void fm_free(FILE_LIST * file_list);




#endif
