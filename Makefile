all: driver

driver: driver.cpp disk.o
	g++ disk.o driver.cpp -o test

disk.o: disk.cpp disk.hpp
	g++ -c disk.cpp -o disk.o

inode.o: inode.cpp inode.hpp
	g++ -c inode.cpp -o inode.o
	
