#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <bitset>
//#include "inode.hpp"
#include "super_block.hpp"
#include "disk_op.hpp"
#include <sys/stat.h>

using namespace std;
diskop::diskop(){
	buffer_len = 3;
}




void diskop::inode::initialize(string filename, diskop *disk){
	
	struct stat st;

	this->file_name = filename;
	cout << this->file_name << endl;
	
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
	int num_blocks;
	if(file_size < disk->get_block_size()){
		num_blocks = 1;
	}else if (file_size % disk->get_block_size() == 0){
		num_blocks = (file_size / disk->get_block_size());
	}else{
		num_blocks = (file_size / disk->get_block_size()) + 1;
	}

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
			this->direct_ptrs[j] = freeBlocks.at(j);
		}else if(j >= 12 && j < num_indirect + 12){
			// write the next free block to the indirect block
			if(j == 12){
				this->indirect_ptrs = freeBlocks.at(j);
			}else{
	//TODO			//write free blocks into the ptr's block
				// pass to the scheduler thread to write to the disk file
			}
			
		}else{
			if(j == num_indirect + 12){
				this->dindirect_ptrs = freeBlocks.at(j);
			}else{
		//TODO		//write free blocks into the ptr's block
				// pass to the scheduler thread to write to the disk file
			}
		}
	}

	//write the inode to memory
	disk->write_inode_to_disk(disk->get_offset() + (disk->read_free_mem_iMap() * disk -> sb.block_size) - (256 * disk -> sb.block_size), disk -> fp, this);
	disk -> inode_mem.push_back(this);
	disk->update_inode_map(index);
	disk->write_data_to_disk(filename);

	//disk->write_inode_to_disk(disk->get_offset() + (disk->read_free_mem_iMap() * disk -> sb.block_size) - (256 * disk -> sb.block_size), disk -> fp, this);
	//(disk->read_free_mem_iMap() * disk -> sb.block_size)


//TODO
	//unlock
	//


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
		inode_map[i] = 0;
	}
	/* this is the size of the free block list because the blocks reserved for inodes, inode map, free block list, and super block = 259 */
	double offset = num_blocks-259;
	offset = offset / 8;
	offset = offset / block_size;

	offset = ((int)(offset / 1) +258);
	offset = offset * block_size;
	//cout << "test: " << fbl_block_count << endl;

	fbl_block_count = offset;
	free_block_list = (int*)malloc(sizeof(int)*fbl_block_count);
	for(int i = 0; i < (fbl_block_count); i++){
		free_block_list[i] = 0;
	}
	write_inode_map(inode_map, file_name, block_size, num_blocks, fp);
	write_fbl(free_block_list, file_name, block_size, num_blocks,fp);
	write_sb(offset, block_size, num_blocks, fp);
	sb.block_size = block_size;
	sb.num_blocks = num_blocks;
	sb.offset = offset;
	cout << read_free_disk_iMap() << endl;	
	cout << this->buffer_len << endl;
	node = new inode();
	node->initialize("foo.txt",  this);
	fclose(fp);
	fp = fopen(file_name, "rb");
	read_inode_from_disk(offset - (256 * sb.block_size), fp);
	fclose(fp);
}

