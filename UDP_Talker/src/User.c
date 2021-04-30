/*************************************************************************
 * @Filename              :  client_fun.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-02 Thu
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-14 Tue
 ************************************************************************/



#include "includes.h"

extern int talk_way;			//私聊模式判别

static bool user_exist = false;		//判断用户是否初始化成功
static pid_t downloader_id = 0;		//下载器的进程ID

/**
 * [发送数据到某人]
 * @param socket_fd   [socket文件描述符]
 * @param package     [数据包指针]
 * @param client_addr [对方地址族]
 */
void Send_To_Someone(int socket_fd, package_t * package,struct sockaddr *client_addr)
{
	sendto(socket_fd, &package->user_id, sizeof(int), 0, client_addr, sizeof(struct sockaddr));

	sendto(socket_fd, &package->buf_size, sizeof(int), 0, client_addr, sizeof(struct sockaddr));

	sendto(socket_fd, package->buf, package->buf_size, 0, client_addr, sizeof(struct sockaddr));

	free(package);
}

/**
* [发送数据到某组]
* @param socket_fd   [socket文件描述符]
* @param package     [数据包指针]
 */
void Send_To_Group(int socket_fd,package_t * package)
{
	struct sockaddr_in client_addr;

	memset((void *)&client_addr, 0, sizeof(struct sockaddr_in));	//先清空缓冲区
        client_addr.sin_family = AF_INET;				//地址族：ipv4
        client_addr.sin_port = htons(DEFAULT_PORT);			//端口号（大端序）
        client_addr.sin_addr.s_addr = inet_addr(DEFAULT_GROUP);		//组播ip地址

	sendto(socket_fd, &package->user_id, sizeof(int), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

	sendto(socket_fd, &package->buf_size, sizeof(int), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

	sendto(socket_fd, package->buf, package->buf_size, 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

	free(package);
}

/**
 * [广播数据]
 * @param socket_fd [套接字]
 * @param package   [要发送的数据包]
 */
void Send_To_All(int socket_fd, package_t * package)
{
	struct sockaddr_in client_addr;

	memset((void *)&client_addr, 0, sizeof(struct sockaddr_in));	//先清空缓冲区
        client_addr.sin_family = AF_INET;				//地址族：ipv4
        client_addr.sin_port = htons(DEFAULT_PORT);			//端口号（大端序）
        client_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);		//广播ip地址

	sendto(socket_fd, &package->user_id, sizeof(int), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

	sendto(socket_fd, &package->buf_size, sizeof(int), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

	sendto(socket_fd, package->buf, package->buf_size, 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

	free(package);
}

/**
 * [输入buf，打包称数据包]
 * @param  user_id [用户ID]
 * @param  buf     [数据包内容]
 * @return         [description]
 */
package_t * Get_Package(int user_id,char *buf)
{
	package_t *package;
	int package_size = 0;

	if(buf == NULL)
		return NULL;

	package_size = sizeof(package_t) + strlen(buf) + 1;
	package = malloc(package_size);
	bzero(package,package_size);

	package->user_id = user_id;
	package->buf_size = strlen(buf) + 1;

	memcpy(package->buf,buf,strlen(buf));

	return package;
}

/**
 * [获取用户名称]
 * @return  [description]
 */
user_list * Get_User_Name(void)
{
	char *buf;
	user_list *user;

	FILE * fptr = fopen("user_msg","r+");	//判断文件是否存在

	//不存在则创建，并输入用户名称
	if(fptr == NULL)
	{
		fptr = fopen("user_msg","w+");
		buf = User_Name_Input("Please input user name:");
		fputs(buf,fptr);
	}
	else
	{
		buf = calloc(1,BUF_SIZE);
		fgets(buf,BUF_SIZE,fptr);
	}

	//创建头节点
	user = calloc(1,sizeof(user_list) + strlen(buf) + 1);
	user->user_id = -1;		//默认用户ID

	//自身地址族
	user->addr.sin_family = AF_INET;
	user->addr.sin_port = htons(DEFAULT_PORT);
	user->addr.sin_addr.s_addr = htonl(INADDR_ANY);

	user->next = NULL;

	//加入用户名称
	strcpy(user->user_name, buf);

	free(buf);
	fclose(fptr);
	return user;
}

/**
 * [添加目标用户]
 * @param user       [链表头指针]
 * @param user_name  [用户名称]
 * @param IP_Address [用户IP]
 */
void Add_Client(user_list *user,char * user_name, char *IP_Address)
{
	user_list *client = calloc(1,sizeof(user_list) + strlen(user_name) + 1);
	client->user_id = 0;

	client->addr.sin_family = AF_INET;
	client->addr.sin_port = htons(DEFAULT_PORT);
	client->addr.sin_addr.s_addr = inet_addr(IP_Address);

	strcpy(client->user_name, user_name);

	client->next = NULL;

	user->next = client;

	user_exist = true;
}

/**
 * [上线操作]
 * @param  socket_fd [套接字]
 * @param  user      [链表头指针]
 * @return           [description]
 */
bool Online_Opration(int socket_fd, user_list * user)
{
	fd_set fds;
	struct timeval timeout = {1,0};

	int user_num = 0;

	char buf[256];

	bzero(buf,256);

	strcat(buf,"Pop up Signal!\n");		//广播上线信号
	Send_To_Group(socket_fd,Get_Package(user->user_id,buf));

	//接受在线用户的回馈信号
	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(socket_fd, &fds);

		timeout.tv_sec=1;
		timeout.tv_usec=0;
		switch(select(sysconf(_SC_OPEN_MAX) + 1,&fds,NULL,NULL,&timeout))
		{
		case -1	:
			printf("\rselect error\n");
			exit(-1);
		case 0	:
			user->user_id = user_num;	//得到唯一的用户ID
			Write_User_Win(user);		//更新用户列表

			//组播正式上线信号
			bzero(buf,256);
			memcpy(buf,user->user_name,strlen(user->user_name));
			strcat(buf," online\n");

			Send_To_Group(socket_fd,Get_Package(user->user_id,buf));
			return true;
		default	:
			if(FD_ISSET(socket_fd,&fds))
			{
				Recv_Package(socket_fd,user);
				user_exist = true;
				user_num ++;
			}
		}
	}
	return false;

}

/**
 * [下线操作]
 * @param socket_fd [套接字]
 * @param user      [链表头]
 */
void Offline_Opration(int socket_fd,user_list *user)
{
	char buf[256];

	//组播下线信号
	bzero(buf,256);
	memcpy(buf,user->user_name,strlen(user->user_name));
	strcat(buf," offline\n");;
	Send_To_Group(socket_fd,Get_Package(user->user_id,buf));
}

/**
 * [接受数据包]
 * @param  socket_fd [套接字]
 * @param  user      [用户链表]
 * @return           [description]
 */
bool Recv_Package(int socket_fd,user_list *user)
{
	socklen_t addr_size = sizeof(struct sockaddr);		//结构体字节数
	struct sockaddr_in client_addr;		//记录客户端的端口与IP

	char *buf = NULL;
	char *msg = NULL;
	char *tmp = 0;
	int size = 0;
	int client_id = 0;

	//接受用户ID
	recvfrom(socket_fd, &client_id, sizeof(int), 0, (struct sockaddr *)&client_addr,&addr_size);

	//接受buf大小
	recvfrom(socket_fd, &size, sizeof(int), 0, (struct sockaddr *)&client_addr,&addr_size);

	//创建对应大小的缓冲区
	buf = calloc(1,size);
	msg = calloc(1,1000);

	//接受buf
	recvfrom(socket_fd, buf, size, 0,(struct sockaddr *)&client_addr,&addr_size);

	//私聊则显示信息
	if(talk_way == 1)
	{
		sprintf(msg,"%s\n",buf);
		Write_Output_Win(msg);
	}
	else if(client_id != user->user_id && user_exist == true)
	{
		//判断信号类型
		switch(Is_Signal(buf))
		{
			case 1 :
				bzero(msg,1000);
				strcpy(msg,user->user_name);
				strcat(msg," on line!\n");
				client_addr.sin_port = htons(DEFAULT_PORT);
				Send_To_Someone(socket_fd, Get_Package(user->user_id,msg),(struct sockaddr *)&client_addr);
				break;
			case 2 :
				Add_User(client_id,&client_addr,buf,user);
				Write_User_Win(user);
				break;
			case 3 :
				Add_User(client_id,&client_addr,buf,user);
				Write_User_Win(user);
				break;
			case 4 :
				Del_User(client_id,user);
				Write_User_Win(user);
			case 5 :break;
			case 6 :sprintf(msg,"%s",buf);
				Write_Input_Win(msg);
				//tmp = Read_Input_Win();
				if(strcmp(tmp,"n"))
				{
					if(downloader_id != 0)
					{
						if(waitpid(downloader_id,NULL,WNOHANG) != 0)
							downloader_id = 0;
					}
					else
					{
						downloader_id = fork();
						if(downloader_id == 0)
							execl("./Downloader"," ",NULL);
					}

					strcpy(msg,"Uploader ");

					if(strstr(buf,"Receive") != 0)
					{
						strcat(msg,"send ");
						tmp = strstr(buf,"file");
						strcat(msg,tmp + 5);
						tmp = strstr(msg,"from");
						*(tmp - 1) = '\0';
					}
					else if(strstr(buf,"Send") != 0)
					{
						strcat(msg,"get ");
						tmp = strstr(buf,"file");
						strcat(msg,tmp + 5);
						tmp = strstr(msg,"to");
						*(tmp - 1) = '\0';
					}

					client_addr.sin_port = htons(DEFAULT_PORT);
					Send_To_Someone(socket_fd, Get_Package(user->user_id,msg),(struct sockaddr *)&client_addr);
				}
				break;
			case 7 :
				strcpy(msg,"gnome-terminal -x ./Uploader ");
				strcat(msg,inet_ntoa(client_addr.sin_addr));
				strcat(msg," ");
				if(strlen(buf) > 9)
					strcat(msg,buf + 9);
				system(msg);
				break;
			default:
				sprintf(msg,"%s\n",buf);
				Write_Output_Win(msg);
				break;
		}
	}
	//得到自身IP
	else if(user_exist == false)
	{
		memcpy(&user->addr,&client_addr,sizeof(struct sockaddr));
	}
	//自身下线信号
	else if(client_id == user->user_id && user_exist == true)
	{
		if(strcmp(buf,"exit") == 0)
		{
			if(downloader_id != 0)
				kill(downloader_id,SIGKILL);
			talk_way = 2;
		}
	}

	free(buf);
	free(msg);
	return true;
}

/**
 * [判断信号类型]
 * @param  buf [信号内容]
 * @return     [description]
 */
int Is_Signal(char * buf)
{
	char *ret = NULL;

	//普通消息
	if(strstr(buf,"2018") != 0)
		return 0;

	//上线信号
	if(strncmp(buf, "Pop up Signal!", 14) == 0)
	{
		return 1;
	}
	//下载信号
	else if(strncmp(buf, "Downloader", 10) == 0)
	{
		return 5;
	}
	//上传信号
	else if(strncmp(buf, "Uploader", 8) == 0)
	{
		return 7;
	}
	//用户在线信号
	ret = strstr(buf, " on line!");
	if(ret != NULL)
	{
		*ret = '\0';
		return 2;
	}
	//正式上线信号
	ret = strstr(buf, " online");
	if(ret != NULL)
	{
		Write_Output_Tip(buf);
		Write_Output_Win("\n");
		*ret = '\0';
		return 3;
	}
	//下线信号
	ret = strstr(buf, " offline");
	if(ret != NULL)
	{
		Write_Output_Tip(buf);
		Write_Output_Win("\n");
		*ret = '\0';
		return 4;
	}
	//文件传输信号
	ret = strstr(buf, "(Y/n)");
	if(ret != NULL)
	{
		return 6;
	}

	return 0;
}

/**
 * [判断输入操作]
 * @param  socket_fd [套接字]
 * @param  buf       [指令内容]
 * @param  user      [用户链表头指针]
 * @return           [description]
 */
int Is_Opration(int socket_fd, char * buf,user_list * user)
{
	user_list * client;

	//私聊操作
	if(strncmp(buf, "TO ", 3)==0)
	{
		client = Find_User(buf + 3,user);
		if(client == NULL)
		{
			Write_Output_Win("No this user\n");
			return 0;
		}
		strcpy(buf,"gnome-terminal -x ./feiq ");
		strcat(buf,client->user_name);
		strcat(buf," ");
		strcat(buf,inet_ntoa(client->addr.sin_addr));
		system(buf);

		return 1;
	}
	//文件传输操作
	else if(strncmp(buf, "open file manager", 16) == 0 && talk_way == 1)
	{
		strcpy(buf,"---->File transfer request from ");
		strcat(buf,user->user_name);
		strcat(buf,"(Y/n)");
		Send_To_Someone(socket_fd, Get_Package(user->user_id,buf),(struct sockaddr *)&user->next->addr);

		return 1;
	}
	//接受文件操作
	else if(strncmp(buf, "get", 3) == 0 && talk_way == 1)
	{
		char * msg = calloc(1,256);
		strcpy(msg,"---->Send file ");
		strcat(msg,buf + 4);
		strcat(msg," to ");
		strcat(msg,user->user_name);
		strcat(msg,"(Y/n)");
		Send_To_Someone(socket_fd, Get_Package(user->user_id,msg),(struct sockaddr *)&user->next->addr);

		free(msg);
		return 1;
	}
	//发送文件操作
	else if(strncmp(buf, "send", 4) == 0 && talk_way == 1)
	{
		char * msg = calloc(1,256);
		strcpy(msg,"---->Receive file ");
		strcat(msg,buf + 5);
		strcat(msg," from ");
		strcat(msg,user->user_name);
		strcat(msg,"(Y/n)");
		Send_To_Someone(socket_fd, Get_Package(user->user_id,msg),(struct sockaddr *)&user->next->addr);

		free(msg);
		return 1;
	}
	//退出操作
	else if(strncmp(buf, "exit", 4) == 0)
	{
		if(talk_way)
			exit(0);
		user->addr.sin_port = htons(DEFAULT_PORT);
		Send_To_Someone(socket_fd, Get_Package(user->user_id,buf),(struct sockaddr *)&user->addr);
		return 1;
	}
	return 0;
}
