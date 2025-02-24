#include <bmp_decoder.h>

unsigned int * core_handle_bmp(const struct bmp_handler * data){
	//in this file , i only consider a well handle file to decode
	unsigned char * data_cp;
	unsigned char b,g,r;
	unsigned int *row_color = NULL;
	unsigned int *row_cp = NULL;
	int i,j;
#if 0
	bmp_width =* (unsigned int *)(data + 0x12);
	printf("%u\n", bmp_width);
	bmp_heigh =* (unsigned int *)(data + 0x16);
	printf("%u\n", bmp_heigh);
	pixel_begin_offset =* (unsigned int *) (data + 0xa);
	printf("%u\n", pixel_begin_offset);	
	data_cp = data + pixel_begin_offset;
	//printf("TEST IN bmp 1  0x%x \n", *data_cp);	
	origin_size = *(unsigned int *)(data + 0x22);
	printf("%d---- %x \n", origin_size, origin_size);
	
#endif
	row_color = (unsigned int *)malloc(data->imagesize * 4 / 3);
	//printf("0x%x \n", *(unsigned int *)(data + 0x22));
	// malloc success?
	if(row_color == NULL){
		perror("image space allocate");
	}
//	row_cp = row_color;
	//column
//	printf("TEST %p\n", row_color);
	data_cp = data->image_data;
	printf("TEST %p\n", data_cp);

	for(j = 1;j < data->image_heigh;j++){
		//one row cycle
		//before fill the data, locate the pointer
//		if(j >= bmp_heigh - 1)
//			printf("%p ", row_cp);
		row_cp = row_color + ((data->image_heigh - j) * data->image_width);
		
//		if(j >= bmp_heigh - 1)
//			printf("%p\n", row_cp);
		
		
		for(i = 0;i < data->image_width; i++){
//			if(j == bmp_heigh - 1)
//				printf("%p  ||", row_cp);
			b = *data_cp++;
			g = *data_cp++;
			r = *data_cp++;
			
//				if(j == bmp_heigh - 1)
//					printf("%p ||", row_cp);
			*row_cp++ = (b | (g << 8) | (r << 16));	
//				if(j == bmp_heigh - 1)
//					printf("%p \n", row_cp);

				//printf("0x%x\n", *(row_cp - 1));
		}
		//printf("wrong?");
		data_cp += data->image_width * 3 % 4;
		//printf("%u \n", data->image_width * 3 % 4);
		//ATTENTION!! row above is begin with the last row
		//so try to reorder this row

	}
	return row_color;
}
