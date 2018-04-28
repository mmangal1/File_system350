#ifndef DISK_OP_HPP
#define DISK_OP_HPP
#include "inode.hpp"
#include "super_block.hpp"

	class diskop{
		public:
			diskop();	
			diskop(char* filename, int buffer_size);
			void create(char* filename, int num_blocks, int block_size);
			int read_free_mem_iMap();
			int read_free_disk_iMap();
			void write_inode_map(int inode_map[], char* file_name, int block_size, int num_blocks, FILE *fp);
			int  write_fbl(int free_block_list[], char* file_name, int block_size, int num_blocks, FILE *fp);
			void write_inode_to_disk(int offset, FILE *fp, inode *node, int block_size);
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
			int get_offset();
			void update_inode_map(int index);
		private:
			FILE *fp;
			int buffer_len;
			int* buf_ptr;
			superblock sb;
			int inode_map[256];
			
	};
#endif
