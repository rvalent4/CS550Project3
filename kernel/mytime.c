#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h> 
#include <linux/miscdevice.h>
#include <linux/time.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL"); // called when module is installed

static  ssize_t my_read(char* buf);


static struct file_operations my_fops = {
	.owner = THIS_MODULE,
        .read = my_read
};


static struct miscdevice my_misc_device = { 
	.minor = MISC_DYNAMIC_MINOR, 
	.name = "my device",
	.fops = &my_fops
};

int __init mytime_init(void)
{
	int result = register_chrdev(70, "mytime", &my_fops);
	misc_register(&my_misc_device);
	printk(KERN_ALERT "mymodule: Hello World!\n");
	return 0; 
}
// called when module is removed 
void __exit mytime_exit(void)
{
	misc_deregister(&my_misc_device);
	printk(KERN_ALERT "mymodule: Goodbye, cruel world!!\n"); 
}

static ssize_t my_read(char* buf)
{
	long ret;
	char* kbuf = (char*)kmalloc(sizeof(char)*1000, GFP_KERNEL);
	struct timespec *current_time_k;
	current_time_k = (struct timespec*)kmalloc(sizeof(struct timespec), GFP_KERNEL);
	if(!current_time_k)
	{
		return -1;
	}


	*current_time_k = current_kernel_time();	
	sprintf(kbuf, "%lu %lu \0", current_time_k->tv_sec, current_time_k->tv_nsec);
	ret = copy_to_user(buf, kbuf, sizeof(kbuf));

	if(ret != 0)
	{	
		printk(KERN_ALERT "Error copying memory\n");
		kfree(current_time_k);
		kfree(kbuf);
		return EFAULT;
	}
	else
	{
		kfree(current_time_k);
		kfree(kbuf);
		return 0;
	}
	
	kfree(current_time_k);
	kfree(kbuf);	
	printk(KERN_ALERT "my_read() called\n");
	return 0;
}


module_init(mytime_init); 
module_exit(mytime_exit);
