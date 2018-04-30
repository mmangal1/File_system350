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
	cout << "num_indirect = " << num_indirect << endl;
	int count = 0; // determine if we need to move to the next indirect block
	int indirect_block;
	int next = 0;
	for(int j = 0; j < num_blocks; j++){
		if(j < 12){
			this->direct_ptrs[j] = freeBlocks.at(j);
		}else if(j >= 12 && j < num_indirect + 13){
			// write the next free block to the indirect block
			if(j == 12){
				this->indirect_ptrs = freeBlocks.at(j);
				// will move to the indirect block to begin writing
				fseek(disk -> fp, (this->indirect_ptrs * disk -> sb.block_size), SEEK_SET);
			}else{
	//TODO			//write free blocks into the ptr's block
				// pass to the scheduler thread to write to the disk file
				fwrite(&freeBlocks.at(j), sizeof(int), 1, disk -> fp);
			}
			
		}else{
			if(j == num_indirect + 13){
				cout << "we got here" << endl;
				this->dindirect_ptrs = freeBlocks.at(j);
				// will move to the indirect block to begin writing
				fseek(disk -> fp, (this->dindirect_ptrs * disk -> sb.block_size), SEEK_SET);
			}else{
		//TODO		//write free blocks into the ptr's block
				// pass to the scheduler thread to write to the disk file
				//have to move to next indirect block
				if(count == 0/*count % num_indirect == 0*/){
				//cout << " in here???" << endl;
					fseek(disk -> fp, (this->dindirect_ptrs * disk -> sb.block_size) + (disk -> sb.block_size * next), SEEK_SET);
					fwrite(&freeBlocks.at(j), sizeof(int), 1, disk -> fp);
					indirect_block = freeBlocks.at(j);
					fseek(disk -> fp, (freeBlocks.at(j) * disk -> sb.block_size), SEEK_SET);
					next++;
				}else{
					//cout << freeBlocks.at(j) << "double indirect " << endl;
					fwrite(&freeBlocks.at(j), sizeof(int), 1, disk -> fp);
					//count++;
				}
				count++;
				if(count ==num_indirect+1) count = 0;

			}	
			
		}
	}
	cout << "this is the value I want " << freeBlocks.at(num_indirect + 13) << endl;
	cout << "this is the value I want " << freeBlocks.at(num_indirect + 14) << endl;
	cout << "this is the value I want " << freeBlocks.at(num_indirect + 15) << endl;
	cout << "this is the value I want next " << freeBlocks.at(num_indirect + 16) << endl;

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





//TODO
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

	cout << endl << "values in the indirect block" << endl;
	fseek(fp, (node1 -> indirect_ptrs * sb.block_size), SEEK_SET);
	int indArr[sb.block_size/4];
	for(int i = 0; i < (sb.block_size/4); i++){
		indArr[i] = 0;
	}
	fread(&(indArr), sizeof(int), (sb.block_size/4), fp);
	for(int i = 0; i < (sb.block_size/4); i++){
		cout << indArr[i] << endl;
	}


	cout << endl << "values in the double indirect block" << endl;
	fseek(fp, (node1 -> dindirect_ptrs * sb.block_size), SEEK_SET);
	int iblock = 0;
	fread(&(iblock), sizeof(int), 1, fp);
	cout << "the value in first di " << iblock << endl;
	fseek(fp, (iblock * sb.block_size), SEEK_SET);
	for(int i = 0; i < (sb.block_size/4); i++){
		indArr[i] = 0;
	}
	fread(&(indArr), sizeof(int), (sb.block_size/4), fp);
	for(int i = 0; i < (sb.block_size/4); i++){
		cout << indArr[i] << endl;
	}

}
























