#include<stdio.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<time.h>
#include<stdlib.h>

int main()
{
	struct timespec *current_time = (struct timespec*)malloc(sizeof(struct timespec));
	struct timespec *garbage;
	int ret;
	ret = syscall(326, current_time);
	printf("syscall return value :%d\n",ret);
	if(ret == 0)
	{
		printf("%lu %lu \n", current_time->tv_sec, current_time->tv_nsec);
	
	}
	return 0;
}
