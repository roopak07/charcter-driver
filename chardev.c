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

DECLARE_WAIT_QUEUE_HEAD(wq);
//wait_queue_head_t wq;
//init_waitqueue_head(&wq);
/*Wait queue head can be initialized in the following ways
Statically using :
DECLARE_WAIT_QUEUE_HEAD(name);

Dynamically using :
wait_queue_head_t my_queue;
init_waitqueue_head(name);*/

typedef struct //insted we can write "struct mydev_t" and can define mydev as "struct mydev_t mydev"
{
int nchar;
char buf[1024];
struct cdev mycdev; //mycdev is of type struct cdev
}mydev_t;

mydev_t mydev; // mydev is a variable of type struct mydev_t

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
int ret=0,res=0;
printk(KERN_ALERT "ENTERED IN TO MYDEV_INIT STATE");
dev_t devno = 0; // devno is of type dev_t
devno=MKDEV(42,0);
ret=register_chrdev_region(devno,10,"mychardev");
printk("ret=%d\n",ret);
if(ret<0)
{
 printk(KERN_ALERT "Register char dev region Error %d\n",ret);
return 1; // driver gets off loaded form kernal space of RAM
}
else
 printk("Register char dev region success %d\n",ret);

cdev_init(&mydev.mycdev,&mycdev_ops); //mycdev.fops = &mycdev_ops
mydev.mycdev.owner=THIS_MODULE;
res = cdev_add(&mydev.mycdev,devno,1);// registering cdev object with kernel

if(res) // if res = non zero value then its a fail ,so dergister cdev object
{
unregister_chrdev_region(devno,10);
return 1;
}
else
{
printk("cdev added sucessfully :%d \n",res);
}
mydev.nchar = 0;
return 0;
}
/***************************** exit function ***********************************/
void mydev_exit(void)
{
dev_t devno=0;
printk(KERN_ALERT "Module unregistered \n");
unregister_chrdev_region(devno,10);
}
/***************************** mydev_open ***********************************/
int mydev_open(struct inode *inode,struct file *file ) // inode is pointer variable of type struct indoe , file is a pointer variable of type struct file , since mydev_open need to return 0 value to sys_open then sys_open will return fd to "open()" function in user space 
{
try_module_get(THIS_MODULE);
file->private_data=&mydev; // copying address of mydev in to file object private_data
// checking weather the application is read mode or write mode , FMODE_WRITE is predefined macro of kernel
if(file->f_mode && FMODE_WRITE ) 
	printk("<i>" "file opened in write mode");
if(file->f_mode && FMODE_READ ) 
	printk("<i>" "file opened in read mode");
return 0;
}
/***************************** mydev_read ***********************************/

ssize_t mydev_read(struct file *file, char *data, size_t size, loff_t *offset)
{
mydev_t *t_dev;
t_dev = file->private_data;
wait_event(wq,t_dev->nchar>0);
if(t_dev->nchar < size) // if in write function we are wrote 5 charcters and in read application is requesting for 20 chacrecters
 {
  size = t_dev-> nchar; // size will be no:of charcters in write function 
 }
  copy_to_user(data,t_dev->buf,size);
  t_dev->nchar = 0;// once we copy data to user space then kernal buffer will be empty so weneedto make nchar = 0
  printk("<i>" "read called \n");
  return size;
}

/***************************** mydev_write ***********************************/
ssize_t mydev_write(struct file *file, const char *data, size_t size, loff_t *offset)// char data will store address of buf which is defined in user space,size will store lenth of the string in user sapce
{
int ret;
mydev_t *t_dev; //t_dev is a pointer of type struct mydev_t, we can also write mydev *t_dev
t_dev = file->private_data;
if(size > 1024) // since our buffer size is 1024 ,if the incoming datais more than 1024 then we need to limit it to 1024 byte
{
size = 1024;
}
ret = copy_from_user(t_dev->buf, data, size ); // t_dev->buf is destination buffer base address ,data contains base address of source buffer, size will contain lenght of the string
printk("no:of bytes not copied: %d \n",ret);
t_dev->nchar=size;
wake_up(&wq); // this will unblock wait_event in mydev_read
printk("<i>" "write called \n");
return size;
}

/***************************** mydev_close ***********************************/
int mydev_close(struct inode *inode, struct file *file)
{
module_put(THIS_MODULE);
printk("<i>" "device closed \n");
return 0;
}
/***************************** initializing and exit ***********************************/
module_init(mydev_init);
module_exit(mydev_exit);

