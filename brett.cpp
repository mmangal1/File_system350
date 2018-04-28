//populate fields of x
inode x;
//check to see if the file already exists on the disk
int on_disk = 0;
int index = search(filename);
//file is on disk
if(index != -1){
	on_disk = 1;
	index = search
}else{
	//if file is not on disk make sure there is a free inode for it
	index = read_free_iMap;
}
x.filename = file_name;
//determine file size
ifstream file( filename, ios::binary | ios::ate);
int file_size = file.tellg();

//determine number of blocks needed based on block size
int num_blocks = file_size / block_size;
x.file_size = file_size;

//an array to hold free blocks we will be giving to the inode
vector<int> freeBlocks = read_fbl;
if(freeBlocks.size() < num_blocks){
	//do not conduct the write
	// do not have enough room
	//implement how to exit
}

//number if ints an indirect block can hold
int num_indirect = block_size / 4;

for(int j = 0; j < num_blocks; j++){
	if(j < 12){
		direct_ptrs[j] = freeBlocks[j];
	}else if(j >= 12 && j < num_indirect + 12){
		// write the next free block to the indirect block
	}else{
		//write into the double i block
	}
}



//read_fbl
vector<int> read_fbl(){
	vector<int> ret_val;
	for(int i = 0; i < num_blocks - 259; i++){//make the loop 'free_block_count' times
		if(free_block_list[i] == 0){
			ret_val.push_back(i+/*offset*/);
		}
	}
return ret_val;
}




































