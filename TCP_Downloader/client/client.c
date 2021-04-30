/*************************************************************************
 * @Filename              :  client.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-07-30 Mon
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-10 Fri
 ************************************************************************/


#include "MySocket.h"
#include "client_fun.h"

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Usage : %s <ip>\n",argv[0]);
		exit(0);
	}

	int socket_fd = 0;			//socket文件描述符
	int UDP_socket_fd = 0;

	char *buf;			//缓冲区

	Check_Dir();

	UDP_socket_fd = UDP_Socket_Create(AF_INET,SOCK_DGRAM);
	socket_fd =  Client_Socket_Create(AF_INET,SOCK_STREAM,argv[1],UDP_socket_fd);	//创建socket

        //接收socket内的信息
        buf = calloc(1,BUF_SIZE);

	if(argc > 2)
	{
		//从入参获取指令
		strcpy(buf,argv[2]);
		strcat(buf," ");
		strcat(buf,argv[3]);

		//检查是否是收发文件
		Send_File(buf,socket_fd);
		Get_File(buf,socket_fd);

		//退出
		bzero(buf,BUF_SIZE);
		strcpy(buf,"exit");
		getc(stdin);
		Turn_Off(buf,socket_fd);

	}
	else
	{
		Display(NULL);

		while(1)
		{
			//捕捉输入
			fgets(buf,BUF_SIZE,stdin);

			buf[strlen(buf)-1] = '\0';
			//检查是否退出
			Turn_Off(buf,socket_fd);

			//检查是否收发文件或指令
			if(Get_File(buf,socket_fd) == false &&
				Send_File(buf,socket_fd) == false &&
				Is_Shell(buf,socket_fd) == false)
			{
				printf("Error Command\n");
				Display(NULL);
			}
		}
	}

	sleep(1);

        //释放资源
        free(buf);
        close(socket_fd);
	close(UDP_socket_fd);

        return 0;
}
