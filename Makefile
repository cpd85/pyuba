all:
	gcc -shared -I/usr/include/python2.7/ -lpython2.7 -o pyuba.so pyuba.c
   
clean:
	rm -rf pyuba.so