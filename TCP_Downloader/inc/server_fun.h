/*************************************************************************
 * @Filename              :  server_fun.h
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-09 Thu
 ************************************************************************/


#ifndef __SERVER_FUN_H
#define __SERVER_FUN_H

#include "MySocket.h"

#define 	BUF_SIZE 	100
#define		LINE_SIZE 	50
#define 	USER_NUM	5

typedef struct
{
	int fd;
	char user_name[256];

	struct sockaddr_in client_addr;
}socket_t;

bool Turn_Off(char * buf);
void Display(char * buf);

bool Send_File(char * buf,int socket_fd);
bool Get_File(char * buf,int socket_fd);
bool Is_Shell(char * buf,int socket_fd,int online_num);

extern socket_t new_socket[USER_NUM];
void Check_Dir(void);	

#endif
