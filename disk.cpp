#include "disk.hpp"
#include "super_block.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void create(char* file_name, int num_blocks, int block_size){
	FILE *fp = fopen(file_name, "wb");

	int size = block_size*num_blocks;	
	char x[size];
	if(fp != NULL){
		fwrite(x, block_size, num_blocks, fp);
		fseek(fp, 0, SEEK_SET);
		fprintf(fp, "%d%d", num_blocks, block_size);
	}else{
		fprintf(stderr, "File did not open");
		exit(1);
	}
	fclose(fp);
}

