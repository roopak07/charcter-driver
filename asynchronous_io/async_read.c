#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
int len,fd,len1,fd1,flag;
void mysighandler(int sig)
{
 printf("%d signal is recived \n",sig);
}
void main()
{
char buf[100];
fd = open("/dev/mycdt",O_RDONLY);
printf("open fd = %d \n",fd);
printf("getpid = %d \n",getpid());
 if(fd<0)
 {
  printf("open ERROR... EXIT\n");
  exit(1);
 }
fcntl(fd,F_SETOWN,getpid()); // copies pid number in f_owner of present fd, When F_SETOWN is invoked, nothing happens, except that pid is assigned to filp->f_owner.
flag = fcntl(fd,F_GETFL); // gets the flags present in f_flag from present fd
fcntl(fd,F_SETFL,flag|FASYNC); // sets FASYNC flag in f_flag along with previous flags in present fd, once FASYNC is set in the flag ,fasync driver method gets invoked in driver code
printf("waiting for signal : \n");
 signal(SIGIO,mysighandler);
 pause();
 printf("waiting for data to be written : \n");
 /*** Read and display the contents ***/   
   len = read(fd,buf,100);
   printf("%d bytes read from device \n", len);
   buf[len-1] = 0;
   puts(buf);  
}

/*explanation of Asynchronous notification - https://www.oreilly.com/library/view/linux-device-drivers/0596000081/ch05s04.html */
