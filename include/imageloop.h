#ifndef _IMAGELOOP_H
#define _IMAGELOOP_H

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>

#define MAX_FILENAME 256
//#define MAX_IMAGEARR 100
#define TYPE_BMP 0x0
#define TYPE_PNG 0x1
#define TYPE_JPG 0x2

struct img_file {
	char f_name[MAX_FILENAME];
	char type;
	char cache[MAX_FILENAME];
	char resolution[10];// 1234x4321
	struct img_file *next;
	struct img_file *pre;
};

struct img_file * fmanage_init(const char * dir_path);
#endif
