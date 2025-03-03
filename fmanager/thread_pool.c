#include <thread_pool.h>

static Pool * pool;

void * task_work(void * arg){
	
	while(1){
		pthread_mutex_lock(&pool->lock);
		
		while(pool->count == 0){
			pthread_cond_wait(&pool->cond, &pool->lock);
		}	
		//fetch the assignment
		Task * task = pool->task_head;
		if(task){
			pool->task_head = pool->task_head->next;
			if(!pool->task_head){
				pool->task_tail = NULL;
			}
			pool->count--;
		}

		pthread_mutex_unlock(&pool->lock);
		
		if(!task)
			return NULL;
		//excute the task
		task->func(task->arg);
		free(task);
	}
	return NULL;
}

int thread_pool_init(){
	int i;

	pool = malloc(sizeof(Pool));
	if(!pool){
		return THR_ALLOC;
	}
	pool->task_head = NULL;
	pool->task_tail = NULL;
	pool->count = 0; // init waitting task
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->cond, NULL);
	
	for(i = 0; i < MAX_THREAD; i++){
		if(pthread_create(&pool->threads[i], NULL, task_work, NULL))
			return THR_CREAT;
	}
	return 0;
}

void thread_pool_destroy(){
	int i;

	pthread_mutex_lock(&pool->lock);
	pool->count = 100;
	pthread_cond_broadcast(&pool->cond);
	pthread_mutex_unlock(&pool->lock);

	for(i = 0; i < MAX_THREAD; i++){
		pthread_join(pool->threads[i], NULL);
	}
	printf("shutdown all threads!\n");
}

int thread_pool_add_task(void (*func)(void *), void * arg){
	Task *task = malloc(sizeof(Task));
	if(!task){
		return THR_ALLOC;
	}
	task->func = func;
	task->arg  = arg;
	task->next = NULL;
	
	pthread_mutex_lock(&pool->lock);
	
	//for first add assignment
	if(pool->task_head == NULL){
		pool->task_head = task;
		pool->task_tail = task;
			
	}else{
		//no first add
		pool->task_tail->next = task;
		pool->task_tail = task;
	}
	
	pool->count++;
	
	pthread_cond_signal(&pool->cond);
	pthread_mutex_unlock(&pool->lock);
	return 0;
}
