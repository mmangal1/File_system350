#include <iostream>
#include <string>
#include <stdlib.h>
#include "disk.hpp"

using namespace std;

int main(int args, char* argv[]){
	
	if(args != 4){
		fprintf(stderr, "invalid usage\n");
		exit(1);
	}else{
		if(sizeof(argv[3]) > 32){
			fprintf(stderr, "file name must be less than 32 characters\n");
			exit(1);
		}
		if(atoi(argv[1]) < 1024 || atoi(argv[1]) > 128000){
			fprintf(stderr, "num_blocks must be between 1024 and 128000\n");
			exit(1);
		}
		if(atoi(argv[2]) < 128 || atoi(argv[2]) > 512){
			fprintf(stderr, "block_size must be between 128 and 512\n");
			exit(1);
		}
	}
	create(argv[3], atoi(argv[1]), atoi(argv[2]));	
	
	return 0;	 	
}
