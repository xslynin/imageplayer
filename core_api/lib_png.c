#include <lib_png.h>

unsigned char * lib_png_handle(){
	unsigned char * bitmap;

	bitmap = malloc(1024 * 600 * sizeof(unsigned int));
	if(bitmap == NULL){
		printf("WRONG malloc in %s\n", __func__);
		return NULL;
	}


	return bitmap;
}
