/*************************************************************************
 * @Filename              :  MySocket.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-12 Sun
 ************************************************************************/


#include "MySocket.h"

extern int talk_way;

/**
 * [创建socket：使用UDP协议]
 * @return [description]
 */
int UDP_Socket_Create()
{
 	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);		//使用UDP协议
 	socklen_t addr_size = sizeof(struct sockaddr);		//获取地址结构体大小

	//判断是否为广播，广播则加入默认组以及开启广播
	if(talk_way == 0)
	{
		struct ip_mreq mreq;
		bzero(&mreq,sizeof(mreq));
		mreq.imr_multiaddr.s_addr = inet_addr(DEFAULT_GROUP);		//组播IP
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		//加入默认组
		setsockopt(socket_fd,IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));

		//开启广播
		int size = 1;
		setsockopt(socket_fd,SOL_SOCKET, SO_BROADCAST, &size,sizeof(int));
	}
	//绑定端口与IP
	struct sockaddr_in server_addr;		//socket信息数据结构

	memset((void *)&server_addr, 0, addr_size);	//先清空缓冲区
	server_addr.sin_family = AF_INET;		//地址族：ipv4
	server_addr.sin_port = htons(DEFAULT_PORT);	//端口号（大端序）
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//ip地址

	//绑定端口与IP
	bind(socket_fd,(struct sockaddr *)&server_addr , addr_size);

	return socket_fd;
}

/**
 * [更换所在组]
 * @param  socket_fd [套阶字文件描述符]
 * @param  group     [组播IP]
 * @return           [description]
 */
bool Change_Group(int socket_fd,char *group)
{
	char buf[4];

	memcpy(buf,group,4);
	strtok(group,".");
	if(atoi(buf) > 239 ||  atoi(buf) < 234)
	{
		printf("\rChange group failed\n");
		return false;
	}

	struct ip_mreq mreq;
	bzero(&mreq,sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(group);

	setsockopt(socket_fd,IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
	printf("\rChange group successed\n");
	return true;
}
