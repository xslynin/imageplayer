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

//extern LogLevel this_loglevel;
#define DEBUGMODEL 1

#ifdef DEBUGMODEL
#define LOG(level,fmt, ...) do{						\
						fprintf(stderr,"[" #level "] %s: %s--" fmt "\n" , __func__, __LINE__, ##__VA_ARGS__)				\
				}while(0)

#else
#define LOG(...) 

#endif
#endif
