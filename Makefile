all: driver 

driver: driver.cpp ryan.o
	g++ ryan.o driver.cpp -g -o test

ryan.o: ryan.cpp super_block.hpp ryan.hpp
	g++ -g -c ryan.cpp -o ryan.o

inode.o: inode.cpp inode.hpp
	g++ -g -c inode.cpp -o inode.o

clean:
	rm -f *.o
	rm -f *.txt
	rm -f *.bin
	rm -f test
