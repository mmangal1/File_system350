#ifndef DISK_OP_HPP
#define DISK_OP_HPP
#include "super_block.hpp"
	class diskop{
		public:	
			diskop(char* filename, int buffer_size);
			void create(char* filename, int num_blocks, int block_size);
			int read_free_mem_iMap(char* fp){
			int read_free_disk_iMap(char* filename){
			void write_inode_map(int inode_map[], char* file_name, int block_size, int num_blocks, FILE *fp);
			int  write_fbl(int free_block_list[], char* file_name, int block_size, int num_blocks, FILE *fp);
			void write_sb(int offset, int block_size, int num_blocks, FILE *fp);
			void create(char* filename);
			void import(char* ssfs_filename, char* unix_filename);
			void cat(char* filename);
			void del(char* filename);
			void write(char *filename, char c, int startByte, int numByte);
			void read(char *filename, int startByte, int numByte);
			void list();
			void shutdown();
			int search(char* filename);
		private:
			FILE *fp;
			int buffer_len;
			int* buf_ptr;
			superblock sb;
			int imap[256];
			
	};
#endif
