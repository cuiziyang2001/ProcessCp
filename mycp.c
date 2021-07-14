#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

int main(int argc,char **argv)
{
	printf("start mycp...\n");
	if(argc < 3)
	{
		printf("参数有误:请输入正确参数\n");
		exit(1);
	}

	int sfd = open(argv[1],O_RDONLY);
	int dfd = open(argv[2],O_WRONLY|O_CREAT,0664);

	//总进程个数
	int process_num = atoi(argv[3]);

	//本子进程之前拷贝了几次
	int cpnum = atoi(argv[4]);

	//总文件大小
	int fd = open(argv[1],O_RDWR);
	int size;
	size = lseek(fd,0,SEEK_END);

	//本次拷贝大小
	int cpsize = size/process_num;

	//文件偏移
	lseek(sfd,cpnum*cpsize,SEEK_SET);
	lseek(dfd,cpnum*cpsize,SEEK_SET);

	char buffer[cpsize];
	bzero(buffer,sizeof(buffer));
	size_t rsize;
	rsize = read(sfd,buffer,sizeof(buffer));
	if(rsize == -1)
		printf("read error...\n");

	size_t wsize;
	wsize = write(dfd,buffer,strlen(buffer));
	if(wsize == -1)
		printf("write error...\n");

	close(sfd);
	close(dfd);

	return 0;
}
