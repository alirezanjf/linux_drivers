obj-m += hello.o

all:
	make CC=x86_64-linux-gnu-gcc-12 -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

