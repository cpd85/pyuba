This is going to be my attempt to beat python's internal list implementation... I'll start out with just integers then move onto other objects


I'm going to start with just a UBA, then probably try and make the buffer circular.


OK so for now, i'm compiling at command line with :

gcc -shared -I/usr/include/python2.7/ -lpython2.7 -o pyuba.so pyuba.c

basic sanity tests up
