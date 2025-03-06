#ifndef _USERTS_H
#define _USERTS_H

#include <linux/input.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>


enum TS_CODE{
	TS_NONE = 0,
	TS_LEFT,
	TS_RIGHT,
	TS_CLOSE,
	TS_IGNORE
};

struct ts_data {
	int x;
	int y;
	char cond;
};

#define TS_XMASKCOND 0x1
#define TS_YMASKCOND 0x2

int ts_routine(int fd);
#endif
