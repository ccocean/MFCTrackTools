#ifndef __HI_TYPE_H__
#define __HI_TYPE_H__

#ifndef WIN32
#define WIN32
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef unsigned char           HI_U8;
typedef unsigned char           HI_UCHAR;
typedef unsigned short          HI_U16;
typedef unsigned int            HI_U32;

typedef signed char             HI_S8;
typedef short                   HI_S16;
typedef int                     HI_S32;

#ifndef _M_IX86
typedef unsigned long long      HI_U64;
typedef long long               HI_S64;
#else
typedef __int64                   HI_U64;
typedef __int64                   HI_S64;
#endif

typedef char                    HI_CHAR;
typedef char*                   HI_PCHAR;

typedef float                   HI_FLOAT;
typedef double                  HI_DOUBLE;
typedef void                    HI_VOID;

typedef unsigned long           HI_SIZE_T;
typedef unsigned long           HI_LENGTH_T;
typedef unsigned long           HI_ULONG;
typedef long					HI_LONG;

typedef enum {
    HI_FALSE    = 0,
    HI_TRUE     = 1,
} HI_BOOL;

#ifndef NULL
#define NULL             0L
#endif
#define HI_NULL          0L
#define HI_NULL_PTR      0L

#define HI_SUCCESS          0
#define HI_FAILURE          (-1)


#define SAFE_DELETE(p)  { if((p) != NULL) { delete (p);     (p) = NULL; } }
#define SAFE_DELETEA(p) { if((p) != NULL) { delete[] (p);   (p) = NULL; } }
#define SAFE_RELEASE(p) { if((p) != NULL) { (p)->Release(); (p) = NULL; } }
#define SAFE_FREE(p)    { if((p) != NULL) { free(p);        (p) = NULL; } }

///#define hi_sdk_sleep(s)    sleep(s)

#ifdef WIN32

#include <winsock2.h>
#include <windows.h>
#define pthread_t HANDLE
#define pthread_mutex_t HANDLE
#define pthread_attr_t	SECURITY_ATTRIBUTES
#define sem_t	HANDLE
typedef int socklen_t;
typedef __int64		int64;

#define useconds_t HI_U32
#define 	fopen64 fopen
#define     fseeko64 fseek
#define     fgetpos64 fgetpos
#define     ftello64	ftell
typedef HANDLE pthread_t;

//int pthread_create(pthread_t *thread,  const  pthread_attr_t *attr, void* (*start_routine)(void*),void *arg);
//int pthread_join(pthread_t thread, void **value_ptr);
//void pthread_exit(void *value_ptr);
//int pthread_cancel(pthread_t thread);
int usleep(int useconds);
int gettimeofday(struct timeval *tp,void *);
//int pthread_detach(pthread_t thread);
//pthread_t pthread_self(void);

void bzero(void *s, int n);

#define  pthread_mutex_init(A, B)   InitializeCriticalSection(A) 
#define  pthread_mutex_lock(A)   (EnterCriticalSection(A),0) 
#define  pthread_mutex_unlock(A)   LeaveCriticalSection(A) 
#define  pthread_mutex_destroy(A)   DeleteCriticalSection(A) 
#define  pthread_handler_decl(A, B)   unsigned __cdecl A(void *B) 

#if 0
int
write(
	  IN SOCKET s,
	  __in_bcount(len) const char FAR * buf,
	  IN int len
	  );
#endif

#undef hi_sdk_sleep
#define hi_sdk_sleep(s)    Sleep(s*1000)

#else
#include <pthread.h>
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_TYPE_H__ */

