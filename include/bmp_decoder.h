#ifndef _BMP_DECODER_H
#define _BMP_DECODER_H

#include <stdio.h>
#include <stdlib.h>
#include <type_handle.h>


#define NULL ((void *)0)

unsigned int * core_handle_bmp(const struct bmp_handler * data);


#endif
