/*************************************************************************
 * @Filename              :  server_fun.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-09 Thu
 ************************************************************************/


#include "server_fun.h"
#include <sys/uio.h>
#include <sys/stat.h>
#include <dirent.h>

socket_t new_socket[USER_NUM];

bool Turn_Off(char * buf)
{
	if(strstr(buf,"exit"))
		return true;
	else
		return false;
}
bool Send_File(char * buf,int socket_fd)
{
	FILE *fptr;
	int tmp = 0;

	unsigned long size = 0;
	unsigned long acount_size = 0;

	char * path = calloc(1,256);

	if(strncmp(buf,"get",3) == 0)
	{
		strcpy(path,"./share/");
		strcat(path,buf + 4);
		fptr = fopen(path,"r");

		if(fptr == NULL)
		{
			send(socket_fd, &acount_size, sizeof(int), 0);
			printf("Path error\n");
			return false;
		}
		else
		{
			fseek(fptr,0,SEEK_END);
			acount_size = ftell(fptr);
			fseek(fptr,0,SEEK_SET);

			send(socket_fd, &acount_size, sizeof(int), 0);

			recv(socket_fd,(void *)&tmp, sizeof(int), 0);
			size = tmp;

			while(size < acount_size)
				sendfile(socket_fd, fileno(fptr), 1024, (off_t *)&size, NULL, 0);
		}
		fclose(fptr);
		free(path);
		return true;
	}
	free(path);
	return false;
}

bool Get_File(char * buf,int socket_fd)
{
	FILE *fptr;

	int tmp = 0;
	char * msg = calloc(1,1024);
	char * path = calloc(1,256);

	unsigned long size = 0;
	unsigned long acount_size = 0;

	int recv_size = 0;

	if(strncmp(buf,"send",4) == 0)
	{
		strcpy(path,"./file_recv/");
		strcat(path,buf + 5);
		fptr = fopen(path,"a");
		recv(socket_fd, &acount_size, sizeof(int), 0);
		if(fptr == NULL)
		{
			printf("Get file error!\n");
			return false;
		}
		else
		{
			tmp = ftell(fptr);
			send(socket_fd, &tmp, sizeof(int), 0);
			size = tmp;
			while(size < acount_size)
			{
				bzero(msg,1024);
				recv_size = read(socket_fd, msg, 1024);
				write(fileno(fptr),msg,recv_size);
				size += recv_size;
			}
		}
		fclose(fptr);
		free(msg);
		free(path);
		return true;
	}
	free(path);
	free(msg);
	return false;
}


bool Is_Shell(char * buf,int socket_fd,int online_num)
{
	int tmp = 0;
	FILE *fptr;

	if(strncmp(buf,"ls",2) == 0)
	{
		strcpy(buf,"ls share -l ");
		strcat(buf," > txt\n");
		system(buf);

		fptr = fopen("txt","r");

		while(1)
		{
			tmp = fgetc(fptr);
			if(tmp != EOF)
				send(socket_fd, (const void *)&tmp, sizeof(int), 0);
			else
			{
				send(socket_fd, (const void *)&tmp, sizeof(int), 0);
				break;
			}
		}
		system("rm txt");

		fclose(fptr);
		return true;
	}
	return false;
}

void Check_Dir(void)
{
	DIR *dir;
	dir = opendir("./file_recv");
	if( dir == NULL)
		mkdir("./file_recv",0766);
	else
		closedir(dir);

	dir = opendir("./share");
	if( dir == NULL)
		mkdir("./share",0766);
	else
		closedir(dir);
}
