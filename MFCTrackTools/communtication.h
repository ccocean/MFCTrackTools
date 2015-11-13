#ifndef _COMMUNTICATION_H__
#define _COMMUNTICATION_H__
#include "pthread.h"
#include "share_socket.h"
typedef enum{
	CLIENT_TO_SERVER = 0,
	REPEAT_CLIENT_TO_SERVER,
	SERVER_TO_CLIENT,
	REPEAT_SERVER_TO_CLIENT
}CMD_DIRECTION;

#define HEARTBIT_CMD 0x99
typedef enum{	
	RET_CMD_SUCCESS,
	RET_ERROR_STRUCT_LEN,
	RET_ERROR_INFO,
	RET_UNKNOWN_CMD
}CMD_RETURN_CODE;

typedef struct COMMUNTICATION_HEAD_S{
	unsigned char  check_start[4]; /*$$$$,标示符*/
	unsigned int   identifier;     /*区分从那个client到那个server*/
	unsigned int   total_len;     /*后面荷载的总长度*/
	unsigned int   struct_len;    /*真实结构体的长度 《= total_len*/
	unsigned int   cmd;     /*case */
	unsigned int   return_code;   /*返回值*/
	unsigned int   seq_num;       /*序列值*/
	CMD_DIRECTION direction;
	unsigned int   check_end[4]; /*####,标示符*/
}Communtication_Head_t;


typedef enum {
	NO_INIT_STATUS = 0,
//	BEGIN_START_STATUS = 1,
	START_STATUS =1,
	BEGIN_STOP_STATUS = 2,
	STOP_STATUS
}COMMUNTICATION_STATUS;
typedef enum
{
	CONNECT_SUCCESS = 0,
	CONNECT_FAIL,
	DISCONNECT_SUCCESS,
}Connect_Status;
typedef  struct _COMMUTICATION_HANDLE * Commutication_Handle_t;

typedef int (*DealCmdFunc)(Communtication_Head_t *,void * ,Commutication_Handle_t , void*);
typedef int (*getheartbitvalue)(char *,int *, void*);
typedef int (*DealheartbitFunc)(char *, void*);
typedef int(*ConnectStatuscall)(Connect_Status, void*);


typedef struct _COMMUTICATION_HANDLE{

	pthread_mutex_t lock;     //互斥锁
	int status; //当前状态 连接中，断连中
	unsigned int seq_num;  //累加值，表示第几条信令。
//	unsigned int server_socket ;  //server socket
	 int client_socket;
	char ip[16];  /*server ,ip表示本地ip, client,ip表示目的地IP*/
	int  port;    /*server,port表示本地监听port, client ,port表示目的的port*/
//	int (*DealCmdFunc)(Communtication_Head_t *head,char *buf ,Commutication_Handle_t handle);
//	int (*getheartbitvalue)(char *buf,int *len);
	DealCmdFunc DealCmdFuncPtr;
	getheartbitvalue GetHeartbitvaluePtr;
	DealheartbitFunc DealHeartbitFuncPtr;
	ConnectStatuscall ConnectStatusPtr;
	void* param;//由用户自己来管理 通过回调抛上来
}Communtication_Handle_t;




#define DEFAULT_CHECK_START_CODE '$'
#define DEFAULT_CHECK_END_CODE   '#'

#define COMMUNTICATION_HEAD_LEN  sizeof(Communtication_Handle_t)
#define DEFATULT_COMMUTICATION_TOTAL_LEN      1024

Commutication_Handle_t communtication_create_clientHandle(char *dst_ip, unsigned short dst_port, 
	DealCmdFunc func1, DealheartbitFunc func2, ConnectStatuscall func3, void* param);
int communtication_send_clientMsg(Communtication_Head_t *head, char *date, int buflen, Communtication_Handle_t *handle);
int communtication_set_handleStatus(Commutication_Handle_t handle, int status);

int commutication_init_head(Communtication_Head_t *head, int identifier);

#endif


