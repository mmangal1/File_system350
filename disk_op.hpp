#ifndef DISK_OP_HPP
#define DISK_OP_HPP

	class diskop{
		public:	
			diskop(char* filename, int buffer_size);
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
			superblock sb;
			int* imap;
			int* dmap;
			
	}
#endif
