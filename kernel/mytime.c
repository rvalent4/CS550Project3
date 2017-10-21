#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h> 
#include <linux/miscdevice.h>
#include <linux/time.h>
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


static ssize_t temp_read(struct file *f, char *buf, size_t q, loff_t * s)
{
	long ret;
	struct timespec current_time_k;
	char* kbuf = (char*)kmalloc(sizeof(char)*q, GFP_KERNEL);
	current_time_k = current_kernel_time();
        sprintf(kbuf, "%lu %lu ", current_time_k.tv_sec, current_time_k.tv_nsec);
        ret = copy_to_user(buf, kbuf, sizeof(kbuf));

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


return 29;
}

module_init(mytime_init); 
module_exit(mytime_exit);
