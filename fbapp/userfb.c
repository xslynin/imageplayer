#include <userfb.h>

#include <test.h> // test for display a image by the way of array bitmap

struct fb_fix_screeninfo fix_info;
struct fb_var_screeninfo v_info;
unsigned int * pbuf;	//locate certain pixel
unsigned long len;

//actually, the smem len include many other sfr 
void background_set(unsigned int width, unsigned int heigh, unsigned int color_argb){
	unsigned int i, j;
	for(i = 0; i < heigh ;i++){
		for(j = 0; j < width; j++){
			*(pbuf + i * WIDTH + j) = color_argb;
		}
	}
}

// this function is based fb image show for a general use
// so, this function will lead to another core api for image handle
// such as, to a function called image_resolve() and this function will read the binary information head
// then, it distribute the image resolve assignment to special api like jpegdecode or bmpdecode so on
//
// 2.24  after bmp decoder's finish, the frame of my work is clear
// image_set only need to be called with the bitmap to display
// the key is to fill pbuf
void image_set(const unsigned int * bitmap){
	//however, now I only want to display one image
	//now, let's consider a array with image infomation	
	unsigned int * copy_pbuf = pbuf;//this pointer used for move from fb's start to end for image display
	int i;

#if 0 
	unsigned char * ptr = NULL;
	fd_img = open(file, O_RDONLY);
	ptr =(unsigned char *) malloc(1024 * 1024 * 2);
	printf("%p \n", ptr);
	ret = read(fd_img, ptr, 1024 * 1024 * 2);
	printf("---------- %d -------------\n", ret);
	//maybe this module should belong to file manage module
	bitmap = core_handle_bmp(ptr);
#endif

	//printf("TEST");
#if 1
	for(i = 0; i < IMAGES_HEIGHT * IMAGES_WIDTH; i++){
			//*copy_pbuf++ = images[i];
			*copy_pbuf++ = bitmap[i];
	}	
#endif

}

//pointer return with two charcter
//first, could the transfer of pointer consume less ram
//second, could handle the situation of error,    that is, i can return NULL as a kind of pointer
int fb_open(){
	int fd, ret;
	//struct fb_user_info fb_info_ret;

	fd = open(FBDEVICE, O_RDWR);
	if(fd < 0){
		perror("open fb device");
		goto end;
	}
	printf("open done!\n");

	//operate the fb
	ret = ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
	if(ret < 0){
		perror("ioctl");
		close(fd);
		return -1;
	}
	
	ret = ioctl(fd, FBIOGET_VSCREENINFO, &v_info);
	if(ret < 0){
		perror("ioctl");
		close(fd);
		return -1;
	}



	printf("the start of memaddr is : 0x%lx, the length is :%d\n", fix_info.smem_start, fix_info.smem_len);
	printf("\n %u \n %u \n  %u \n %u \n", v_info.xres_virtual, v_info.yres_virtual, v_info.xoffset, v_info.yoffset);

//test for double buffer
#if 0	
//test how to make use of double buffer mode
	//v_info.xoffset = 400;
	v_info.yoffset = 300;
	ret = ioctl(fd, FBIOPUT_VSCREENINFO, &v_info);
	if(ret < 0){
		perror("set offset error");
		close(fd);
		return -1;
	}

#endif
	len = v_info.xres_virtual * v_info.yres_virtual * v_info.bits_per_pixel / 8 ;
	pbuf = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(pbuf == NULL){
		perror("mmap");
		close(fd);	
		return -1;
	}
//	printf("pbuf = %p\n", pbuf);
/*
mmap_end:
	munmap((void *)pbuf, fix_info.smem_len);
*/
end:
	return fd;
}

void fb_close(int fd){
	munmap((void *)pbuf, len);
	close(fd);
}
