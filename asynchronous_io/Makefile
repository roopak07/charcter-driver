obj-m = cd_async.o
all:
	make -C /usr/src/linux-headers-5.0.0-31-generic M=$(shell pwd) modules
	gcc -o asyncr async_read.c
	gcc -o asyncw devwrite.c
clean:
	rm -f *.o *.ko *.mod.c *.order *.ko.cmd *.mod.o.cmd
	rm -f *asyncw *asyncr
