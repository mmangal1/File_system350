#include <iostream>
#include <string>
#include <cstring>
#include "inode.hpp"
#include <sys/stat.h>


void initalize(inode *node, char* name){
	
	struct stat st;

	strcpy(node->file_name, name);
	
	if(stat(name, &st) != 0){
		fprintf(stderr, "file_size is 0");
		exit(1);
	}
	node->file_size = st.st_size;
	
	for(int x = 0; x < 12; x++){
		node->direct_ptrs[x] = -1;
	}	
	node->indirect_ptrs = -1;
	node->dindirect_ptrs = -1;

}
