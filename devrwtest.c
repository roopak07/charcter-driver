#include<stdio.h>
#include<fcntl.h>
void main()
{
int fd = open("/dev/mychdriver",O_RDWR);
printf("open fd = %d \n",fd);
 if(fd<0)
 {
  printf("open ERROR... EXIT\n");
  exit(1);
 }
}
