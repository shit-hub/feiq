/*************************************************************************
 * @Filename              :  Display.c
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-04 Sat
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-12 Sun
 ************************************************************************/

#include "includes.h"

static WINDOW * OutputWin;	//输出窗口
static WINDOW * OutputBoxWin;	//输出边框窗口

static WINDOW * InputWin;	//输入窗口
static WINDOW * InputBoxWin;	//输入边框窗口

static WINDOW * UserWin;	//用户列表窗口
static WINDOW * UserBoxWin;	//用户列表窗口
/**
 * [窗口初始化]
 */
void Win_Init(void)
{
	int row = 0, col = 0;		//默认总端的行与列

	setlocale(LC_ALL,"");		//获取本地字符，支持中文：须在初始化窗口前

	//初始化窗口，开启鼠标检测
	initscr();
	mousemask(ALL_MOUSE_EVENTS,NULL);

	//检测终端是否支持颜色
	if(has_colors() == FALSE)
	{
		endwin();
		printf("You terminal dose not support color\n");
		exit(1);
	}
	//开启颜色模式
	start_color();

	//清屏
	clear();
	refresh();

	//获取终端的大小
	getmaxyx(stdscr,row,col);

	//设置各个窗口的大小
	OutputWin = newwin(row*2/3 - 1, col*3/4 - 2, 1, 1);
	OutputBoxWin = newwin(row*2/3 + 1, col*3/4, 0, 0);

	InputWin = newwin( row/3 - 2, col*3/4 - 2, row*2/3 + 1,1);
	InputBoxWin = newwin( row/3, col*3/4, row*2/3,0);

	UserWin = newwin(row - 2, col/4 - 2, 1, col*3/4);
	UserBoxWin = newwin(row, col/4 + 1, 0, col*3/4 - 1);

	//开启输出窗口的桂平效果
	scrollok(OutputWin,TRUE);
	scrollok(InputWin,TRUE);
	scrollok(UserWin,TRUE);

	//设置颜色配置样式
	init_pair(1,COLOR_CYAN,COLOR_BLUE);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_BLACK);

	//给窗口配置颜色样式
	wattron(OutputWin,COLOR_PAIR(2));
	wattron(InputWin,COLOR_PAIR(2));
	wattron(UserWin,COLOR_PAIR(2));

	//设置边框颜色
	wattron(OutputBoxWin,COLOR_PAIR(1));
	wattron(InputBoxWin,COLOR_PAIR(1));
	wattron(UserBoxWin,COLOR_PAIR(1));

	//设置边框样式
	wborder(OutputBoxWin,'|','|','-','-','+','+','+','+');
	wborder(InputBoxWin,'|','|','-','-','+','+','+','+');
	wborder(UserBoxWin,'|','|','-','-','+','+','+','+');

	keypad(stdscr,TRUE);	//打开的键盘定义：支持方向键等
	cbreak();		//键盘输入不进入缓冲区，直接捕捉
	noecho();		//输入使不显示在屏幕上

	//刷新各个窗口
	wrefresh(OutputBoxWin);
	wrefresh(InputBoxWin);
	wrefresh(UserBoxWin);
}

/**
 * [回收串口]
 */
void Win_Deinit(void)
{
	//回收各个窗口
	delwin(OutputWin);
	delwin(OutputBoxWin);
	delwin(InputWin);
	delwin(InputBoxWin);
	delwin(UserWin);
	delwin(UserBoxWin);

	beep();		//响铃
	sleep(1);
	endwin();
}

/**
 * [在输出窗口显示命令]
 * @param buf [要显示的内容]
 */
void Write_Output_Tip(char *buf)
{
	wattron(OutputWin,COLOR_PAIR(3));	//命令使用绿色

	wprintw(OutputWin,"----->");		//命令提示符
	wprintw(OutputWin,buf);
	wrefresh(OutputWin);

	wattron(OutputWin,COLOR_PAIR(2));

	Write_Input_Win(NULL);
}

/**
 * [在输出串口显示数据]
 * @param buf [要显示的内容]
 */
