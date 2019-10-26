#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
void main()
{
char buf[100];
int fd = open("/dev/sy",O_WRONLY);
printf("open fd = %d \n",fd);
 if(fd<0)
 {
  printf("open ERROR... EXIT\n");
  exit(1);
 }
printf("Enter data to write: \n");
scanf("%s",buf);
write(fd,buf,strlen(buf));
}
