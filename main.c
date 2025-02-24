#include <stdio.h>
#include <type_handle.h>
#include <userfb.h>

int main(){
	int fb_ret;
	
	printf("-----------------------the begin of the project\n");
	fb_ret = fb_open();
	if(fb_ret < 0){
		perror("open fb device");
		return -1;	
	}
	//background_set(WIDTH, HEIGH, 0xffff0000);
	do_type_handle("./image/test.bmp");
//	munmap((void *)fb_ret.pbuf,fb_ret.fix_info.xres_virtual * fb_ret.fix_info.yres_virtual * fb_ret.fix_info.bits_per_pixel / 8);
	fb_close(fb_ret);
	
	return 0;
}
