#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/ioport.h>
#include<linux/cdev.h>
#include<linux/errno.h>
#include<linux/sched.h>
#include<uapi/linux/wait.h>
#include<linux/poll.h>

MODULE_AUTHOR("Teckbuz");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("charter driver");

DECLARE_WAIT_QUEUE_HEAD(wq);
#define maxsize 1024
struct cdev mycdev;
char kbuff[maxsize];
int nochar;
dev_t devno;
static struct class *cl; // Global variable for the device class

ssize_t mydev_read(struct file *file,char *udata,size_t size,loff_t *offset);
ssize_t mydev_write(struct file *file,const char *udata,size_t size,loff_t *offset);
int mydev_open(struct inode *inode,struct file *file);
int mydev_close(struct inode *inode,struct file *file);

static struct file_operations fops=
{
.owner 	=	THIS_MODULE,
.open		=	mydev_open,
.release	=	mydev_close,
.read		=	mydev_read,
.write		=	mydev_write,
};

int myinit(void)
{
int p,q;
p=alloc_chrdev_region(&devno,0,1,"mychardev"); 
if(p < 0)
 printk("register chardev region error \n");
else
 printk("register chardev region sucess \n");
if ((cl = class_create(THIS_MODULE, "mychardev")) == NULL)
  {
    unregister_chrdev_region(devno, 1);
    return -1;
  }
  if (device_create(cl, NULL, devno, NULL, "mychardev") == NULL)
  {
    class_destroy(cl);
    unregister_chrdev_region(devno, 1);
    return -1;
  }
cdev_init(&mycdev,&fops);
q=cdev_add(&mycdev,devno,1);
if(q < 0)
{
 printk("chardev region error \n");
 device_destroy(cl, devno);
 class_destroy(cl);
 unregister_chrdev_region(devno,10);
}
else
 printk("chardev region sucess \n");
nochar= 0;
return 0;
}

void myexit(void)
{
  device_destroy(cl, devno);
  class_destroy(cl);
 unregister_chrdev_region(devno,1);
 printk("unregistered chardev region \n");
}

int mydev_open(struct inode *inode,struct file *file)
{
 printk("in open funtion method \n");
 try_module_get(THIS_MODULE);
 if(file->f_mode && FMODE_READ)
	printk("file opened in read mode");
 else if(file->f_mode && FMODE_WRITE)
	printk("file opened in write mode");
return 0;
}

int mydev_close(struct inode *inode,struct file *file)
{
 printk("in close funtion method \n");
 module_put(THIS_MODULE);
 printk("device closed \n");
 return 0;
}

ssize_t mydev_read(struct file *file, char *udata, size_t size,loff_t *offset)
{
 printk("in read function method \n");
 wait_event(wq,nochar>0);
 if(nochar < size)
	size = nochar;
 copy_to_user(udata,kbuff,size);
 nochar = 0;
 return size;
}

ssize_t mydev_write(struct file *file,const char *udata,size_t size,loff_t *offset)
{
 printk("in write finction method \n");
 wait_event(wq,nochar<maxsize);
 if(size > maxsize)
	size = maxsize;
 copy_from_user(kbuff,udata,size);
 nochar = size;
 wake_up(&wq);
 return size;
}
module_init(myinit);
module_exit(myexit);
