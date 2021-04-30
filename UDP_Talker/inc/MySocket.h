/*************************************************************************
 * @Filename              :  MySocket.h
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-12 Sun
 ************************************************************************/



#ifndef __MYSOCKET_H
#define __MYSOCKET_H

#include "includes.h"

#define DEFAULT_PORT	9520			//默认端口号
#define DEFAULT_GROUP 	"236.236.236.236"	//组播IP
#define BROADCAST_IP	"192.168.14.255"	//广播IP

#define SIMPLE		0
#define MULTICAST 	1
#define BROADCAST	2

int 	UDP_Socket_Create 	(void);		//创建socket：加入默认组并开启广播
bool 	Change_Group		(int socket_fd,char *group);	//改变用户所在组

#endif
