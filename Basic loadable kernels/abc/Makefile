CONFIG_MODULE_SIG=n
obj-m=abc.o
all:
	make -C /usr/src/linux-headers-5.0.0-31-generic M=$(shell pwd) modules

clean:
	rm -f *.o *.ko *.mod.c
