# charcter-driver
simple charcter driver code
steps
1.insmod driver_file.ko
2.mknod /dev/device_file c 42 0 // 42 - major number ,0-minor number
3.execute the application i.e read or write
