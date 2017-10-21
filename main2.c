#include<stdio.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<time.h>
#include<stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close


int main()
{
long res;
int fd = open("mytime", O_RDONLY);
char *buf = (char *)malloc(sizeof(char)*1000);
printf("%d\n",fd);
res = read(fd,buf, 1000);
printf("result is %lu, buf is: %s\n",res,buf);


}
