//////////////////////////////////////////////////////////////////////////////////////////
//		本地数据定义头文件 LinuxEvent.h
//////////////////////////////////////////////////////////////////////////////////////////
#define MACIOS	1

#include "LinuxEvent.h"
#if MACIOS
#include <dispatch/dispatch.h>
#include <dispatch/semaphore.h>
#else
#define ANDROID_OS
#endif

#define CONDWAIT	0
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagLinuxEvent
{
	#ifdef ANDROID_OS
	#if !CONDWAIT
	sem_t						m_semt;
	#else
	pthread_cond_t				cond;
	pthread_mutex_t				mutex;
	#endif	
	#else
	dispatch_semaphore_t		m_semt;
	#endif
}LINUXEVENT;

HANDLE CreateEvent()
{
	LINUXEVENT *pEvent=new LINUXEVENT;
	if(pEvent==NULL) return NULL;
	memset(pEvent,0,sizeof(LINUXEVENT));
	#ifdef ANDROID_OS
	#if !CONDWAIT
	int nRet=sem_init(&pEvent->m_semt,0,0);
    if(nRet!=0)
	{
		delete pEvent;
		return NULL;
	}
	#else
	pthread_mutex_init(&pEvent->mutex, NULL);
	pthread_cond_init(&pEvent->cond, NULL);
	#endif
	#else
	pEvent->m_semt=dispatch_semaphore_create(0);
	if(pEvent->m_semt==NULL)
	{
		delete pEvent;
		return NULL;
	}
	#endif

	return pEvent;
}

//激活事件
void SetEvent(HANDLE hHandle)
{
	LINUXEVENT *pEvent=(LINUXEVENT *)hHandle;
	if(pEvent==NULL) return;
	#ifdef ANDROID_OS
	#if !CONDWAIT
	sem_post(&pEvent->m_semt);
	#else
	pthread_mutex_lock(&pEvent->mutex);
	pthread_cond_signal(&pEvent->cond);  
	pthread_mutex_unlock(&pEvent->mutex); 
	#endif
	#else
	dispatch_semaphore_signal(pEvent->m_semt);
	#endif
}

//等待事件
DWORD WaitForSingleObject(HANDLE hHandle,DWORD dwMilliseconds)
{
	LINUXEVENT *pEvent=(LINUXEVENT *)hHandle;
	if(pEvent==NULL) return -2;
	int nRet=0;
	#ifdef ANDROID_OS
	#define NSECTOSEC 1000000000
	#if CONDWAIT
	struct timeval now;  
	struct timespec outtime;	
	gettimeofday(&now, NULL);
	now.tv_usec+=dwMilliseconds*1000;
    outtime.tv_sec = now.tv_sec;  
    outtime.tv_nsec =now.tv_usec*1000;
	outtime.tv_sec+=outtime.tv_nsec/NSECTOSEC;
	outtime.tv_nsec=outtime.tv_nsec%NSECTOSEC;
	//pthread_mutex_lock(&pEvent->mutex);
    pthread_cond_timedwait(&pEvent->cond,&pEvent->mutex,&outtime);  
	//pthread_mutex_unlock(&pEvent->mutex);
	#else	
	struct timespec ts;
	if(clock_gettime(CLOCK_REALTIME,&ts)<0)
		return -3;
	DWORD dwSec=dwMilliseconds/1000;
	DWORD dwNsec=(dwMilliseconds-dwSec*1000)*1000000;	
 
	dwNsec=ts.tv_nsec+dwNsec;
	dwSec=ts.tv_sec + dwSec+dwNsec/NSECTOSEC;
	dwNsec%=NSECTOSEC;
	ts.tv_sec = dwSec;
    ts.tv_nsec = dwNsec;
	while((nRet=sem_timedwait(&pEvent->m_semt,&ts))==-1 && errno == EINTR)
	{
		ts.tv_sec = dwSec;
		ts.tv_nsec = dwNsec;
		continue;
	}
	if(nRet==-1)
	{
		nRet=errno;
	}
	#endif
	#else
	dispatch_time_t timout=dispatch_time(DISPATCH_TIME_NOW,dwMilliseconds*1000000);
	dispatch_semaphore_wait(pEvent->m_semt,timout);
	#endif

	return nRet;
}

//关闭事件句柄
void CloseHandle(HANDLE hHandle)
{
	LINUXEVENT *pEvent=(LINUXEVENT *)hHandle;
	if(pEvent==NULL) return;
	#ifdef ANDROID_OS
	#if !CONDWAIT
	sem_destroy(&pEvent->m_semt);
	#else
	pthread_mutex_destroy(&pEvent->mutex);
	pthread_cond_destroy(&pEvent->cond);
	#endif
	#endif
	delete pEvent;
}

