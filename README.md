# Kernel log
tail -l /var/log/syslog // for kernel logs

# charcter-driver
simple charcter driver code
steps
1.insmod driver_file.ko
2.mknod /dev/device_file c 42 0 // 42 - major number ,0-minor number
3.execute the application i.e read or write

mknod -m permissions name type major minor 
where: name is the full pathname of the device to create, such as /dev/rft0. 
type is either c for a character device or b for a block device. 
major is the major number of the device. 
minor is the minor number of the device. 
-m permissions is an optional argument that sets the permission bits of the new device file to permissions docstore.mik.ua/orelly/linux/run/ch06_03.htm

Ex: mknod -m 666 /dev/bogus b 42 0 docstore.mik.ua/orelly/linux/run/ch06_03.htm
