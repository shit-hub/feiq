/*************************************************************************
 * @Filename              :  main.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-07-30 Mon
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-14 Tue
 ************************************************************************/


#include "includes.h"

extern int talk_way;

user_list *user;
int talk_way = 0;

void *Get_Data_From_Win(void *arg);

int main(int argc, char *argv[])
{
	if(argc > 1)
		talk_way = 1;

	int socket_fd = 0;			//socket文件描述符
	pthread_t pthread_id;

	Win_Init();

	socket_fd = UDP_Socket_Create();	//创建UDP：加入默认组，开启广播
	user = Get_User_Name();			//获取用户名称

	if(talk_way)
	{
		Add_Client(user,argv[1],argv[2]);	//添加用户
		Write_User_Win(user->next);		//更新用户列表
	}
	else
		Online_Opration(socket_fd,user);	//上线操作：广播获取用户列表


	pthread_create(&pthread_id,NULL,Get_Data_From_Win,&socket_fd);	//创建线程

	while(1)
	{
		Recv_Package(socket_fd,user);		//接受数据
		if(talk_way == 2)			//判断是否退出
			break;
	}

	Offline_Opration(socket_fd,user);		//下线操作

	sleep(1);
	Win_Deinit();		//窗口还原

        //释放资源
	Destory_List(user);
        close(socket_fd);

        return 0;
}

/**
 * [从屏幕捕捉输入的线程]
 * @param  arg [socket的文件描述符]
 * @return     [description]
 */
void *Get_Data_From_Win(void *arg)
{
	int cmd_mode = 0;	//记录是否为命令模式
	char *buf = NULL;	//数据缓冲区
	time_t current_time;	//当前时间

	char *message = calloc(1,BUF_SIZE + 20 + strlen(user->user_name));

	while(1)
	{
		//根据模式在输入框中显示
		if(cmd_mode)
			Write_Input_Win("cmd > ");
		else
			Write_Input_Win(NULL);

		buf = Read_Input_Win(user);		//从输入窗口捕获数据
		time(&current_time);		//获取当前时间

		if(strlen(buf) == 0)		//切换模式
		{
			cmd_mode = (cmd_mode)?0:1;
			continue;
		}

		if(cmd_mode)		//跟句模式操作
		{
			if(!Is_Opration(*(int *)arg,buf,user))
				Write_Output_Tip("Error commond\n\n");
		}
		else
		{
			//屏幕显示数据
			sprintf(message,"%s %s%s\n",user->user_name,ctime(&current_time)+11,buf);
			Write_Output_Win(message);
			Write_Output_Win("\n");

			//跟句模式选择饲料还是群发
			if(talk_way == 0)
				Send_To_Group(*(int *)arg,Get_Package(user->user_id,message));
			else
			{
				sprintf(message,"(private)%s %s%s\n",user->user_name,ctime(&current_time)+11,buf);
				Send_To_Someone(*(int *)arg,Get_Package(user->user_id,message),(struct sockaddr *)&user->next->addr);
			}
		}

		free(buf);
	}

	free(message);
}
