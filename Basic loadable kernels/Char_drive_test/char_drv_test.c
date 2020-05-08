#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include <linux/fs.h> // to include file_operations 
#include <linux/poll.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <uapi/linux/wait.h>

MODULE_AUTHOR("Roopak");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sample charcter driver");
ssize_t mydev_read(struct file *file, char *data, size_t size, loff_t *offset);
ssize_t mydev_write(struct file *file, const char *data, size_t size, loff_t *offset);
int mydev_open(struct inode *inode,struct file *file);
int mydev_close(struct inode *inode, struct file *file);

static struct file_operations mycdev_ops =
{
.open	=	mydev_open,
.release=	mydev_close,
.write	=	mydev_write,
.read	=	mydev_read,
};

/***************************** mydev_init ***********************************/
int mydev_init(void)
{
int res=0;
printk(KERN_ALERT "ENTERED IN TO MYDEV_INIT STATE");
dev_t devno = 0; // devno is of type dev_t
devno=MKDEV(43,0);
res=register_chrdev_region(devno,10,"mychardev");
printk("res=%d\n",res);

  if(res<0)
  {
    printk(KERN_ALERT "Register char dev region Error %d\n",res);
    return res;
  }
  else
    printk(KERN_ALERT "Register char dev region success %d\n",res);
return 0;
}

/***************************** mydev_open ***********************************/
int mydev_open(struct inode *inode,struct file *file ) // inode is pointer variable of type struct indoe , file is a pointer variable of type struct file , since mydev_open need to return 0 value to sys_open then sys_open will return fd to "open()" function in user space 
{
printk(KERN_ALERT "In open function \n");
return 0;
}
/***************************** mydev_read ***********************************/

ssize_t mydev_read(struct file *file, char *data, size_t size, loff_t *offset)
{
printk(KERN_ALERT "In read function \n");
return 0;
}

/***************************** mydev_write ***********************************/
ssize_t mydev_write(struct file *file, const char *data, size_t size, loff_t *offset)// char data will store address of buf which is defined in user space,size will store lenth of the string in user sapce
{
printk(KERN_ALERT "In write function \n");
return size;
}

/***************************** mydev_close ***********************************/
int mydev_close(struct inode *inode, struct file *file)
{
printk(KERN_ALERT "In close function \n");
return 0;
}

void mydev_exit(void)
{
printk(KERN_ALERT"in exit function");
dev_t devno = MKDEV(43, 0);
unregister_chrdev_region(devno, 10);
}
module_init(mydev_init);
module_exit(mydev_exit);
