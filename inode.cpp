#include <iostream>
#include <string>
#include <cstring>
#include "inode.hpp"
#include "disk_op.hpp"
#include <sys/stat.h>


void inode::initialize(string file_name, diskop *disk){
	
	struct stat st;

	this -> file_name = file_name;
	
	if(stat(file_name.c_str(), &st) != 0){
		fprintf(stderr, "file_size is 0");
		exit(1);
	}

	//determine file size
	//ifstream file( filename, ios::binary | ios::ate);
	//int file_size = file.tellg();
	int file_size = st.st_size;
	this->file_size = file_size;
	

	//check to see if the file already exists on the disk
	int on_disk = 0;
//TODO
	//lock here


	int index = disk -> read_free_mem_iMap();
	//file is on disk
	if(index != -1){
		on_disk = 1;
		//index = search//
	}else{
		//if file is not on disk make sure there is a free inode for it
		index = disk -> read_free_mem_iMap();
	}

	// case where there is not an available free inode
	if(index == -1){
		fprintf(stderr, "no available inodes");
		exit(1);
	}
	//number of blocks needed to store the file
	int num_blocks = file_size / disk->get_block_size();

	//vector to hold free blocks we will be giving to the inode
	vector<int> freeBlocks = disk->read_fbl();
//TODO: add code to handle the need for additional bytes for indirect blocks
	if(freeBlocks.size() < num_blocks){
		//do not conduct the write
		fprintf(stderr, "not enough available free blocks");
		exit(1);
	}

	//number if ints an indirect block can hold
	int num_indirect = disk->get_block_size() / 4;

	for(int j = 0; j < num_blocks; j++){
		if(j < 12){
			this->direct_ptrs[j] = freeBlocks[j];
		}else if(j >= 12 && j < num_indirect + 12){
			// write the next free block to the indirect block
			if(j == 12){
				this->indirect_ptrs = freeBlocks[j];
			}else{
				//write free blocks into the ptr's block
			}
			
		}else{
			if(j == num_indirect + 12){
				this->dindirect_ptrs = freeBlocks[j];
			}else{
				//write free blocks into the ptr's block
			}
		}
	}

	//write the inode to memory
	disk->update_inode_map(index);
	


//TODO
	//unlock
	//


}
