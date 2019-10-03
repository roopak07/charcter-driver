obj-m = chardev.o
all:
	gcc devread.c -o devread
	gcc devwrite.c -o devwrite
	gcc devrwtest.c -o devrwtest
	make -C /usr/src/linux-headers-5.0.0-29-generic M=$(shell pwd) modules
clean:
	# rm -f devread devwrite devrwtest *.o *.ko *.mod.c
	rm -f *.o *.ko *.mod.c *.symvers *.order *.ko.cmd *.mod.o.cmd
