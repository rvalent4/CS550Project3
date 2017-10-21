#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h> 
#include <linux/miscdevice.h>
#include <linux/time.h>
#include <linux/string.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL"); // called when module is installed

static ssize_t temp_read(struct file *, char *, size_t, loff_t *);

static struct file_operations my_fops = {
	.owner = THIS_MODULE,
        .read = temp_read
};


static struct miscdevice my_misc_device = { 
	.minor = MISC_DYNAMIC_MINOR, 
	.name = "mytime",
	.fops = &my_fops
};

int __init mytime_init(void)
{
	int result = register_chrdev(70, "mytime", &my_fops);
	int result2 = misc_register(&my_misc_device);
	if(result < 0 || result2 < 0)
	{
		printk(KERN_ALERT "mytime: error registering device \n");
	}
	
	printk(KERN_ALERT "mytime: initalizing mytime module!\n");
	return 0; 
}
void __exit mytime_exit(void)
{
	misc_deregister(&my_misc_device);
	printk(KERN_ALERT "mytime: removing mytime module!\n"); 
}


static ssize_t temp_read(struct file *f, char *buf, size_t q, loff_t * s)
{
	long ret;
	struct timespec current_time_k;
	char* kbuf = (char*)kmalloc(sizeof(char)*q, GFP_KERNEL);
	
	if(!kbuf)
	{
		printk(KERN_ALERT "Error kmallocing buf\n");
		return -1;
	}

	current_time_k = current_kernel_time();
        if(sprintf(kbuf, "%lu %lu ", current_time_k.tv_sec, current_time_k.tv_nsec) < 0)
	{
		printk(KERN_ALERT "Error calling sprintf\n");
	}
	printk(KERN_ALERT "mytime.ko: %s   size is:%lu\n", kbuf, strlen(kbuf));
        ret = copy_to_user(buf, kbuf, strlen(kbuf));
	
        if(ret != 0)
        {
                printk(KERN_ALERT "Error copying memory\n");
                kfree(kbuf);
                return EFAULT;
        }
        else
        {
                kfree(kbuf);
                return 0;
        }	
}

module_init(mytime_init); 
module_exit(mytime_exit);
