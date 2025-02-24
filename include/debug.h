#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

enum LogLevel{
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL
};

extern LogLevel this_loglevel;

#define DEBUG(level, )

#endif
