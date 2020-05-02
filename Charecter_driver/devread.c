#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
int len,fd;
void main()
{
char buf[100];
fd = open("/dev/mychdrive",O_RDONLY);
printf("open fd = %d \n",fd);
 if(fd<0)
 {
  printf("open ERROR... EXIT\n");
  exit(1);
 }
 /*** Read and display the contents ***/   
   len = read(fd,buf,100);
   printf("%d bytes read from device \n", len);
   buf[len-1] = 0;
   puts(buf);
   close(fd);
}
