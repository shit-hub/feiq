/*************************************************************************
 * @Filename              :  server.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-07-30 Mon
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-10 Fri
 ************************************************************************/

#include <pthread.h>

#include "MySocket.h"
#include "server_fun.h"

void *Socket_Dispose(void *arg);		//套阶字处理函数
int online_num = 0;				//在线人数

int main(int argc, char const *argv[])
{
	int socket_fd;		//套接字文件描述符
	pthread_t pid;		//线程PID
	socklen_t addr_size = sizeof(struct sockaddr);	//接受字节数与结构体大小

	//检查所需文件夹是否存在，不在则创建
	Check_Dir();

	socket_fd = Server_Socket_Create(10);	//创建socket

	struct sockaddr_in client_addr;		//socket信息数据结构

	while(1)
	{
		//查看链接人数是否在规定的人数之内
		if(online_num < USER_NUM)
		{
			//等待接受链接信息
			bzero(&new_socket[online_num],sizeof(socket_t));
			new_socket[online_num].fd = accept(socket_fd,(struct sockaddr *)&client_addr,&addr_size);
			if(new_socket[online_num].fd < 0)
			{
				printf("server accept error\n");
				exit(0);
			}
			//链接成功后将文件描述符传给子线程进行处理
			pthread_create(&pid,NULL,Socket_Dispose,(void *)&new_socket[online_num].fd);
			online_num++;
		}

		//若没有用户链接则退出
		if(online_num == 0)
			break;
	}
	sleep(1);

	//释放资源
	close(socket_fd);

 	return 0;
}

void *Socket_Dispose(void *arg)
{
	socklen_t recv_size = 0;		//接受的字节数大小
	char *buf = malloc(BUF_SIZE);		//接受缓冲区
	int thread_num = 0;			//记录当前是第几个线程

	while(1)
	{
		bzero(buf,BUF_SIZE);

		//接受指令
		recv_size = recv(*(int *)arg, buf, BUF_SIZE, 0);
		if(recv_size == 0)
			exit(0);
		else if(recv_size < BUF_SIZE)
			buf[recv_size] = '\0';

		//检查是否退出
		if(Turn_Off(buf))
		{
			close(*(int *)arg);		//关闭文件描述符

			//查找自身是第几个线程
			for (size_t i = 0; i < online_num; i++)
			{
				if(new_socket[i].fd == *(int *)arg)
					thread_num = i;
			}

			//将后面的文件描述符移上来
			for (size_t j = thread_num; j < online_num - 1; j++)
				new_socket[j].fd = new_socket[j+1].fd;
			//最后的文件描述符清零
			new_socket[online_num - 1].fd = 0;
			//在线人数减少
			online_num --;
			break;
		}

		Send_File(buf,*(int *)arg);			//是否发送文件
		Get_File(buf,*(int *)arg);			//是否接受文件
		Is_Shell(buf,*(int *)arg,online_num);		//是否使指令
	}

	free(buf);
	pthread_exit(NULL);
}
