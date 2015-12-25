#ifndef TRACK_CTRL_PARAMS
#define TRACK_CTRL_PARAMS
#pragma once

typedef struct
{
	int tea_time_min;//教师画面切换最小保持时间
	int stu_time_min;//学生画面切换最小保持时间
	int ppt_time_min;//ppt画面切换最小保持时间
	int blb_time_min;//黑板画面切换最小保持时间
}Time_Set_t;

typedef struct
{
	Time_Set_t time;
	int mut_pic_flag;//多画面标志位 设置为1表示需要多画面，0表示关闭多画面
	int stu_track_flag;//开启学生标志位 设置为1表示需要学生跟踪，0表示关闭学生跟踪
	int stu_overview_flag;//开启学生全景标志位 设置为1表示需要学生全景， 0表示关闭学生全景
}Policy_Set_t;

#endif