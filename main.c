#include <stdio.h>
#include <thread_pool.h>
#include <type_handle.h>
#include <userfb.h>
#include <debug.h>
#include <userts.h>
#include <img_manage.h>
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


#define FB_DF "/dev/fb0"
#define TS_DF "/dev/input/event1"
#define IMG_ROOT "./image"

int main(){
	int ret, fd;
	FILE_LIST * img_filelist;
	FB_HANDLER * fb;
	
	ret = 0;
	//separate the thread pool to child progress

	if(thread_pool_init() ){
		printf("init thread pool wrong!:( \n");
		ret = -1;
		goto err_thread_pool;;
	}	
	
	//fb and touch should be parent progress
	fd = open(FB_DF,O_RDWR);
	if(fd < 0)
		perror("open fb device");
	fb = fb_init(fd);
	if(!fb){
		perror("fb_init ");
		goto err_fb;
	}

#if 0
	if(ts_init()){
		printf("initate the touchscreen wrong!:( \n");
		return -1;
#endif


#if 0
	struct input_event ev;
	fd = open("/dev/input/event1", O_RDONLY);
	if(fd < 0){
		perror("open ts device");
		return 1;
	}
	while(1){
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd, &ev, sizeof(struct input_event));
		LOG(DEBUG, "type %hd code %hd value:%d", ev.type, ev.code, ev.value);
	}
//	close(fd_ts);
#endif




#if 1
	fd = open(TS_DF, O_RDONLY);
	if(fd < 0){
		perror("open ts device");
		goto err_ts;
	}
	//initation
	img_filelist = fmanage_init(IMG_ROOT);
	if(!img_filelist){
		perror("fmanager module error");
		goto err_fmanage;
	}
	
	struct img_file * cp = img_filelist->head;
	while(1){
//		display current image linklist points to
		
//		do_type_handle(img_file->f_name);
		LOG(DEBUG, "%s", cp->f_name);
#if 0
		//		fresh the double buffer for two direction
		listening the touch screen for img change
		if	pre   ... load the remaining half data //there consider one more buffer in ram, that is, keep a img size's ram to save the half data don't be load 
						call fb api to change the windows for display
							
		elif next ... load the remaining half data

		elif close ... break;
#endif
		ret = ts_routine(fd);
		if(ret == TS_LEFT){
			LOG(DEBUG, "left");
			cp = cp->pre;
		}else if(ret == TS_RIGHT){
			LOG(DEBUG, "right");
			cp = cp->next;
		}else if(ret == TS_CLOSE){
			LOG(DEBUG, "CLOSE");
			break;
		}
		//LOG(DEBUG, "-----------a click");
	}
#endif
	//free all the resource 
	//parent progress infinite the loop to handle the touch event



//ERROR handle for reflection
err_fmanage:
	fm_free(img_filelist);
err_ts:
	close(fd);	
err_fb:
	fb_free(fb);
err_thread_pool:	
	thread_pool_destroy();

	
	
	return ret;

}







