obj-m = chardev.o
all:
	make -C /usr/src/linux-headers-5.6.12 M=$(shell pwd) modules
	gcc -o dr devread.c
	gcc -o dw devwrite.c
clean:
	rm -f *.o *.ko *.mod.c *.order *.ko.cmd *.mod.o.cmd *dw *dr
	rm -f *dw *dr
