#obj-m = cdtest.o
all:
	#make -C /usr/src/linux-headers-5.0.0-31-generic M=$(shell pwd) modules
	gcc -o dr devread.c
	gcc -o dw devwrite.c
clean:
	rm -f *.o *.ko *.mod.c *.order *.ko.cmd *.mod.o.cmd *dw *dr
	rm -f *dw *dr
