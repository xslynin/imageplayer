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
	ret = read(fd, head_check, 8);//8 for compatibation of libpng
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
	//if png?
	//by lib
	if(!png_sig_cmp(head_check, 0, 8)){
		return 3;
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

static unsigned int * jpg_lib_handle(const char * file_path){
	FILE * infile;
	JSAMPARRAY buffer;
	int row_stride,i;
	unsigned int * bitmap, *ptr;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);	
	jpeg_create_decompress(&cinfo);

	if ((infile = fopen(file_path, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", file_path);
		exit(1);
	}

	jpeg_stdio_src(&cinfo, infile);
	
	jpeg_read_header(&cinfo, TRUE);
	
	jpeg_start_decompress(&cinfo);	
	LOG(DEBUG, "get image width and height: %u %u %d",cinfo.output_width, cinfo.output_height,cinfo.output_components);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	bitmap = malloc(cinfo.output_height * cinfo.output_width * sizeof(unsigned int));
	ptr = bitmap;

	while(cinfo.output_scanline < cinfo.output_height){
		//LOG(DEBUG, " output lines %u", cinfo.output_scanline);
		jpeg_read_scanlines(&cinfo, buffer, 1);
#if 0
		memcpy(ptr, buffer[0], row_stride);
		ptr += row_stride;
#endif

#if 1
		for(i=0;i<row_stride;){
			*ptr++ = (buffer[0][i]<<16) | (buffer[0][i+1]<<8) | (buffer[0][i+2]);
			i += 3;
		}
#endif
	}
#if 0
	for(i=0;i<cinfo.output_height;i++){
		for(j=0;j<row_stride;j++){
			LOG(DEBUG, "%x %d %d", buffer[i][j], i, j);
		}
	}	
//	LOG(DEBUG, "fault before image set");
//	image_set2((unsigned char **)buffer);
//	LOG(DEBUG, "fault after");
#endif
	jpeg_finish_decompress(&cinfo);

	fclose(infile);	
	
	jpeg_destroy_decompress(&cinfo);
	return bitmap;
}

static unsigned int * png_lib_handle(const char * file_path){
//	int i,j;
	unsigned int * bitmap = NULL;

	png_structp png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
	if (!png_ptr) 
		return NULL;

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr){ 
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL); 
		return NULL; 
	}
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info){ 
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL); 
		return NULL; 
	}
	
	FILE *fp = fopen(file_path, "rb"); 
	if(!fp){ 
		return NULL; 
	}

	png_init_io(png_ptr, fp);
//	int png_transforms = PNG_TRANSFORM_STRIP_ALPHA | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_SHIFT;
	int png_transforms = PNG_TRANSFORM_STRIP_ALPHA | PNG_TRANSFORM_EXPAND;
	//	int png_transforms = PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_SHIFT;
	png_read_png(png_ptr, info_ptr, png_transforms, NULL);

	//handle the png_ptr to bitmap
	LOG(DEBUG, "width %lu height %lu bitdeep %u rowbytes %lu bpp %u", info_ptr->width, info_ptr->height, info_ptr->bit_depth,info_ptr->rowbytes, info_ptr->pixel_depth);

	//now we know the row pointer gives no 00 for align
#if 0	
	bitmap = (unsigned int *)malloc(info_ptr->height * info_ptr->width * 4);
	//printf("%ld",(unsigned int)info_ptr->height * info_ptr->width * sizeof(unsigned int));
	for(i=0;i < info_ptr->height * info_ptr->width;){
		for(j = 0;j < info_ptr->width;){
			bitmap[i] =( info_ptr->row_pointers[i % 1024][j*3]<<16)|  (info_ptr->row_pointers[i % 1024][j*3 + 1] <<8)| (info_ptr->row_pointers[i % 1024][j*3 + 2]);
			//LOG(DEBUG, "0x%x ", bitmap[i]);
			j++;
			i++;
			LOG(DEBUG, "pixel %d ", j);
		}
		LOG(DEBUG, "row %d done", i + 1);
	}
#endif
	//image_set2((unsigned char **)info_ptr->row_pointers);



	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	/* Close the file */
	fclose(fp);

	return bitmap;
}

int do_type_handle(char * name){
	//handle the file head infomation to confirm which type this image belong
	int n, fd;
#if 0
	unsigned int *bitmap = NULL;
	struct bmp_handler * bmp = NULL;
	//struct jpg_handler * jpg = NULL;
#endif
	//open image file
	char * path =malloc(256 * sizeof(char));
	memset(path, '\0', 256);
	strcat(path, "./image/");
	strcat(path, name);	
//	LOG(DEBUG, "%s", path);
	fd = open(path, O_RDONLY);
	if(fd < 0){
		perror("FILE OPEN WRONG!");
		return -1;
	}
	
	n = head_handle(fd);
	
	return n;
//	lseek(fd, 0, SEEK_SET);
	/*
	 1 means bmp image
	 2 means jpg image
	 3 means png image
	 */
#if 0
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
			bitmap = jpg_lib_handle(path);
			image_set(bitmap);
			free(bitmap);
			break;
		case 3:
			//png
			png_lib_handle(path);
			free(bitmap);
			break;
		default:
			perror("Wrong Image Type in this application :)");
			free(path);
			//display_error_message(err_code);  if design such a function
			return -1;
	}
#endif
}




