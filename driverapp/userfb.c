#include <userfb.h>


//test for fb use
#if 1 
//actually, the smem len include many other sfr 
void background_set(unsigned int width, unsigned int heigh, unsigned int color_argb, FB_HANDLER * fb){
	unsigned int i, j;
	unsigned int * copy_buf = fb->pbuf;
	for(i = 0; i < heigh ;i++){
		for(j = 0; j < width; j++){
			*(copy_buf + i * IMAGES_WIDTH + j) = color_argb;
		}
	}
}
#endif


// 
void image_set(FB_HANDLER * fb, struct img_file * obj){
	unsigned int * copy_pbuf = fb->pbuf;//this pointer used for move
	int i;

	for(i = 0; i < IMAGES_HEIGH * IMAGES_WIDTH; i++){
		*copy_pbuf++ = ((unsigned int *)obj->pspecial)[i];
	}	

	for(i = 0; i < fb->res; i++){
		if(i < fb->res/2)
			*copy_pbuf++ = ((unsigned int *)obj->pre->pspecial)[i];
		else
			*copy_pbuf++ = ((unsigned int *)obj->next->pspecial)[i];
	}

}

void image_before_show(FB_HANDLER * fb, struct img_file * obj){
	int i, j;
	unsigned int *copy_buf, *cp;
	if(fb->state & 0x1){
		//fb->state = 0b.....1 ^ 0x1 = 1 
		//so current offset is 600, set the buf to 0
		copy_buf = fb->pbuf;
	}else
		copy_buf = fb->pbuf + fb->res;//res is size of a img 1024 multi 600

	if(fb->state & 0x2){
		//fill the top half
		cp = copy_buf;
		j = 0;
	}else{
		//fb->state = 0b...0.
		//load the pre img, fill the bottom half
		cp = copy_buf + fb->res/2;
		j = fb->res/2;
	}

	for(i = 0; i < fb->res/2; i++){
		*cp++ = ((unsigned int *)obj->pspecial)[i + j];
	}	
}

void image_preload(FB_HANDLER * fb, struct img_file * obj){
	int i, j;
	unsigned int * copy_buf;
	unsigned int * target;
	//clear our mind
	//the image_set2 should support: 
	//	fill the buffer that don't display yet;

	if(fb->state & 0x1)
		copy_buf = fb->pbuf;
	else
		copy_buf = fb->pbuf + fb->res;
	
//	copy_pbuf += fb->v_info.xres_virtual * fb->v_info.yres_virtual / 2;
	if(fb->state & 0x2){
		//next image was chosen
		//once click to next, so in preload, we only load the next's next img to the bottom half
		copy_buf += fb->res/2;
		target = (unsigned int *)obj->next->pspecial;
		j = fb->res/2;
	}else{
		//pre image chose			attention!!! preload happened before next change and continue from recent change
		//once click to pre, we only load the pre's pre to the top half
		target = (unsigned int *)obj->pre->pspecial;
		j = 0;
	}

	for(i = 0; i < fb->res/2; i++){
		*copy_buf++ = target[i + j];
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
	fb->res = fb->v_info.xres_virtual * fb->v_info.yres_virtual / 2;
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
int fb_switch(FB_HANDLER * fb){
	int ret;
	//state [0] = 0   ----> current 0 need to change to 600
	//            1         current 600 need to change to 0
	fb->v_info.yoffset = (fb->state & 0x1)? 0:600;
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
