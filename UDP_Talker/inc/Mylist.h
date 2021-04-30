/*************************************************************************
 * @Filename              :  Mylist.h
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-03 Fri
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-12 Sun
 ************************************************************************/



#ifndef __MYLIST_H
#define __MYLIST_H

#include "includes.h"

//用户链表结构体
typedef struct user
{
	int user_id;			//用户ID
	struct sockaddr_in addr;	//用户地址

	struct user *next;		//下一个用户信息的指针

	char user_name[0];		//用户名称
}user_list;

//添加用户到用户链表
void 		Add_User	(int client_id,struct sockaddr_in * client_addr,char *buf,user_list * user);
//从链表中删除用户
void 		Del_User	(int client_id,user_list *user);
//在链表中寻找用户
user_list * 	Find_User	(char * buf,user_list *user);
//删除链表
void 		Destory_List	(user_list *user);

#endif
