#include "disk.hpp"
#include "super_block.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

void create(char* file_name, int num_blocks, int block_size){
	/* Creates disk */
	FILE *fp = fopen(file_name, "wb");
	int size = block_size*num_blocks;	
	char x[size];
	/* Superblock creation
	 * Contains num_blocks and block_size 
	 * TODO: Add in offsets for inodes, data, inode map, and free block list*/
	if(fp != NULL){
		/* create the file of size block_size*num_blocks */
		fwrite(x, block_size, num_blocks, fp);
		/* go to the beginning of the file and write superblock data */
		/*fseek(fp, 0, SEEK_SET);
		fprintf(fp, "%d%d", num_blocks, block_size);*/
	}else{
		fprintf(stderr, "File did not open");
		exit(1);
	}

	/* inode map creation in memory 
	 * Initializing all to unused */
	int inode_map[256];
	for(int i = 0; i < 256; i++){
		inode_map[i] = 1;
	}
	/* this is the size of the free block list because the blocks reserved for inodes, inode map, free block list, and super block = 259 */
	int fbl_block_count = num_blocks - 259;
	//Need more than one block for free list.
	int num = 259;
	float testing = (num_blocks - num);
	int bs = block_size*8;
	while(testing > bs){
		num++;
		bs = bs*2;
		testing = num_blocks - num;
			
	}
	cout << "test: " << num << endl;
	/*
	int free_block_list[fbl_block_count];
	for(int i = 0; i < (fbl_block_count); i++){
		free_block_list[i] = 0;
	}
	write_inode_map(inode_map, file_name, block_size, num_blocks, fp);
	write_sb(write_fbl(free_block_list, file_name, block_size, num_blocks, fp), block_size, num_blocks, fp);
	fclose(fp);
*/
	/*
	fp = fopen(file_name, "rb");
	uint8_t test = 1;
	int total = 0;
	fseek(fp, block_size+total, SEEK_SET);
	for(int i = 0; i < 8; i++){
		fread(&test, 1, 1, fp);
		printf("%d\n", test);
		total++;
		fseek(fp, block_size+total, SEEK_SET);
	}*/
}

void write_inode_map(int inode_map[], char* file_name, int block_size, int num_blocks, FILE *fp){
	uint8_t currbyte = 0;
	int bitcount = 0;
	int totalcount = 0;
	/* go one block to the origin to write inode bitmap 
	 * Need to write bytes to a file.. so 8 bits at a time*/
	fseek(fp, block_size, SEEK_SET);
	for(int i = 0; i < 256; i++){
		currbyte = (currbyte << 1) | inode_map[i];
		bitcount++;
		if(bitcount == 8){
			totalcount++;
			fwrite(&currbyte, 8, 1, fp);
			//fputc(currbyte, fp);
			fseek(fp, block_size+totalcount, SEEK_SET);
			currbyte = 0;
			bitcount = 0;
		}
	}
}
/*
int write_fbl(int free_block_list[], char* file_name, int block_size, int num_blocks, FILE *fp){
	int currbyte = 0;
	int bitcount = 0;
	int totalcount = 0;
	fseek(fp, 2*block_size, SEEK_SET);
	for(int i = 0; i < (num_blocks - 259); i++){
		currbyte = (currbyte << 1) | free_block_list[i];
		bitcount++;
		if(bitcount == 8){
			totalcount++;
			fwrite(&currbyte, 8, 1, fp);
			fseek(fp, block_size+totalcount, SEEK_SET);
			currbyte = 0;
			bitcount = 0;
		}
	}
	if(bitcount != 0){
		while(bitcount != 8){
			currbyte = (currbyte << 1) | 1;
			bitcount++;
		}
		totalcount++;
		fwrite(&currbyte, 8, 1, fp);

	}
	int num = 0;
	int offset = (block_size+totalcount)%block_size;
	while(offset != 0){
		offset = (block_size+totalcount+num)%block_size;
		num++;
	}
	offset = (block_size+totalcount+num)/block_size + 2*block_size;
	return offset;
}*/
/*
void write_sb(offset, char* file_name, int block_size, int num_blocks, FILE *fp){
	fseek(fp, 8, SEEK_SET);
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d%d%d", num_blocks, block_size, offset);
}*/
