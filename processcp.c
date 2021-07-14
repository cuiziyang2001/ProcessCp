#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<string.h>

int main(int argc,char ** argv)
{
	//cp命令输入参数不足，强制退出
	if(argc < 3)
	{
		printf("参数有误，请输入正确参数\n");
		exit(-1);
	}

	//获取创建进程数量
	int process_num = 1;
	//创建进程默认为1
	//由于接下来exec传参为argv[3]，这里的process_num仅为判断进程个数是否符合要求，所以要默认下面argv[3]为1
	if(!argv[3])
		argv[3] = "1";
	else
	{
		process_num = atoi(argv[3]);
	}
	printf("process_num %d\n",process_num);

	//获取的进程数量不符合要求，强制退出
	if(process_num < 0 || process_num > 100){
		printf("输入进程数量不符合要求，请输入0-100进程数...\n");
		exit(1);
	}

	//创建规定数量进程
	pid_t pid;
	int i;
	char process_id[10];
	for(i=0;i<process_num;i++)
	{
		//创建第i+1个子进程
		pid = fork();
		sprintf(process_id,"%d",i);
		if(pid == 0)break;
	}


	if(pid > 0)
	{
		//父进程
		printf("Parent process create sucessfully...\n");
	}
	else if(pid == 0)
	{
		//子进程
		printf("come to exec...\n");
		execl("/home/colin/0603Linux/process/mycp","mycp",argv[1],argv[2],argv[3],process_id,NULL);
	}
	else
	{
		//进程创建失败
		perror("error...\n");
		exit(0);
	}


	//非阻塞回收
	pid_t wpid;
	int status;
	int ecode;
	while((wpid=waitpid(-1,&status,WNOHANG)) != -1)
	{
		if(wpid > 0)
		{
			if(WIFEXITED(status))
			{
				ecode = WEXITSTATUS(status);
				printf("parent process sucessfully pid %d exit code %d\n",wpid,ecode);
			}

			if(WIFSIGNALED(status))
			{
				ecode = WTERMSIG(status);
				printf("parent process sucessfully pid %d signal num %d\n",wpid,ecode);
			}
		}
		else
		{
			printf("try again...\n");
			usleep(100000);
		}
	}
	return 0;
}
