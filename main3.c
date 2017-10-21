#include<stdio.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<time.h>
#include<stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <sys/time.h>
#define N 10
#define MAX_LENGTH 1000

int main()
{
	struct timeval gtodTimes[N];
	char *procClockTimes[N];
	int fd;

	for(int i = 0; i < N; i++)
	{
		procClockTimes[i] = (char *)malloc(sizeof(char)*1000);
		if(!procClockTimes[i])
		{	
			printf("Error allocating memory \n");
			return 0;
		}	
	}
	fd = open("/dev/mytime", O_RDONLY);
	if(fd < 0)
	{
		printf("Error opening character device \n");
	}

	for(int  i=0; i < N; i++)
	{

		int result = gettimeofday(&gtodTimes[i], 0);
		int bytes_read = read(fd, procClockTimes[i], MAX_LENGTH);
		
		if(result < 0 || bytes_read < 0)
		{
			printf("error getting times \n");
			return 0;
		}

	}	

	close(fd);

	for(int i=0; i < N; i++) 
	{
		printf("in user:%lu %lu    in kernel:%s \n", gtodTimes[i].tv_sec, gtodTimes[i].tv_usec, procClockTimes[i]);
	}

	for(int i = 0; i < N; i++)
	{
		free(procClockTimes[i]);
	}	

}
