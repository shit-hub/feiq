/*************************************************************************
 * @Filename              :  client_fun.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-10 Fri
 ************************************************************************/



#include "client_fun.h"

#include <sys/uio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <limits.h>

/**
 * [显示函数]
 * @param buf [要显示的内容]
 */
void Display(char * buf)
{
	char * spacebuf = malloc(LINE_SIZE);

	//显示buf内容
	bzero(spacebuf,LINE_SIZE);
	if(buf != NULL)
	{
		memset(spacebuf,' ',LINE_SIZE-(strlen(buf)));
		printf("\r%s%s\n",spacebuf,buf);
	}
	//界面显示
	memset(spacebuf,'-',LINE_SIZE);
	printf("\n%s\n",spacebuf);
	printf("cmd > ");
	fflush(stdout);

	free(spacebuf);
}

/**
 * [进度条显示函数]
 * @param size        [当前大小]
 * @param acount_size [总大小
 */
static void Progress_Bar(unsigned long size, unsigned long acount_size)
{
	//显示百分比
	printf("\r[progress : %3ld%%] ",size * 100 / acount_size);

	//显示进度条
	putchar('|');
	for (size_t i = 0; i < 30; i++)
	{
		if(i < size * 30 / acount_size)
			putchar('#');
		else
			putchar('-');
	}
	putchar('|');
	fflush(stdout);
}

/**
 * [检查是否退出]
 * @param buf       [指令内容]
 * @param socket_fd [套阶字文件描述符]
 */
void Turn_Off(char * buf,int socket_fd)
{
	if(strncmp(buf,"exit",4) == 0)
	{
		printf("\rReady to exit\n");
		sleep(1);
		exit(0);
	}
}

/**
 * [发送文件]
 * @param  buf       [指令内容]
 * @param  socket_fd [socket的文件描述符]
 * @return           [description]
 */
bool Send_File(char * buf,int socket_fd)
{
	FILE *fptr;

	int tmp = 0;
	char *base_name = calloc(1,256);
	char *msg = calloc(1,256);

	unsigned long size = 0;
	unsigned long acount_size = 0;

	if(strncmp(buf,"send",4) == 0)
	{
		realpath(buf+5,msg);
		strcpy(base_name,"send ");
		strcat(base_name,basename(msg));
		send(socket_fd, (const void *)base_name, strlen(base_name), 0);
		printf("\rSend file : %s\n",buf + 5);
		fptr = fopen(buf + 5,"r");

		fseek(fptr,0,SEEK_END);
		acount_size = ftell(fptr);
		fseek(fptr,0,SEEK_SET);
		if(fptr == NULL)
		{
			printf("Path error\n");
			return false;
		}
		else
		{
			send(socket_fd, &acount_size, sizeof(int), 0);
			recv(socket_fd,(void *)&tmp, sizeof(int), 0);
			size = tmp;

			Progress_Bar(size, acount_size);
			while(size < acount_size)
			{
				sendfile(socket_fd, fileno(fptr), 1024, (off_t *)&size, NULL, 0);
				Progress_Bar(size, acount_size);
			}
		}
		printf("\n%ld bytes had been sent!\n",size-tmp);
		printf("Done!\n");
		Display(NULL);
		fclose(fptr);
		free(base_name);
		return true;
	}
	free(base_name);
	return false;
}

/**
 * [接受文件]
 * @param  buf       [指令内容]
 * @param  socket_fd [套接字文件描述符]
 * @return           [description]
 */
bool Get_File(char * buf,int socket_fd)
{
	FILE *fptr;

	int tmp = 0;
	char * msg = calloc(1,1024);
	char * base_name = calloc(1,1024);

	unsigned long size = 0;
	unsigned long acount_size = 0;

	int recv_size = 0;

	if(strncmp(buf,"get",3) == 0)
	{
		send(socket_fd, (const void *)buf, strlen(buf), 0);
		recv(socket_fd, &acount_size, sizeof(int), 0);

		if(strncmp(buf+4,"../",3) == 0)
		{
			printf("Permission denied\n");
			return false;
		}

		if(acount_size == 0)
		{
			printf("Permission denied\n");
			return false;
		}

		strcpy(base_name,"./file_recv/");
		strcat(base_name,basename(buf + 4));

		printf("\rGet file : %s\n",buf + 4);
		fptr = fopen(base_name,"a");
		if(fptr == NULL)
		{
			printf("Can not create file\n");
			return false;
		}
		else
		{
			tmp = ftell(fptr);
			send(socket_fd, &tmp, sizeof(int), 0);
			size = tmp;
			Progress_Bar(size, acount_size);
			while(size < acount_size)
			{
				bzero(msg,1024);

				recv_size = read(socket_fd, msg, 1024);
				write(fileno(fptr),msg,recv_size);

				size += recv_size;

				Progress_Bar(size, acount_size);
			}
		}
		printf("\nGot %ld bytes!\n",size-tmp);
		printf("Done\n");
		Display(NULL);
		fclose(fptr);
		free(msg);
		free(base_name);
		return true;
	}
	free(msg);
	free(base_name);
	return false;
}

/**
 * [查看是否为命令]
 * @param  buf       [指令内容]
 * @param  socket_fd [套接字]
 * @return           [description]
 */
bool Is_Shell(char * buf,int socket_fd)
{
	int tmp = 0;
	//ls命令
	if(strncmp(buf,"ls",2) == 0)
	{
		system(buf);
		Display(NULL);
		return true;
	}
	//cp命令
	else if(strncmp(buf,"cp",2) == 0)
	{
		system(buf);
		Display(NULL);
		return true;
	}
	//mv命令
	else if(strncmp(buf,"mv",2) == 0)
	{
		system(buf);
		Display(NULL);
		return true;
	}
	//rm命令
	else if(strncmp(buf,"rm",2) == 0)
	{
		system(buf);
		Display(NULL);
		return true;
	}
	//tree命令
	else if(strncmp(buf,"tree",4) == 0)
	{
		system(buf);
		Display(NULL);
		return true;
	}
	//传送ls命令到服务器
	else if(strncmp(buf,"sl",2) == 0)
	{
		buf[0] = 'l';
		buf[1] = 's';

		send(socket_fd, (const void *)buf, strlen(buf), 0);

		while(1)
		{
			recv(socket_fd,(void *)&tmp, sizeof(int), 0);
			if(tmp != EOF)
				printf("%c",tmp);
			else
			{
				printf("\n");
				break;
			}
		}
		Display(NULL);
		return true;
	}
	return false;
}

/**
 * [查找应有的文件加是否存在]
 */
void Check_Dir(void)
{
	DIR *dir;

	//打开下载文件夹
	dir = opendir("./file_recv");
	if( dir == NULL)
		mkdir("./file_recv",0766);
	else
		closedir(dir);

	//打开共享文件夹
	dir = opendir("./share");
	if( dir == NULL)
		mkdir("./share",0766);
	else
		closedir(dir);
}
