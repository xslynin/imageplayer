#include <stdio.h>
#include <thread_pool.h>
#include <userfb.h>
#include <debug.h>
#include <userts.h>
//#include <img_manage.h>
#include <type_handle.h>

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
	struct img_file * cp;

	ret = 0;

#if 0
	if(thread_pool_init() ){
		printf("init thread pool wrong!:( \n");
		ret = -1;
		goto err_thread_pool;;
	}	
#endif

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
	cp = img_filelist->head;

#if 1 //at beginning   set the image
	decoder_handle(cp);	
//	free(cp->pspecial);
	decoder_handle(cp->pre);
	decoder_handle(cp->next);
	image_set(fb, cp);
#endif


	while(1){
	//	LOG(DEBUG, "%s", cp->f_name);

		
ignore_loop:
		ret = ts_routine(fd);// waiting for the change
		if(ret == TS_LEFT){
	//		LOG(DEBUG, "left");
//			fb->state ^= 0x2;// pre image fill the bottom half set
			fb->state &= ~(1 << 1);
			free(cp->next->pspecial);
			cp->next->pspecial = NULL;
//			LOG(DEBUG, "after left and free %p, we got %p %p", cp->next->pspecial, cp-);
			cp = cp->pre;
		}else if(ret == TS_RIGHT){
	//		LOG(DEBUG, "right");
			fb->state |= 1 << 1;
			free(cp->pre->pspecial);
			cp->pre->pspecial = NULL;
			cp = cp->next;
		}else if(ret == TS_CLOSE){
	//		LOG(DEBUG, "CLOSE");
			background_set(1024, 1200, 0x00000000, fb);
			break;
		}else{
			//ret == TS_IGNORE
			goto ignore_loop;
		}
		
		decoder_handle(cp->pre);
		decoder_handle(cp->next);
		LOG(DEBUG, "pre%p\n cur%p\n next%p", cp->pre->pspecial, cp->pspecial, cp->next->pspecial);		
		//now it's time to change
		image_before_show(fb, cp);//input the newest to show
		fb_switch(fb);//actual function for displaying the image
		image_preload(fb, cp);

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

#if 0
err_thread_pool:	
	thread_pool_destroy();
#endif
	
	return ret;

}







