#ifndef TRACK_CTRL_PARAMS
#define TRACK_CTRL_PARAMS
#pragma once

typedef struct
{
	int tea_time_min;//��ʦ�����л���С����ʱ��
	int stu_time_min;//ѧ�������л���С����ʱ��
	int ppt_time_min;//ppt�����л���С����ʱ��
	int blb_time_min;//�ڰ廭���л���С����ʱ��
}Time_Set_t;

typedef struct
{
	Time_Set_t time;
	int mut_pic_flag;//�໭���־λ ����Ϊ1��ʾ��Ҫ�໭�棬0��ʾ�رն໭��
	int stu_track_flag;//����ѧ����־λ ����Ϊ1��ʾ��Ҫѧ�����٣�0��ʾ�ر�ѧ������
	int stu_overview_flag;//����ѧ��ȫ����־λ ����Ϊ1��ʾ��Ҫѧ��ȫ���� 0��ʾ�ر�ѧ��ȫ��
}Policy_Set_t;

#endif