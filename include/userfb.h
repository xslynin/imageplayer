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
#include <img_manage.h>

#define FBDEVICE			"/dev/fb0"
#define IMAGES_WIDTH 		1024 
#define IMAGES_HEIGH 		600


typedef struct fb_handler{
	int io_entry;
	unsigned int * pbuf;
	unsigned long len;
	unsigned int res;
	struct fb_fix_screeninfo fix_info;
	struct fb_var_screeninfo v_info;
	int flag;			//no use for now
	char state;//state [0] map the 0 and 600 offset
			   //state [1] map the top half and bottom half  img fill
}FB_HANDLER;



//function in userfb.c
void background_set(unsigned int width, unsigned int heigh, unsigned int color_argb, FB_HANDLER * fb);
void image_set(FB_HANDLER * fb, struct img_file * obj);
void image_before_show(FB_HANDLER * fb, struct img_file * obj);
void image_preload(FB_HANDLER * fb, struct img_file * obj);
FB_HANDLER * fb_init(int fd);
int fb_switch(FB_HANDLER * fb);
void fb_free(FB_HANDLER * fb);

#endif
