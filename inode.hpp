#ifndef INODE_HPP
#define INODE_HPP
#include <string>
#include "disk_op.hpp"

using namespace std;

class inode{
	public:
		inode();
		void initialize(string file_name, diskop *disk);
		string file_name;
		int file_size;
		int direct_ptrs[12];
		int indirect_ptrs;
		int dindirect_ptrs;
	
};	
	
#endif

