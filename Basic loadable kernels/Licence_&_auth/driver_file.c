#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>

int myinit(void)
{
printk(KERN_ALERT"Init function");
return 0;
}

void myexit(void)
{
printk(KERN_ALERT"Exit function");
}

module_init(myinit);
module_exit(myexit);
MODULE_AUTHOR("Teckbuz");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sample Kernel Module");
MODULE_SUPPORTED_DEVICE("Linux");
