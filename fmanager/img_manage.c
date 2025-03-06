#include <img_manage.h>
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

//#define tricky(x,size)	(size)?tricky(x->next, size - 1):x->next
void fmanage_load(FILE_LIST * obj){




}

FILE_LIST * fmanage_init(const char * dir_path){
	DIR *dir;
	struct dirent * entry;

#if 0	
	struct img_file * sets = (struct img_file *)malloc(sizeof(struct img_file));
	struct img_file * head = sets;
	sets->next = NULL;
	sets->pre = NULL;
#endif

	if((dir = opendir(dir_path)) == NULL){
		perror("Failed to open dictory");
		return NULL;
		//		pthread_exit(NULL);
	}

	FILE_LIST * file_list = (FILE_LIST *) malloc(sizeof(FILE_LIST));
	file_list->head = NULL;
	file_list->size = 0;

	while((entry = readdir(dir)) != NULL){
#if 0
		ret = do_type_handle(entry->d_name);
		if(ret != NULL){
			img_list_register(sets, ret);
		}
		memset(ret, 0, sizeof(xxxhandle));
#endif
		int n = do_type_handle(entry->d_name);
		if(n > 0){
			LOG(DEBUG, "%s is a image file", entry->d_name);
			struct img_file *temp = (struct img_file*) malloc(sizeof(struct img_file));
			if(!temp) printf("%s create struct fail when malloc\n",entry->d_name);
			memcpy(temp->f_name,entry->d_name, 256);
			temp->type = n; 
			if(!file_list->head){
				file_list->head = temp;
			}else{
				temp->next = file_list->head;
				file_list->head->pre = temp;
				file_list->head = temp;
			}
			file_list->size += 1;
//			LOG(DEBUG, "!!!!!!!%s!!!!!!!!!", file_list->head->f_name);	
		}else{
			LOG(DEBUG, "%s isn't a correct image file", entry->d_name);
		}
	}
	//make a loop connect
	struct img_file * cp = file_list->head;
	while(cp){
		if(cp->next)
			cp = cp->next;
		else{
			//cp is the last element
			cp->next = file_list->head;
			file_list->head->pre = cp;
//			LOG(..., "%s \n %s", file_list->head->next->f_name, file_list->head->pre->f_name);
			goto end;
		}
	}

	LOG(DEBUG, "No image is insert or only one element!");

end:
	
	closedir(dir);

	return file_list;
}




void fm_free(FILE_LIST * file_list){
	int i;
	struct img_file * cp = file_list->head->next;
	for(i = 0;i < file_list->size;i++){
		free(file_list->head);
		file_list->head = cp;
		cp = cp->next;
	}

	free(file_list);

}
