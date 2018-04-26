#ifndef DISK_HPP
#define DISK_HPP

#include <string>

	void create(char* filename, int num_blocks, int block_size);
	void write_inode_map(int inode_map[], char* file_name, int block_size, int num_blocks);
#endif 
