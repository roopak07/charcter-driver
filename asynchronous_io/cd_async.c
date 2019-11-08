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

MODULE_AUTHOR("ROOPAK");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Asynchronous - IO charcter driver test");
ssize_t mycdt_read(struct file *file, char *data, size_t size, loff_t *offset);
ssize_t mycdt_write(struct file *file, const char *data, size_t size, loff_t *offset);
int mycdt_open(struct inode *inode,struct file *file);
int mycdt_close(struct inode *inode, struct file *file);
static int mycdt_fasync(int fd,struct file *file,int mode);

#define majorno 43
#define minorno 0

struct fasync_struct *async_q;
DECLEARE_WAIT_QUEUE_HEAD(wq);
struct mydev_t
{
 int nc;
 char buf[1024];
 struct cdev mycdevt;
};
struct mydev_t mycdt;

static struct file_operations fops =
{
.open		=	mycdt_open,
.release	=	mycdt_close,
.read		=	mycdt_read,
.write		=	mycdt_write,
.fasync		=	mycdt_fasync,
};
/******* init function ********/
int mycdt_init(void)
{
 int res,ret;
 dev_t devno=0;
 devno = MKDEV(majorno,minorno);
 res = register_chrdev_region(devno,10,"cdtest");
 printk("res: %d \n",res);
 if(res < 0)
 {
  printk("charecter driver region error :%d \n",res);
  return res;
 }
else
printk("charecter driver region sucess:%d \n",res);
cdev_init(&mycdt.mycdevt,&fops);
mycdt.mycdevt.owner = THIS_MODULE;
ret = cdev_add(&mycdt.mycdevt,devno,1);
if(ret)
{
 unregister_chrdev_region(devno,10);
 return 1;
}
else
{
 printk("cdev added sucessfully : %d \n",ret);
}
mycdt.nc = 0;
return 0;
}
/******* exit function ********/
void mycdt_exit(void)
{
 dev_t devno;
 devno = MKDEV(majorno,minorno);
 printk("unregistering chardev region \n");
 unregister_chrdev_region(devno,10);
}
/******* mycdt_open function ********/
int mycdt_open(struct inode *inode,struct file *file)
{
 printk("In Open function \n");
 try_module_get(THIS_MODULE);
 file->private_data = &mycdt;
 if(file->f_mode && FMODE_WRITE)
	printk("file opened in write mode\n");
if(file->f_mode && FMODE_READ)
	printk("file opened in read mode \n");
return 0;
}
/******* mycdt_close function ********/
int mycdt_close(struct inode *inode, struct file *file)
{
 printk("In close function \n");
 module_put(THIS_MODULE);
 printk("device closed \n");
 return 0;
}
/******* mycdt_read function ********/
ssize_t mycdt_read(struct file *file, char *data, size_t size, loff_t *offset)
{
 int red;
 struct mydev_t *devt_t;
 devt_t = file->private_data;
 printk("In read function \n"); 
 wait_event(wq,devt_t->nc>0);
 if(devt_t->nc < size)
	size = devt_t->nc;
 red = copy_to_user(data,devt_t->buf,size);
 printk("no:of bytes not copied from kernal space to user space is :%d",red);
 devt_t->nc = 0;
 return size;
}
/******* mycdt_write function ********/
ssize_t mycdt_write(struct file *file, const char *data, size_t size, loff_t *offset)
{
int ws;
struct mydev_t *devt_t;
devt_t = file->private_data;
printk("In Write function \n");
wait_event(wq,devt_t->nc<1024); // if the buffer is full then it will block (i.e more than 1024 data(condition will be false)), if buffer has some space (i.e less than 1024 data is present in buffer (condition will  be true))
 if(size > 1024)
	size = 1024;
ws = copy_from_user(devt_t->buf, data, size );
printk("no of bytes not copied from user space to kernal space is: %d \n",ws);
devt_t -> nc = size;
wake_up(&wq);
/*check in the application list that if non of the applications are intrested in asynchronous IO then async_q will NULL, if fasync_q some adress the call kill_fasync ,if fasync_q is NULL then dont call kill_fasync since no application will be intrested in fasync */
 if(async_q) // 
 {
  kill_fasync(&async_q,SIGIO,POLL_IN); //sending signals to multiple applications from kernal, kill_fasync is used to signal the interested processes when data arrives
 }
 else
 {
  printk("non of the application is intrested in asyncronous IO since async is :%d",async_q);
 }
return size;
}

/******* mycdt_fasync function ********/
static int mycdt_fasync(int fd,struct file *file,int mode)
{
 printk("In fasync function \n");
 fasync_helper(fd,file,mode,&async_q);
}
module_init(mycdt_init);
module_exit(mycdt_exit);

