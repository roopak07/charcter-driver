#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/errno.h>
#include<linux/poll.h>
#include<linux/cdev.h>
#include<linux/ioport.h>
#include<linux/sched.h>
#include<uapi/linux/wait.h>

#define major_no 43
#define minor_no 0
#define nodev 10

MODULE_AUTHOR("ROOPAK");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Asynchronous - IO charcter driver test");
ssize_t sy_read(struct file *file,char *buff,size_t size,loff_t *offset);
ssize_t sy_write(struct file *file,const char *buff,size_t data, loff_t *offset);
int sy_open(struct inode *inode,struct file *file);
int sy_close(struct inode *inode,struct file *file);
static int sy_fasync(int fd,struct file *file,int mode);

struct cdev mycdev;
char buf[1024];
int nochar;

DECLARE_WAIT_QUEUE_HEAD(wq);
struct fasync_struct *asq;

static struct file_operations mycdev_ops = 
{
 .owner		= THIS_MODULE,
 .open 		= sy_open,
 .release 	= sy_close, // close(fd) will call .release function
 .read 		= sy_read,
 .write 	= sy_write,
 .fasync 	= sy_fasync,
};
/************************* init function **************************/
int init_async(void)
{
 int creg,cdr;
 dev_t devno=0;
 printk("Init function called \n");
 devno = MKDEV(major_no,minor_no);
 creg = register_chrdev_region(devno,nodev,"async_driver");
 printk("charecter driver region creg: %d \n",creg);
 if(creg == 0)
	printk("Charcter driver registered with kernal");
 else
 {
  printk("ERROR..!: Charcter driver not registered with kernal");
  return creg;
 }
 cdev_init(&mycdev,&mycdev_ops);
 //mycdev.owner=THIS_MODULE;
 cdr = cdev_add(&mycdev,devno,1);
 printk("cdev region: %d \n",cdr);
 if(cdr == 0)
 	printk("Cdev region sucessfully linked to device \n");
 else
 {
 	printk("ERROR..!: Cdev region not linked to device \n");
 	unregister_chrdev_region(devno,nodev);
	return 1;
 }
 nochar = 0;
 return 0;
}
/************************* Exit function **************************/
void exit_async(void)
{
 printk("exit function called \n");
 dev_t devno=0;
 devno = MKDEV(major_no,minor_no);
 unregister_chrdev_region(devno,nodev);
 printk("unregistered charcter driver region \n");
}
/************************* open function **************************/
int sy_open(struct inode *inode,struct file *file)
{
 printk("open function called \n");
 try_module_get(THIS_MODULE);
 if(file->f_mode && FMODE_READ)
	printk("file is opened in read mode \n");
 if(file->f_mode && FMODE_WRITE)
	printk("file is opened in write mode \n");
return 0;
}
/************************* close function **************************/
int sy_close(struct inode *inode,struct file *file)
{
 printk("close function called \n");
 module_put(THIS_MODULE);
 printk("device closed \n");
 return 0;
}
/************************* fasync function **************************/
static int sy_fasync(int fd,struct file *file,int mode)
{
 int fv;
 printk("async function called \n");
 fv=fasync_helper(fd,file,mode,&asq);
 printk("async value:%d \n",fv);
 return fv;
}
/************************* read function **************************/
ssize_t sy_read(struct file *file,char *buff,size_t size,loff_t *offset)
{
 int nnc;
 printk("read function called \n");
 wait_event(wq,nochar>0);
 if(nochar<size)
	size=nochar;
 nnc=copy_to_user(buff,buf,size);
 if(nnc  == 0)
	printk("read sucessful \n");
 else
	printk("read error...! no:of bytes not copied:%d \n",nnc);
 nochar = 0;
 return size;
  
}
/************************* write function **************************/
ssize_t sy_write(struct file *file,const char *buff,size_t size, loff_t *offset)
{
 int nnc;
 printk("write function called \n");
 wait_event(wq,nochar<1024);
 printk("read size request :%d \n",size);
 if(size>1024)
	size = 1024;
 nnc=copy_from_user(buf,buff,size);
 
 if(nnc  == 0)
	printk("write sucessful \n");
 else
	printk("write error...! no:of bytes not copied:%d \n",nnc);
 
 nochar = size;
 wake_up(&wq); 
 
 if(asq)
	kill_fasync(&asq,SIGIO,POLL_IN);
 else
	printk("no applications are intrested in asynchronous IO \n");
 return size;
}

module_init(init_async);
module_exit(exit_async);


