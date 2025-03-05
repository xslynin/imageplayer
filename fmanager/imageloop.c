#include <imageloop.h>
#if 0
void img_create_cache(struct img_file, ){

	

}

static void img_list_register(struct img_file img_list, handle){
	
	img_list->name ...
			 type  ...
			res    ...
	
	img_create_cache(sets, ...) --->  thread_pool


}
#endif


struct img_file * fmanage_init(const char * dir_path){
	DIR *dir;
	struct dirent * entry;
	
	struct img_file * sets = (struct img_file *)malloc(sizeof(struct img_file));
	struct img_file * head = sets;
	sets->next = NULL;
	sets->pre = NULL;

	if((dir = opendir(dir_path)) == NULL){
		perror("Failed to open dictory");
		return NULL;
		//		pthread_exit(NULL);
	}
	
	while((entry = readdir(dir)) != NULL){
#if 0
		ret = do_type_handle(entry->d_name);
		if(ret != NULL){
			img_list_register(sets, ret);
		}
		memset(ret, 0, sizeof(xxxhandle));
#endif
		if(entry->d_name[0] != '.'){
			memcpy(sets->f_name,entry->d_name, 256);
			sets->next = malloc(sizeof(struct img_file));
			sets->next->pre = sets;
			sets = sets->next;
		}
	}
	sets->next = head;
	head->pre = sets;
	closedir(dir);

	return head;
}
