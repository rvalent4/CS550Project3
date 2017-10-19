#include<stdio.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<time.h>
#include<stdlib.h>

int main()
{
	struct timespec *current_time = (struct timespec*)malloc(sizeof(struct timespec));
	struct timespec *garbage = NULL;
	int ret;
	int ret2;

	if(current_time == NULL)
	{
		printf("error mallocing timespec");
	}
	ret = syscall(326, current_time);
	ret2 = syscall(326, garbage);
	printf("syscall return value :%d\n",ret);
	if(ret == 0)
	{
		printf("%lu %lu \n", current_time->tv_sec, current_time->tv_nsec);
	
	}
	if(ret2 != 0)
	{
		printf("return for unallocated memory returned %d\n", ret2);
	}
	free(current_time);
	return 0;
}
