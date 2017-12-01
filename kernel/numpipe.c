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
char* kbuf;
int start = 0;
int end = 0;
int size = 0;


static DEFINE_SEMAPHORE(full);
static DEFINE_SEMAPHORE(empty);
static DEFINE_SEMAPHORE(mutex);

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
	kbuf = (char*)kmalloc(sizeof(int)*N, GFP_KERNEL);
	
	if(!kbuf)
	{
		printk(KERN_ALERT "Error kmallocing buf\n");
		return -1;
	}



	sema_init(&full, 0);
	sema_init(&empty, N);
	sema_init(&mutex,1);

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
        kfree(kbuf);
	misc_deregister(&numpipe);
	printk(KERN_ALERT "numpipe: removing numpipe module!\n"); 
}


static ssize_t temp_read(struct file *f, char *buf, size_t q, loff_t * s)
{
		int ret;
		if(q != sizeof(int))
			return -2;

		if(down_interruptible(&full))
			return -1;
		if(down_interruptible(&mutex))
		{
			up(&full);
			return -1;
		}

		if(size != 0)
		{
			ret = copy_to_user(buf, &kbuf[start%N], sizeof(int));
			if(ret != 0)
			{
				printk(KERN_ALERT "error copying memory\n");
				up(&mutex);
				up(&full);
				return EFAULT;
			}		
			start = start + 1;
			size = size -1;
		}
		up(&mutex);
		up(&empty);
		return 0;

}

static ssize_t temp_write(struct file *f, const char *buf, size_t q, loff_t * s)
{
		int ret;
		if(q != sizeof(int))
			return -2;
		if(down_interruptible(&empty))
			return -1;
		if(down_interruptible(&mutex))
		{
			up(&empty);
			return -1;
		}
		if(size != N)
		{
			ret = copy_from_user(&kbuf[end%N],buf, sizeof(int));
			if(ret !=0)
			{
				printk(KERN_ALERT "error copying memory\n");
				up(&mutex);
				up(&empty);
				return EFAULT;
			}
			printk(KERN_ALERT "Kernel recieved number %d\n", kbuf[end%N]); 
			end = end+1;
			size = size + 1;
		}
		up(&mutex);
		up(&full);
		return 0;

}


module_init(numpipe_init); 
module_exit(numpipe_exit);
