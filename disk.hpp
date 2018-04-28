#ifndef DISK_HPP
#define DISK_HPP

#include <string>

	void create(char* filename, int num_blocks, int block_size);
	void write_inode_map(int inode_map[], char* file_name, int block_size, int num_blocks, FILE *fp);
	int write_fbl(int free_block_list[], char* file_name, int block_size, int num_blocks, FILE *fp);
	void write_sb(int offset, int block_size, int num_blocks, FILE *fp);
#endif 
