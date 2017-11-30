#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h> 
#include <linux/miscdevice.h>
#include <linux/time.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/semaphore.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL"); 

//insmod numpipe.o N=10

int N = 0;
module_param(N, int, S_IRUGO);

static ssize_t temp_read(struct file *, char *, size_t, loff_t *);
static ssize_t temp_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations my_fops = {
		.owner = THIS_MODULE,
        .read = temp_read,
        .write = temp_write
};

struct miscdevice numpipe = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "numpipe",
    .fops = &my_fops,
};

int __init numpipe_init(void)
{
	int result = misc_register(&numpipe);
	if(result < 0)
	{
		printk(KERN_ALERT "numpipe: error registering misc device \n");
	}
	
	printk(KERN_ALERT "numpipe: initalizing numpipe module!\n");
	printk(KERN_ALERT "numpipe: recieved param N as :%i!\n", N);
	return 0; 
}

void __exit numpipe_exit(void)
{
	misc_deregister(&numpipe);
	printk(KERN_ALERT "numpipe: removing numpipe module!\n"); 
}


static ssize_t temp_read(struct file *f, char *buf, size_t q, loff_t * s)
{

		printk(KERN_ALERT "You called read\n");
		return 0;

}

static ssize_t temp_write(struct file *f, const char *buf, size_t q, loff_t * s)
{
		printk(KERN_ALERT "You called write\n");
		return 0;
}


module_init(numpipe_init); 
module_exit(numpipe_exit);
