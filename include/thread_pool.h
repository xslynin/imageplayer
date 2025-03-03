#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include <stdlib.h>
#include <pthread.h>
#include <debug.h>

#define MAX_TASK 10
#define MAX_THREAD 4

typedef struct task_listnode {
	void (*func)(void *);
	void * arg;
	struct task_listnode *next;
} Task;

typedef struct pool{
	pthread_t threads[MAX_THREAD];
	Task *task_head;
	Task *task_tail;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	int count; // plus one every task is added
} Pool;

enum pthread_pool{
	THR_ALLOC,
	THR_CREAT
};

void * task_work(void * arg);
int thread_pool_init();
void thread_pool_destroy();
int thread_pool_add_task(void (*func)(void *), void * arg);



#endif
