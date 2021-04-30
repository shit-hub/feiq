/*************************************************************************
 * @Filename              :  Mylist.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-03 Fri
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-09 Thu
 ************************************************************************/



#include "includes.h"

/**
 * [向链表中添加用户]
 * @param client_id   [用户ID]
 * @param client_addr [用户地址族]
 * @param buf         [用户名称]
 * @param user        [链表头]
 */
void Add_User(int client_id,struct sockaddr_in * client_addr,char *buf,user_list * user)
{
	user_list *client = NULL;

	client = calloc(1,sizeof(user_list) + strlen(buf) + 1);
	client->user_id = client_id;

	memcpy(&client->addr,client_addr,sizeof(struct sockaddr_in));

	client->addr.sin_port = htons(DEFAULT_PORT);	//使用默认端口号

	while(user->next != NULL)
		user = user->next;

	user->next = client;

	client->next = NULL;

	strcpy(client->user_name, buf);
}

/**
 * [从链表中删除某个用户]
 * @param client_id [用户ID]
 * @param user      [description]
 */
void Del_User(int client_id,user_list *user)
{
	user_list *tmp;
	user_list *client;

	while(user ->next != NULL)
	{
		if(user->next->user_id == client_id)
		{
			client = user->next;
			tmp = user;
		}
		if(user->user_id > client_id)
			user->user_id -= 1;

		user = user->next;
	}

	if(user->user_id > client_id)
		user->user_id -= 1;

	tmp->next = tmp->next->next;

	free(client);
}

/**
 * [通过名字查找用户]
 * @param  buf  [用户名称]
 * @param  user [链表透支真]
 * @return      [description]
 */
user_list * Find_User(char * buf,user_list *user)
{
	while(user != NULL && strstr(buf,user->user_name) == NULL)
	{
		user = user->next;
	}

	return user;
}

/**
 * [删除链表]
 * @param user [链表头指针]
 */
void Destory_List(user_list *user)
{
	user_list *tmp = user->next;

	while(tmp != NULL)
	{
		tmp = tmp->next;
		free(user);
		user = tmp;
	}
}
