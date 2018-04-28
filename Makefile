all: driver

driver: driver.cpp disk.o
	g++ disk.o driver.cpp -g -o test

disk.o: disk.cpp disk.hpp
	g++ -g -c disk.cpp -o disk.o

inode.o: inode.cpp inode.hpp
	g++ -g -c inode.cpp -o inode.o

clean:
	rm -f *.o
	rm -f *.txt
	rm -f *.bin
	rm -f test
