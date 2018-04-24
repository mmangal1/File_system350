#include "disk.hpp"
#include <string>
#include <fstream>

using namespace std;

void create(char* file_name, int num_blocks, int block_size){
	FILE *fp = fopen(file_name, "wb");

	int size = block_size*num_blocks;	
	char x[size];// = {};
	if(fp != NULL){
		fwrite(x, block_size, num_blocks, fp);
	}else{
		fprintf(stderr, "File did not open");
		exit(1);
	}
	fclose(fp);
}

