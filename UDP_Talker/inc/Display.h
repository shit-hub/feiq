/*************************************************************************
 * @Filename              :  Display.h
 * @Author                :  sanjo heron
 * @Email                 :  sanjo.heron@gmail.com
 * @Date                  :  2018-08-04 Sat
 * @Last modified by      :  sanjo heron
 * @Last modified time    :  2018-08-08 Wed
 ************************************************************************/

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "includes.h"

void Win_Init(void);
void Win_Deinit(void);

void Write_Output_Tip(char *buf);
void Write_Output_Win(char *buf);
void Write_Input_Win(char *buf);
void Write_User_Win(user_list *user);

char * User_Name_Input(char *buf);
char * Read_Input_Win(user_list *user);



#endif
