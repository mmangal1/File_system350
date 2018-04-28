#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <bitset>
#include "inode.hpp"
#include "super_block.hpp"
#include "disk_op.hpp"

using namespace std;
diskop::diskop(){
}
void diskop::create(char* file_name, int num_blocks, int block_size){
	/* Creates disk */
	fp = fopen(file_name, "wb");
	int size = block_size*num_blocks;	
	char x[size];
	/* Superblock creation
	 * Contains num_blocks and block_size 
	 * TODO: Add in offsets for inodes, data, inode map, and free block list*/
	if(fp != NULL){
		/* create the file of size block_size*num_blocks */
		fwrite(x, block_size, num_blocks, fp);
		/* go to the beginning of the file and write superblock data */
	}else{
		fprintf(stderr, "File did not open");
		exit(1);
	}

	/* inode map creation in memory 
	 * Initializing all to unused */
	for(int i = 0; i < 256; i++){
		inode_map[i] = 1;
	}
	/* this is the size of the free block list because the blocks reserved for inodes, inode map, free block list, and super block = 259 */
	int fbl_block_count = num_blocks - 259;
	//Need more than one block for free list.
	int num = 259;
	int bs = block_size*8;
	while(fbl_block_count > bs){
		num++;
		bs = bs*2;
		fbl_block_count = num_blocks - num;
			
	}
	//cout << "test: " << fbl_block_count << endl;

	
	int free_block_list[fbl_block_count];
	for(int i = 0; i < (fbl_block_count); i++){
		free_block_list[i] = 1;
	}
	write_inode_map(inode_map, file_name, block_size, num_blocks, fp);
	int offset = write_fbl(free_block_list, file_name, block_size, num_blocks,fp);
	write_sb(offset, block_size, num_blocks, fp);
	sb.block_size = block_size;
	sb.num_blocks = num_blocks;
	sb.offset = offset;
	cout << read_free_disk_iMap() << endl;
	fclose(fp);
}

void diskop::write_inode_map(int inode_map[], char* file_name, int block_size, int num_blocks, FILE *fp){
	uint8_t currbyte = 0;
	int bitcount = 0;
	int totalcount = 0;
	inode_map[60] = 0;
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

int diskop::write_fbl(int free_block_list[], char* file_name, int block_size, int num_blocks, FILE *fp){
	int currbyte = 0;
	int bitcount = 0;
	int totalcount = 0;
	fseek(fp, 2*block_size, SEEK_SET);
	free_block_list[49] = 0;
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
	return offset-256;
}

void diskop::write_sb(int offset, int block_size, int num_blocks, FILE *fp){
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d%d%d", num_blocks, block_size, offset);
}

void diskop::write_inode_to_disk(int offset, FILE *fp, inode *node, int block_size){
	fseek(fp, block_size*offset, SEEK_SET);
	fwrite(&(node -> file_name), sizeof(node -> file_name), 1, fp);
	fwrite(&(node -> file_size), sizeof(node -> file_size), 1, fp);
	for(int i = 0; i < 12; i++){
		fwrite(&(node -> direct_ptrs[i]), sizeof(node -> file_name), 1, fp);
	}
	fwrite(&(node -> indirect_ptrs), sizeof(node -> indirect_ptrs), 1, fp);
	fwrite(&(node -> dindirect_ptrs), sizeof(node -> dindirect_ptrs), 1, fp);
}
	
/*int diskop::search(char* filename){
	for(int x = 0; x < sizeof(imap)/sizeof(imap[0]); x++){
		if(imap[x] == 1){
			if(strcmp(imap[x].file_name, filename) == 0){
				return x;
			}
		}
	}
	return -1;

}*/
void diskop::import(char* ssfs_filename, char* unix_filename){}
void diskop::cat(char* filename){}
void diskop::del(char* filename){}
void diskop::write(char *filename, char c, int startByte, int numByte){

/*
	int file_index = search(filename);
	inode file_inode;
	
	fseek(fp, file_index*sb.block_size, SEEK_SET);
	fread(&file_inode, sizeof(inode), 1, fp);
*/	
	
}

void diskop::read(char *filename, int startByte, int numByte){
/*	int inode_index = search(filename);
	inode inode_1;
	
	fseek(fp, inode_index*sb.block_size, SEEK_SET);
	fread(&inode_1, sizeof(inode), 1, fp);
	rewind(fp);
	
	char *data = (char*)malloc(sizeof(char)*sb.block_size);
	int beg_byte = startByte%(sb.block_size);
	int num_blocks = numByte/(sb.block_size);
	if((numByte%sb.block_size) > 0){
		num_blocks++;
	}

	for(int x = startByte; x < 12; x++){
		//check bit of data block for valid
		rewind(fp);
		fseek(fp, sb.block_size*inode_1[x], SEEK_SET);
		fread(data, sb.block_size, sizeof(char), fp);
		cout << data[beg_byte];
		beg_byte++;
		//mod by sb.block_size????
	}
*/			
}

int diskop::read_free_mem_iMap(){
	uint8_t byte = 0;
	for(int x = 0; x < sizeof(inode_map)/sizeof(inode_map[0]); x++){
		byte = inode_map[x];
		
		if(byte == 0){
			char buff[8];
			fseek(fp, 8, SEEK_SET);
			fread(buff, sizeof(int), 1, fp);
			return x + atoi(buff);
		}
	}
	return -1;
}

int diskop::read_free_disk_iMap(){
	int bit_index = 0;
	char buff[32];
	fseek(fp, sb.block_size, SEEK_SET);
	fread(buff, 1, 32, fp);
	for(int x = 0; x < 32; x++){
		cout << buff[x] - '0' << endl;	
		if((buff[x] - '0') == 0){
			cout << "found it" << endl;
			return bit_index;
		}else if((buff[x]) < 255){
			cout << "didnt findd it" << endl;
			for(int y = 0; y < 8; y++){
				int curr_bit = (buff[x] & (1 << (y))) ;
				if(curr_bit == 0){
					return bit_index;
				}
				bit_index++;
			}
		}
		bit_index+=8;
		
	}
	return -1;
}

/*void diskop::update_inode(char* filename, int index){
	fseek(fp, index*sb.block_size, SEEK_SET);
	fwrite(&(this->imap[index]), sizeof(this->imap[index]), 1, fp);
}

void diskop::list();
void diskop::shutdown(){
	fclose(fp);
}*/


