obj-m=x.o
all:
	make -C /usr/src/linux-headers-5.0.0-27-generic M=$(shell pwd) modules
clean:
	rm -f *.o *.ko *.mod.c
