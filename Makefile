all: driver 

driver: driver.cpp disk_op.o
	g++ disk_op.o driver.cpp -g -o test

ryan.o: disk_op.cpp super_block.hpp disk_op.hpp
	g++ -g -c disk_op.cpp -o ryan.o

inode.o: inode.cpp inode.hpp
	g++ -g -c inode.cpp -o inode.o

clean:
	rm -f *.o
	rm -f *.txt
	rm -f *.bin
	rm -f test
