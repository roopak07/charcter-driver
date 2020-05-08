#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
MODULE_LICENSE("GPL"); // enable license ,without license kernal will assume this module as mallware
int abc_init(void)
{
printk(KERN_ALERT "ABC is in init state");
return 0;
}

void abc_exit(void)
{
printk(KERN_ALERT "ABC is in Exit State");
}
module_init(abc_init);
module_exit(abc_exit);
