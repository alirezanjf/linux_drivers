obj-m += dummydriver1.o
obj-m += dummydriver2.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) CC=x86_64-linux-gnu-gcc-12 modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

