obj-m = chardevt.o
all:
	make -C /usr/src/linux-headers-5.0.0-29-generic M=$(shell pwd) modules
clean:
	# rm -f devread devwrite devrwtest *.o *.ko *.mod.c
	rm -f *.o *.ko *.mod.c
