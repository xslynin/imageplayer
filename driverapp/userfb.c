#include <userfb.h>


//test for fb use
#if 0 
//actually, the smem len include many other sfr 
void background_set(unsigned int width, unsigned int heigh, unsigned int color_argb){
	unsigned int i, j;
	for(i = 0; i < heigh ;i++){
		for(j = 0; j < width; j++){
			*(pbuf + i * WIDTH + j) = color_argb;
		}
	}
}
#endif


// this function is based fb image show for a general use
// so, this function will lead to another core api for image handle
// such as, to a function called image_resolve() and this function will read the binary information head
// then, it distribute the image resolve assignment to special api like jpegdecode or bmpdecode so on
//
// 2.24  after bmp decoder's finish, the frame of my work is clear
// image_set only need to be called with the bitmap to display
// the key is to fill pbuf
void image_set(FB_HANDLER * fb, const unsigned int * bitmap){
	//however, now I only want to display one image
	//now, let's consider a array with image infomation	
	unsigned int * copy_pbuf = fb->pbuf;//this pointer used for move from fb's start to end for image display
	int i;

#if 1
	for(i = 0; i < IMAGES_HEIGH * IMAGES_WIDTH; i++){
			//*copy_pbuf++ = images[i];
			*copy_pbuf++ = bitmap[i];
//			printf("0x%x ", bitmap[i]);
	}	
#endif
	
	//fill the double buffer
	//...
	
}

void image_set2(FB_HANDLER * fb, unsigned char ** bitmap){
	//however, now I only want to display one image
	//now, let's consider a array with image infomation	
	unsigned int * copy_pbuf = fb->pbuf;//this pointer used for move from fb's start to end for image display
	int i,j;

	for(i = 0; i < IMAGES_HEIGH; i++){
		for(j = 0; j < IMAGES_WIDTH;j++){
			*copy_pbuf++ = (bitmap[i][j*3 + 1] << 16) |(bitmap[i][j*3 + 1] << 8) |(bitmap[i][j*3 + 1]);
		}		
	}	
	//fill the double buffer
	//...
	
}

//pointer return with two charcter
//first, could the transfer of pointer consume less ram
//second, could handle the situation of error,    that is, i can return NULL as a kind of pointer
//err handle leave for main 
FB_HANDLER * fb_init(int fd){
	int ret;
	//struct fb_user_info fb_info_ret;
	FB_HANDLER * fb = (FB_HANDLER *) malloc(sizeof(FB_HANDLER));
	if(!fb){
//		perror("Alloc framebuffer handler fail!");
		return NULL;
	}
	fb->io_entry = fd;
	fb->flag  = 0;
	fb->state = 0;

#if 0
	fb->fd = open(FBDEVICE, O_RDWR);
	if(fb->fd < 0){
		perror("open fb device");
		free(fb);
		return NULL;
	}
#endif

	//operate the fb
	ret = ioctl(fb->io_entry, FBIOGET_FSCREENINFO, &fb->fix_info);
	if(ret < 0){
		perror("ioctl");
		return NULL;
	}
	
	ret = ioctl(fb->io_entry, FBIOGET_VSCREENINFO, &fb->v_info);
	if(ret < 0){
		perror("ioctl");
		return NULL;
	}
//debug infomation
#if 0	
	printf("the start of memaddr is : 0x%lx, the length is :%d\n", fix_info.smem_start, fix_info.smem_len);
	printf("\n %u \n %u \n  %u \n %u \n", v_info.xres_virtual, v_info.yres_virtual, v_info.xoffset, v_info.yoffset);
#endif

	fb->len = fb->v_info.xres_virtual * fb->v_info.yres_virtual * fb->v_info.bits_per_pixel / 8 ;
	fb->pbuf = mmap(NULL, fb->len, PROT_READ | PROT_WRITE, MAP_SHARED, fb->io_entry, 0);
	if(!fb->pbuf){
		perror("mmap");
		return NULL;
	}

	return fb;
}

//change the double buffer
//when the entire fb handler is designed, consider the flag and the state elements
//the operation for logical bussiness could be seperated from the I/O operation
int fb_doublebuffer_switch(FB_HANDLER * fb){
	int ret;
	fb->v_info.yoffset = fb->state? 0:600;
	//fb->state = 0 for offset 0, then call this function to change  the offset for 600
	
	ret = ioctl(fb->io_entry, FBIOPUT_VSCREENINFO, &fb->v_info);
	if(ret < 0){
		perror("SWITCH ERROR");
		return -1;
	}
	//flip when success
	fb->state = fb->state ^ 0x1;

	return 0;
}

void fb_free(FB_HANDLER * fb){
	munmap((void *)fb->pbuf, fb->len);
	close(fb->io_entry);
	free(fb);
}
