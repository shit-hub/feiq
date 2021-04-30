/*************************************************************************
 * @Filename              :  MySocket.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-10 Fri
 ************************************************************************/


#include "MySocket.h"

int Server_Socket_Create(int num)
{
	int socket_fd = socket(AF_INET,SOCK_STREAM,0);		//创建socket
	socklen_t addr_size = sizeof(struct sockaddr);		//记录地址结构体大小

	int sin_size = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &sin_size, sizeof(int));	//开启地址复用

 	struct sockaddr_in server_addr;		//socket信息数据结构

	memset((void *)&server_addr, 0, addr_size);		//先清空缓冲区
	server_addr.sin_family = AF_INET;			//地址族：ipv4
	server_addr.sin_port = htons(DEFAULT_PORT);		//端口号（大端序）
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//ip地址

	//绑定端口与IP
 	bind(socket_fd,(struct sockaddr *)&server_addr, addr_size);
	//监听等待队列
	listen(socket_fd,num);

	return socket_fd;
}

int Client_Socket_Create(int demain,int type,char *IP_address,int UDP_socket_fd)
{
 	int socket_fd = 0;		//套接字文件描述符

	int user_id = -1;				//用户ID
	int buf_size = 0;				//缓冲区大小
	char * buf = NULL;				//蝗虫去指针

 	socklen_t addr_size = sizeof(struct sockaddr);	//地址结构体大小

	//创建1个TCPsocket
        socket_fd = socket(demain,type,0);

        //绑定端口与IP
        struct sockaddr_in server_addr;		//socket信息数据结构

        memset((void *)&server_addr, 0, addr_size);		//先清空缓冲区
        server_addr.sin_family = demain;			//地址族：ipv4
        server_addr.sin_port = htons(DEFAULT_PORT);		//端口号（大端序）
        server_addr.sin_addr.s_addr = inet_addr(IP_address);	//ip地址

	if(connect(socket_fd,(struct sockaddr *)&server_addr, addr_size) != 0)
	{
		//若发现服务器不存在，则向聊天端发送信号
		buf=calloc(1,256);
		strcpy(buf,"Downloader");
		buf_size = strlen(buf)+1;

		//发送信号
		sendto(UDP_socket_fd, &user_id, sizeof(int), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		sendto(UDP_socket_fd, &buf_size, sizeof(int), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		sendto(UDP_socket_fd, buf, buf_size, 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

		//睡眠1s后继续检测
		sleep(1);
		if(connect(socket_fd,(struct sockaddr *)&server_addr, addr_size) != 0)
		{
			printf("User too much,Pleasr try again later\n");
			sleep(1);
			free(buf);
			exit(0);
		}
		free(buf);
	}

	return socket_fd;
}

int UDP_Socket_Create(int demain,int type)
{
 	int socket_fd;					//套接字文件描述符
 	socklen_t addr_size = sizeof(struct sockaddr);	//地址 结构体大小

	//创建1个socket
        socket_fd = socket(demain,type,0);

        //绑定端口与IP
        struct sockaddr_in server_addr;			//socket信息数据结构

        memset((void *)&server_addr, 0, addr_size);		//先清空缓冲区
        server_addr.sin_family = demain;			//地址族：ipv4
        server_addr.sin_port = htons(DEFAULT_PORT);		//端口号（大端序）
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//ip地址

	//绑定自身IP
        bind(socket_fd,(struct sockaddr *)&server_addr , addr_size);

	return socket_fd;
}
