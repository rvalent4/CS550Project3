#include <linux/export.h>
#include <linux/time.h>
#include <asm/uaccess.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/errno.h>


asmlinkage int sys_my_xtime(struct timespec *current_time)
{

	
	struct timespec *current_time_k;
	long ret;
	current_time_k = (struct timespec*)kmalloc(sizeof(struct timespec), GFP_KERNEL);


	if(!current_time_k)
	{
		return -1;
	}


	*current_time_k = current_kernel_time();	

	ret = copy_to_user(current_time, current_time_k, sizeof(struct timespec));

	if(ret != 0)
	{	
		printk(KERN_ALERT "Error copying memory\n");
		kfree(current_time_k);
		return EFAULT;
	}
	else
	{
		printk(KERN_ALERT "my_xtime: %lu\n",current_time_k->tv_nsec);
		kfree(current_time_k);
		return 0;
	}
	
	kfree(current_time_k);
}

EXPORT_SYMBOL(sys_my_xtime);
