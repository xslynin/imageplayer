#include <stdio.h>
#include <thread_pool.h>
#include <type_handle.h>
#include <userfb.h>

int main(){
	int fb_ret;
	
	if(thread_pool_init() ){
		printf("init thread pool wrong!:( \n");
		while(1);
	}	
	
	fb_ret = fb_open();
	if(fb_ret < 0){
		perror("open fb device");
		return -1;	
	}
	
	
	
	do_type_handle("./image/test1024x600.jpg");
//	munmap((void *)fb_ret.pbuf,fb_ret.fix_info.xres_virtual * fb_ret.fix_info.yres_virtual * fb_ret.fix_info.bits_per_pixel / 8);
	fb_close(fb_ret);
	thread_pool_destroy();
	return 0;
}
