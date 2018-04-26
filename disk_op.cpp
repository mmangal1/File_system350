#include <iostream>
#include "disk_op.hpp"
#include "inode.hpp"

using namespace std;

diskop::diskop(char* filename, int buffer_len){
	this->buffer_len = buffer_len;
	
	//shared buffer?
	buf_ptr = (int*)mmap(0, sizeof(buffer_len), PROT_WRITE|PROT_READ, MAP_SHARED, -1, 0);

	if(buf_ptr < 0){
		fprintf(stderr, "Error with shared memory");
		exit(1);
	}
	
	fp = fopen(filename, "rb+");
	if(fp != NULL){
		fread(&sb, sizeof(superblock), 1, fp);
		fseek(fp, sb.block_size, SEEK_SET);
	
		char* imap = (char*)malloc(sizeof(char) * sb.block_size);
		char* dmap = (char*)malloc(sizeof(char) * sb.block_size);

		//initialize imap and dmap to 0
		for(int x = 0; x < sb.block_size; x++){
			imap[x] = 0;
			dmap[x] = 0;
		}
		
		//populate file with imap
		rewind(fp);
		fseek(fp, sb.block_size, SEEK_SET);
		fwrite(imap, sb.block_size, sizeof(char), fp);
		this->imap = imap;

		//populate file with dmap
		rewind(fp);
		fseek(fp, sb.block_size*2, SEEK_SET);
		fwrite(dmap, sb.block_size, sizeof(char), fp);
		this->dmap = dmap;
	}
	
}
int search(char* filename){
	for(int x = 0; x < imap.size(); x++){
		if(imap[x] == 1){
			if(strcmp(imap[x].file_name, filename) == 0){
				return x;
			}
		}
	}
	return -1;

}
void create(char* filename);
void import(char* ssfs_filename, char* unix_filename);
void cat(char* filename);
void del(char* filename);
void write(char *filename, char c, int startByte, int numByte);

void read(char *filename, int startByte, int numByte){
	int inode_index = search(filename);
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

	for(int x = st; x < 12; x++){
		//check bit of data block for valid
		rewind(fp);
		fseek(fp, sb.block_size*inode_1[x], SEEK_SET);
		fread(data, sb.block_size, sizeof(char), fp);
		cout << data[beg_byte];
		beg_byte++;
		//mod by sb.block_size????
	}
			
}

void update_inode(char* filename, int index){
	fseek(fp, index*sb.block_size, SEEK_SET);
	fwrite(&(this->imap[index]), sizeof(this->imap[index]), 1, fp);
}

void list();
void shutdown(){
	fclose(fp);
}

