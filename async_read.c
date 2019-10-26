#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
void mysighandler(int sig)
{
 printf("signal handler is called with signal-no: %d \n",sig);
}
void main()
{
 int len,fd,flg;
 char buf[100];
 fd=open("/dev/sy",O_RDONLY);
 printf("open fd = %d \n",fd);
 if(fd<0)
 {
  printf("file ERROR...EXIT \n");
  exit(1);
 }
 else
	printf("file open sucess... fd:%d \n",fd);
 printf("pid of the process:%d \n",getpid());

 fcntl(fd,F_SETOWN,getpid());
 flg = fcntl(fd,F_GETFL);
 fcntl(fd,F_SETFL, flg|FASYNC);
 signal(SIGIO,mysighandler);
 pause();
 len = read(fd,buf,50);
 printf("no:of bytes read is : %d \n",len);
 puts(buf);
 close(fd);
}
