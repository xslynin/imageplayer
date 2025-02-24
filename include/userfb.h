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


#define FBDEVICE	"/dev/fb0"
#define WIDTH 		1024 
#define HEIGH 		600


#if 0
struct fb_user_info {
	int fd;
	unsigned int * pbuf;
};
#endif
//function in userfb.c
void background_set(unsigned int width, unsigned int heigh, unsigned int color_argb);
void image_set(const unsigned int * file);
int fb_open();
void fb_close(int fd);


#endif
