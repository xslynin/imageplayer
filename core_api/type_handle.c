/***
 *
 *	use for solve out which image type it belong
 *	writed by lxs
 *
 * 	BELONG TO MODULE OF FILE REFFER!
 *
 *
 *
 *
 * 
 */
#include <type_handle.h>

static int head_handle(int fd){
	//in this step, we only do some file i/o rather than cp the whole file to ram
	int ret;
	unsigned char * head_check = NULL;
	
	head_check = (unsigned char *)malloc(4 * sizeof(unsigned char));// for jpg's 0xff 0xd8 ... 0xff 0xd9
	ret = read(fd, head_check, 2);
	if(ret < 0){
		perror("head read");
		goto end;	
	}
	//if bmp
	if( (*head_check == 0x42) || (*(head_check + 1) == 0x4d) )
		return 1;
	//maybe if jpg
	if( (*head_check == 0xff) || (*(head_check + 1) == 0xd8) ){
		lseek(fd, 2, SEEK_END);
		ret = read(fd, head_check, 2);
		if(ret < 0){
			perror("wrong read in second jpg verify" );
			goto end;
		}
		if( (*head_check == 0xff) || (*(head_check + 1) == 0xd9) )
			return 2;
	}

end:
	return -1;
}

static struct bmp_handler * bmp_header_handle(int fd){
	int ret;
	struct bmp_handler *bmp_data = NULL;
	unsigned int * head_info = NULL;
	
	bmp_data = (struct bmp_handler *)malloc(sizeof(struct bmp_handler));
	head_info = (unsigned int *)malloc(13 * sizeof(unsigned int));
	if(bmp_data == NULL || head_info == NULL){
		free(bmp_data);
		free(head_info);
		printf("malloc handle in header-get error\n");
		goto error;
	}
	lseek(fd, 2, SEEK_SET);
	ret = read(fd, head_info, 13 * sizeof(unsigned int));
	if(ret < 0){
		perror("read head_info");
		goto error;
	}
	//handle head info
	bmp_data->filesize    = head_info[0];
	bmp_data->dataoffset  = head_info[2];
	bmp_data->image_width = head_info[4];
	bmp_data->image_heigh = head_info[5];
	bmp_data->imagesize   = head_info[8];
	
	bmp_data->image_data  = malloc(bmp_data->imagesize);
	ret = read(fd, bmp_data->image_data, bmp_data->imagesize);
	if(ret < 0){
		perror("read bmp bitmap");
		goto error;
	}
	
	free(head_info);

	return bmp_data;


error:
	free(bmp_data->image_data);
	free(head_info);
	free(bmp_data);
	return NULL;
}



int do_type_handle(const char * file_path){
	//handle the file head infomation to confirm which type this image belong
	int n, fd;
	unsigned int *bitmap = NULL;
	struct bmp_handler * bmp = NULL;
	//struct jpg_handler * jpg = NULL;
	//open image file
	fd = open(file_path, O_RDONLY);
	if(fd < 0){
		perror("FILE OPEN WRONG!");
		return -1;
	}
	
	n = head_handle(fd);
	lseek(fd, 0, SEEK_SET);
	/*
	 1 means bmp image
	 2 means jpg image
	 */

	switch(n){
		case 1:
	//well , there I come up to a sense that this function call a special image decode function, so what should hanpen?	
	//maybe return a image array? maybe a file like test.h? maybe other? as least now I consider a cache file for buffer	
			bmp = bmp_header_handle(fd);
			bitmap = core_handle_bmp(bmp);
			free(bmp->image_data);
			free(bmp);
			if(bitmap != NULL)
				image_set(bitmap);
			else
				printf("error when getting bitmap in bmp decoder\n");
			free(bitmap);
			break;
		case 2:
//			jpg = jpg_header_handle(fd);
///			bitmap = core_handle_jpg(&jpg);
//			image_set(bitmap);
//			free(bitmap);
			break;
		default:
			perror("Wrong Image Type in this application :)");
			//display_error_message(err_code);  if design such a function
			return -1;
	}
	return 0;
}




