/*************************************************************************
 * @Filename              :  MySocket.h
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-10 Fri
 ************************************************************************/



#ifndef __MYSOCKET_H
#define __MYSOCKET_H

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#include <sys/uio.h>

#define DEFAULT_PORT 9520	//默认端口号

int Server_Socket_Create(int num);		//创建服务器
int Client_Socket_Create(int demain,int type,char *IP_address,int UDP_socket_fd);	//创建客户端
int UDP_Socket_Create(int demain,int type);	//创建UDPsocket

#endif
