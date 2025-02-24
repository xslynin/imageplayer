#ifndef _TYPE_HANDLE_H
#define _TYPE_HANDLE_H

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

struct bmp_handler {
	unsigned int filesize;
    unsigned int dataoffset;
	unsigned int image_width;
	unsigned int image_heigh;
	unsigned int imagesize;
	unsigned char * image_data;
};


#include <bmp_decoder.h>
#include <userfb.h>


int do_type_handle(const char * file_path);

#endif
