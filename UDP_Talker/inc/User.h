/*************************************************************************
 * @Filename              :  client_fun.h
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-12 Sun
 ************************************************************************/


#ifndef __CLIENT_FUN_H
#define __CLIENT_FUN_H

#include "includes.h"

#define 	BUF_SIZE 	256	//缓冲区大小

//信息数据包
typedef struct package
{
	int user_id;		//用户ID
	int buf_size;		//内容大小

	char buf[0];		//内容
}package_t;

//以不同方式发送信息
void 	Send_To_Someone	(int socket_fd, package_t * package,struct sockaddr *client_addr);
void 	Send_To_Group	(int socket_fd, package_t * package);
void 	Send_To_All	(int socket_fd, package_t * package);

//获取用户列表
user_list * 	Get_User_Name	(void);
void 		Add_Client	(user_list *user,char * user_name, char *IP_Address);

//上线操作与下线操作
bool 	Online_Opration		(int socket_fd, user_list * user);
void 	Offline_Opration	(int socket_fd,user_list *user);

//接受数据包与将数据打包成数据包
bool 		Recv_Package	(int socket_fd,user_list *user);
package_t * 	Get_Package	(int user_id,char *buf);

//接受信息是否是信号
int 	Is_Signal	(char * buf);
//输入信息是否是特殊操作
int	Is_Opration	(int socket_fd, char * buf,user_list * user);

#endif
