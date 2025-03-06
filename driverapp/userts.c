/*
 *
 *	so let's talk a bit about the touch screen's app's level development
 *	
 *	now I have try for the input system what it will do
 *	and i wanna insert a tiny introduce in linux/input.h 's define
 *	type 0 means EV_SYN event
 *	while 1 means EV_KEY, 3 means EV_ABS,where we congnize it as touch screen
 *	
 *	code in different type owns different meaning, while value means specific content in certain code
 *	there code 0 and 1 in type 3 means X and Y, code 24 means PRESS
 *
 *	code 330 in type 1 means BTN_TOUCH
 *	well, the driver is really pretty tricky
 *
 *
 *	when I press:
 *		get 5 input_event value, they are:
 *			type 3 code 0 value X
 *			type 3 code 1 value Y
 *			type 1 code 330 value 1 (means a touch event)
 *			type 3 code 24 value 1 means (while then unpress it comes to 0)
 *			type 0 code 0 value 0 (Synchron event)
 *
 *	And, when I keep pressing, move finger at the same time
 *	it keep output information format like:
 *		type 3 code 0/1 value X/Y
 *		type 0 code 0   value 0
 *
 *	OR
 *		type 3 code 0   value X
 *		type 3 code 1   value Y
 *		type 0 code 0   value 0
 *
 *	dependence on how you finger move, it will continuous output like above format
 *
 *	another tips: the value drive provided about x,y axis
 *	start at left top and x is width oriented, while y is height oriented
 *	
 *
 *	HOWEVER!  IN THIS PROJECT, THE DISCIPLINE DON'T NEED
 *
 * consider abort those value equals 0 's event
 */


#include <userts.h>

static int ts_app(struct ts_data ts){
	//emmmm, maybe y is not important
	if(ts.x <= 300){
		return TS_LEFT;
	}else if(ts.x >=800){
		return TS_RIGHT;
	}else{
		if(ts.y > 500){
			return TS_CLOSE;
		}
	}
	return TS_IGNORE;
}

int ts_routine(int fd){
	//handle only the type 3 code 0/1 
	//end at type 3 code 24 value 0
	//abort those type don't equals 3
	int res = 0;
	//the output of driver is 5 when press and 11 when unpress
	struct input_event ev;
	struct ts_data ts = {0};
	while(1){
//		LOG(DEBUG, "A routine");	
		memset(&ev, '\0',  sizeof(struct input_event));
		if(read(fd, &ev, sizeof(struct input_event)) < 0){
			perror("Read");
			break;
		}
		if(ev.type == 3 && ev.code == 0){
			ts.x = ev.value;
			read(fd, &ev, sizeof(struct input_event));
			ts.y = ev.value;
			res = ts_app(ts);
//			LOG(DEBUG, "hit!");
//			read(fd, &ev, sizeof(struct input_event));
			break;
		}
#if 0
		//first for x axis
		ts.x = ev[0].value;
		ts.y = ev[1].value;
		if( (ev[2].type == 1) && (ev[2].code == 330) && (ev[2].value == 1) ){
			res = ts_app(ts);
			LOG(DEBUG, "hit!");
//			read(fd, ev, 16 * sizeof(struct input_event));
			break;
		}
#endif
	}
	return res;
}




//RESERVED case for input test
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
