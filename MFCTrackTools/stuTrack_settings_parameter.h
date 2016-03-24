#ifndef stuTrack_settings_parameter_h__
#define stuTrack_settings_parameter_h__

#ifdef  __cplusplus
extern "C" {
#endif

#define HEIGHT_STUTRACK_IMG_ 264
#define WIDTH_STUTRACK_IMG_	480

typedef struct _POINT
{
	int x;
	int y;
}TrackPrarms_Point_t;

typedef struct _SIZE
{
	int width;
	int height;
}TrackPrarms_Size_t;

typedef struct _StuITRACK_ClientParams
{
	int flag_setting;	//�����Ƿ�����
	int height;			//���õ�ͼ��߶�
	int width;			//���õ�ͼ����
	int stuTrack_debugMsg_flag;					//������Ϣ����ȼ�
	int stuTrack_Draw_flag;						//�Ƿ���ƽ��
	int stuTrack_direct_standard[4];			//�ĸ�����λ����ֱ������ͼ���еĽǶ�
	int stuTrack_stuWidth_standard[4];			//�ĸ�����λ��ѧ����ͼ������ռ�Ŀ��
	int stuTrack_direct_range;					//����ʱ����ĽǶ�ƫ�뷶Χ
	int stuTrack_standCount_threshold;			//�ж�Ϊ������֡����ֵ
	int stuTrack_sitdownCount_threshold;		//�ж�Ϊ���µ�֡����ֵ
	int stuTrack_moveDelayed_threshold;			//�ƶ�Ŀ�걣�ָ��ٵ���ʱ���������ʱ�����˶������������(��λ������)
	int stuTrack_deleteTime_threshold;			//Ŀ�걣�ָ��ٵ��ʱ��ʱ���������ʱ��ɾ��Ŀ��(��λ����)
	double stuTrack_move_threshold;				//�ж����ƶ�Ŀ���ƫ����ֵ����ֵ��
	double stuTrack_standup_threshold;			//�ж�������Ŀ���ƫ����ֵ����ֵ��
	TrackPrarms_Point_t stuTrack_vertex[4];		//ѧ�������ĸ�����λ��

	double transformationMatrix[9];				//ͼ����������̨����ı任����
	double stretchingAB[2];						//����ϵ��
}StuITRACK_ClientParams_t;

//---------------------------------------------------------------------����������


//-----------------------------------------------------------------------

#define RESULT_STUTRAKC_NULL_CAMERA				0			//��ʾ���Խ���ѧ��������
#define RESULT_STUTRAKC_MOVE_CAMERA				1			//�ƶ���ͷ
#define RESULT_STUTRAKC_FEATURE_CAMERA			2			//��ѧ����д
#define RESULT_STUTRAKC_PAN0RAMA_CAMERA			4			//ѧ��ȫ��
#define RESULT_STUTRAKC_noTCH_FEATURE_CAMERA	8			//��Ҫ��ʦ���ڽ�̨��������д
#define RESULT_STUTRAKC_noTCH_PAN0RAMA_CAMERA	16			//��Ҫ��ʦ�޶���ʱ��ȫ��

typedef struct _StuITRACK_OutParams
{
	int result_flag;						//��ǰ��Ҫ��״̬
	TrackPrarms_Point_t position;			//Ŀ��λ��
	int stretchingCoefficient;				//����ϵ��
}StuITRACK_OutParams_t;

#ifdef  __cplusplus  
}
#endif  /* end of __cplusplus */ 

#endif
