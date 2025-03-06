#ifndef _USERFB_H
#define _USERFB_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <stdlib.h>
#include <bmp_decoder.h>


#define FBDEVICE			"/dev/fb0"
#define IMAGES_WIDTH 		1024 
#define IMAGES_HEIGH 		600


typedef struct fb_handler{
	int io_entry;
	unsigned int * pbuf;
	unsigned long len;
	struct fb_fix_screeninfo fix_info;
	struct fb_var_screeninfo v_info;
	int flag;			//no use for now
	char state;
}FB_HANDLER;



//function in userfb.c
//void background_set(unsigned int width, unsigned int heigh, unsigned int color_argb);
void image_set(FB_HANDLER * fb, const unsigned int * file);
void image_set2(FB_HANDLER * fb, unsigned char ** bitmap);
FB_HANDLER * fb_init(int fd);
int fb_doublebuffer_switch(FB_HANDLER * fb);
void fb_free(FB_HANDLER * fb);

#endif
