#ifndef INODE_HPP
#define INODE_HPP

	typedef struct inode{
		char file_name[32];
		int file_size;
		int direct_ptrs[12];
		int indirect_ptrs;
		int dindirect_ptrs;
	} inode;
	
	void initialize(inode *node, char* name);
	
	
#endif

