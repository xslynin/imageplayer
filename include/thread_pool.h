#include <pthread.h>

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