void Write_Output_Win(char *buf)
{
	wprintw(OutputWin,buf);
	wrefresh(OutputWin);
	Write_Input_Win(NULL);
}

/**
 * [在输入窗口显示数据]
 * @param buf [要显示的数据]
 */
void Write_Input_Win(char *buf)
{
	if(buf != NULL)
		wprintw(InputWin,buf);
	wrefresh(InputWin);
}

/**
 * [从输入窗口获取名字]
 * @param  buf [显示的提示语]
 * @return     [description]
 */
char * User_Name_Input(char *buf)
{
	wprintw(InputWin,buf);
	wrefresh(InputWin);

	char *  user_name = calloc(1,BUF_SIZE);

	wgetstr(InputWin,user_name);
	wclear(InputWin);
	wrefresh(InputWin);

	return user_name;
}

/**
 * [更新用户列表窗口]
 * @param user [链表头指针]
 */
void Write_User_Win(user_list *user)
{
	char *buf = calloc(1,2*BUF_SIZE);
	wclear(UserWin);
	wrefresh(UserWin);

	//遍历用户链表，查询用户名字与IP
	while(user != NULL)
	{
		sprintf(buf,"%s\n%s\n------------------",user->user_name,inet_ntoa(user->addr.sin_addr));

		wprintw(UserWin,buf);
		wrefresh(UserWin);

		user = user->next;
	}

	Write_Input_Win(NULL);
}

/**
 * [从输入端口获取数据]
 * @return  [description]
 */
char * Read_Input_Win(user_list *user)
{
	char * buf = calloc(1,BUF_SIZE);
	int tmp = 0;		//捕捉键盘输入的字符
	int num = 0;		//当前输入字符的个数

	int startX = 0,startY = 0;	//开始的坐标
	int x = 0,y = 0;		//当前的坐标位置
	int select_user_num = 0;	//选择的用户

	MEVENT event;			//鼠标输入事件结构体

	getyx(InputWin,startY,startX);	//获取开前的坐标
	wmove(InputWin,startY,startX);
	while(1)
	{
		getyx(InputWin,y,x);	//获取当前坐标位置
		tmp = getch();		//获取字符

		//判断输入事件是否为鼠标事件，以及获取鼠标事件是否成功
		if(tmp == KEY_MOUSE && getmouse(&event) == OK)
		{
			//判断是否是双击事件
			if(event.bstate & BUTTON1_DOUBLE_CLICKED)
			{
				//判断点击的是否是用户列表
				if(event.x > 60 && event.y % 3 != 0)
				{
					char *msg = calloc(1,BUF_SIZE);
					select_user_num = event.y / 3;

					//查询用户
					for (size_t i = 0; i < select_user_num; i++)
					{
						if(user != NULL)
							user = user->next;
					}

					//开启私聊终端
					if(user != NULL)
					{
						strcpy(msg,"gnome-terminal -x ./feiq ");
						strcat(msg,user->user_name);
						strcat(msg," ");
						strcat(msg,inet_ntoa(user->addr.sin_addr));
						system(msg);
					}

					free(msg);
				}
			}
		}
		//按下回车则结束循环
		else if(tmp == '\n')
		{
			break;
		}
		else
		{
			//判断输入字符是否为特殊键，若不是，则输出到屏幕上
			switch(tmp)
			{
				//左方向键
				case KEY_LEFT 	:
						if(x > startX && y == 0)
						{
							x --;
							num --;
						}
						wmove(InputWin,y,x);
						break;
				//右方向键
				case KEY_RIGHT	:
						if(x < startX + num)
						{
							x ++;
							num ++;
						}
						wmove(InputWin,y,x);
						break;
				//退格键
				case 127	:
						wmove(InputWin,y,--x);
						buf[--num] = 0;
						waddch(InputWin,' ');
						break;
				//其他按键
				default 	:
						if(tmp < 127 && tmp > 31)
						{
							buf[num++] = tmp;
							waddch(InputWin,tmp);
						}
						break;
			}
			wrefresh(InputWin);	//刷新屏幕
		}
	}

	wclear(InputWin);	//按下enter后清空输入屏幕，将信息返回处理
	wrefresh(InputWin);

	return buf;
}
