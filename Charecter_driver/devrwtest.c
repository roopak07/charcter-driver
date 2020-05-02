#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
void main()
{
int fd = open("/dev/mychdrive",O_RDWR);
printf("open fd = %d \n",fd);
 if(fd<0)
 {
  printf("open ERROR... EXIT\n");
  exit(1);
 }
}
