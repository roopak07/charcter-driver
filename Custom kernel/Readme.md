1.Create a directory like "my_chardevf" inside drivers(which is in the Linux source code) for your driver and put your driver (my_chardev.c) file inside this directory. It will looks like /linux_source_code/drivers/my_chardevf/my_chardev.c

2.Create one Makefile inside your driver directory (using vi any editor) and inside this put obj-$(CONFIG_MY_CHARDEV) += my_chardev.o and save this file. This will appears like /linux_source_code/drivers/my_chardevf/Makefile

3.Create one Kconfig file inside your driver directory (using vi any editor) and inside this put

  menuconfig MY_CHARDEV
	tristate "my chardriver support" # this name will appear in make menu config
	default y
	help
	  If you want my_charecter driver support, you should say Y here and also to the
	  specific driver for your bus adapter(s) below.
	  This my_charecter driver can also be built as a module.  If so, the module
	  will be called my_chardev.
    
 Save this file, this will appears like /linux_source_code/drivers/my_chardevf/Kconfig

    in both Makefile and Kconfig file in the Linux source drivers Makefile and Kconfig file which are at /linux_source_code/drivers/Makefile and /linux_source_code/drivers/Kconfig

    In the Makefile add below in last line

     obj-y    += my_chardevf/ 

    or

     obj-$(CONFIG_MY_CHARDEV)   += my_chardevf/

    In Kconfig file add below in last line

    source "drivers/my_chardevf/Kconfig"

then compile code using make

    #defconfig in this add below line in the last

    #CONFIG_MY_DRIVER=y

Note:- Last step will differ according to your architecture, so that you have take care. Now you can compile your driver by using make command. (eg: sun7i_defconfig)
