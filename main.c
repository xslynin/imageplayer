#include <stdio.h>
#include <thread_pool.h>
#include <type_handle.h>
#include <userfb.h>
#include <debug.h>
#include <userts.h>
#include <imageloop.h>
//let's do a complete conclusion
//first, we initate a thread pool for image decompress assignment and till now we don't actually ensure the need of process, but at lease we realize some threads
//second, fb should open, return the related resource to control
//third, run the image file manager module, do:
//1, recurse the whole "image/" dictionary and verify the image file to linklist
//2, decompress all the file and save cache(after this step, we could directly use cache to display )
//3, display the "first image", should be bmp and before all the image difference
//fourth, touchscreen should be open, a single progress or thread to handle the event of click on screen to change image display
//and more important, do the double buffer logic to faster the image display
//
//

int main(){
	int fb_ret, fd_ts, ret;
	struct img_file * img_file;
//	struct input_event ev;

	//separate the thread pool to child progress

	if(thread_pool_init() ){
		printf("init thread pool wrong!:( \n");
		return -1;
		while(1);
	}	
	
	//fb and touch should be parent progress
	fb_ret = fb_open();
	if(fb_ret < 0){
		perror("open fb device");
		return -1;	
	}
#if 0
	if(ts_init()){
		printf("initate the touchscreen wrong!:( \n");
		return -1;
	}
#endif

	fd_ts = open("/dev/input/event1", O_RDONLY);
	if(fd_ts < 0){
		perror("open ts device");
		return 1;
	}
	//initation
	img_file = fmanage_init("./image");

	while(1){
//		display current image linklist points to
		
		do_type_handle(img_file->f_name);
		LOG(DEBUG, "%s", img_file->f_name);
#if 0
		//		fresh the double buffer for two direction
		listening the touch screen for img change
		if	pre   ... load the remaining half data //there consider one more buffer in ram, that is, keep a img size's ram to save the half data don't be load 
						call fb api to change the windows for display
							
		elif next ... load the remaining half data

		elif close ... break;
#endif
		ret = ts_routine(fd_ts);
		if(ret == TS_LEFT){
			LOG(DEBUG, "left");
			img_file = img_file->pre;
		}else if(ret == TS_RIGHT){
			LOG(DEBUG, "right");
			img_file = img_file->next;
		}else if(ret == TS_CLOSE){
			LOG(DEBUG, "CLOSE");
			break;
		}

	}
	//free all the resource 
	//parent progress infinite the loop to handle the touch event
#if 0
	fd_ts = open("/dev/input/event1", O_RDONLY);
	if(fd_ts < 0){
		perror("open ts device");
		return 1;
	}
	while(1){
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd_ts, &ev, sizeof(struct input_event));
		LOG(DEBUG, "type %hd code %hd value:%d", ev.type, ev.code, ev.value);
	}
//	close(fd_ts);
#endif
//	do_type_handle("./image/Cheetahs.jpg");
//	munmap((void *)fb_ret.pbuf,fb_ret.fix_info.xres_virtual * fb_ret.fix_info.yres_virtual * fb_ret.fix_info.bits_per_pixel / 8);
	close(fd_ts);	
	
	fb_close(fb_ret);
	thread_pool_destroy();
	return 0;
}
