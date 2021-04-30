/*************************************************************************
 * @Filename              :  client_fun.h
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-10 Fri
 ************************************************************************/


#ifndef __CLIENT_FUN_H
#define __CLIENT_FUN_H

#include "MySocket.h"

#define 	BUF_SIZE 	100		//缓冲区大小
#define		LINE_SIZE 	50		//行数的大小

void Display(char * buf);			//显示函数

void Turn_Off(char * buf,int socket_fd);	//检查是否退出
bool Send_File(char * buf,int socket_fd);	//检查是否发送文件
bool Get_File(char * buf,int socket_fd);	//检查是否接受文件
bool Is_Shell(char * buf,int socket_fd);	//检查是否为命令

void Check_Dir(void);				//检查共享文件夹和下载目录是否存在

#endif