vector<int> diskop::read_fbl(){
	vector<int> ret_val;
	for(int i = 0; i < fbl_block_count; i++){//make the loop 'free_block_count' times
		if(free_block_list[i] == 0){
			ret_val.push_back(sb.offset+(sb.block_size * i));
		}
	}
	return ret_val;
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

void diskop::write_fbl(int free_block_list[], char* file_name, int block_size, int num_blocks, FILE *fp){
	int currbyte = 0;
	int bitcount = 0;
	int totalcount = 0;
	fseek(fp, 2*block_size, SEEK_SET);

	for(int i = 0; i < fbl_block_count; i++){
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

}

void diskop::write_sb(int offset, int block_size, int num_blocks, FILE *fp){
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d%d%d", num_blocks, block_size, offset);
}

void diskop::write_inode_to_disk(int offset, FILE *fp, inode *node){
	fseek(fp, offset, SEEK_SET);
	fwrite(&(node -> file_name), sizeof(node -> file_name), 1, fp);
	cout << "file name: " << node -> file_name << endl;
	fwrite(&(node -> file_size), sizeof(node -> file_size), 1, fp);
	cout << "file size: " << node -> file_size << endl;
	for(int i = 0; i < 12; i++){
		fwrite(&(node -> direct_ptrs[i]), sizeof(node -> file_name), 1, fp);
		cout << "dir ptr: " << i << " " << node -> direct_ptrs[i] << endl;
	}
	fwrite(&(node -> indirect_ptrs), sizeof(node -> indirect_ptrs), 1, fp);
	cout << "indir ptr: " << node -> indirect_ptrs << endl;
	fwrite(&(node -> dindirect_ptrs), sizeof(node -> dindirect_ptrs), 1, fp);
	cout << "dindir ptr: " << node -> dindirect_ptrs << endl;
}
	

void diskop::read_inode_from_disk(int offset, FILE *fp){
	fseek(fp, offset, SEEK_SET);
	inode* node1 = new inode();
	fread(&(node1 -> file_name), sizeof(node1 -> file_name), 1, fp);
	cout << "file name: " << node1 -> file_name << endl;
	fread(&(node1 -> file_size), sizeof(node1 -> file_size), 1, fp);
	cout << "file size: " << node1 -> file_size << endl;
	for(int i = 0; i < 12; i++){
		fread(&(node1 -> direct_ptrs[i]), sizeof(node1 -> file_name), 1, fp);
		cout << "dir ptr: " << i << " " << node1 -> direct_ptrs[i] << endl;
	}
	fread(&(node1 -> indirect_ptrs), sizeof(node1 -> indirect_ptrs), 1, fp);
	cout << "indir ptr: " << node1 -> indirect_ptrs << endl;
	fread(&(node1 -> dindirect_ptrs), sizeof(node1 -> dindirect_ptrs), 1, fp);
	cout << "dindir ptr: " << node1 -> dindirect_ptrs << endl;
	inode_mem.push_back(node1);
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
void diskop::cat(char *filename){
	vector<inode*>::iterator iter;
	bool found = false;
	bool done = false;
	int count = 3;
	for(iter = inode_mem.begin(); iter != inode_mem.end(); iter++){
		inode *test = *iter;
		if(test->file_name == filename){
			fseek(fp, count*block_size, SEEK_SET);
			char buff[sb.block_size];
			fread(&buff, startByte, test->file_size, fp);
			cout << buff << endl;
			found = true;
			break;
		}
		count++;
	}
	if(!found){
		count = 3;
		read_inode_from_disk(sb.offset-256*sb.block_size, fp);
		for(iter = inode_mem.begin(); iter != inode_mem.end(); iter++){
			inode *test = *iter;
			if(test->file_name == filename){
				fseek(fp, count*block_size, SEEK_SET);
				char buff[sb.block_size];
				fread(&buff, startByte, test->file_size, fp);
				cout << buff << endl;
				found = true;
				break;
			}
			count++;
		}
		if(!found){
			fprintf(stderr, "error: file %s could not be found", filename);
			exit(1);
		}
	}

}
void diskop::del(char* filename){}
void diskop::write_data_to_disk(string filename){
	vector<inode*>::iterator iter;
	bool found = false;
	bool done = false;
	for(iter = inode_mem.begin(); iter != inode_mem.end(); iter++){
		inode *test = *iter;
		if(test->file_name == filename){
			found = true;
			break;
		}
	}
	if(!found){
		read_inode_from_disk(sb.offset-256*sb.block_size, fp);
		for(iter = inode_mem.begin(); iter != inode_mem.end(); iter++){
			inode *test = *iter;
			if(test->file_name == filename){
				found = true;
				break;
			}
		}
		if(!found){
			inode *new_node = new inode();
			new_node -> initialize(filename, this);
			for(iter = inode_mem.begin(); iter != inode_mem.end(); iter++){
				inode *test = *iter;
				if(test->file_name == filename){
					found = true;
					break;
				}
			}
		}
		if(!found){
			fprintf(stderr, "error: file %s could not be found", filename);
			exit(1);
		}
	}
	

	FILE *file_to_write = fopen(filename.c_str(), "rb");
	inode *node_to_write = *iter;
	for(int i = 0; i < 12; i++){
		if(node_to_write->direct_ptrs[i] == 0){
			done = true;
			break;
		}
		if(!done){
			fseek(fp, sb.offset + node_to_write -> direct_ptrs[i]*sb.block_size, SEEK_SET);
			char buff[sb.block_size];
			fread(&buff, sb.block_size, 1, file_to_write);
			fwrite(&buff, sb.block_size, 1, fp);
		}
	}
	fclose(file_to_write);
}

void diskop::read(char *filename, int startByte, int numByte){
	vector<inode*>::iterator iter;
	bool found = false;
	bool done = false;
	for(iter = inode_mem.begin(); iter != inode_mem.end(); iter++){
		inode *test = *iter;
		if(test->file_name == filename){
			char buff[sb.block_size];
			fread(&buff, startByte, numByte, test->file_name);
			cout << buff << endl;
			found = true;
			break;
		}
	}
	if(!found){
		read_inode_from_disk(sb.offset-256*sb.block_size, fp);
		for(iter = inode_mem.begin(); iter != inode_mem.end(); iter++){
			inode *test = *iter;
			if(test->file_name == filename){
				char buff[sb.block_size];
				fread(&buff, startByte, numByte, test->file_name);
				cout << buff << endl;
				found = true;
				break;
			}
		}
		if(!found){
			fprintf(stderr, "error: file %s could not be found", filename);
			exit(1);
		}
	}

}


int diskop::read_free_mem_iMap(){
	for(int x = 0; x < sizeof(inode_map)/sizeof(inode_map[0]); x++){
		if(inode_map[x] == 0){
			cout << x << endl;
			return x;
		}
	}
	return -1;
}


/*int diskop::read_free_mem_iMap(){
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
*/
int diskop::get_offset(){
	return sb.offset;
}

int diskop::get_block_size(){
	return sb.block_size;
}

int diskop::get_num_blocks(){
	return sb.num_blocks;
}

void diskop::update_inode_map(int index){
	inode_map[index] = 1;
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


