// ////////////////////////////////////////////////////////////////////////////////////////////////////
//      线程管理服务类头文件  ServiceThread.h
// ////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_SERVICE_THREAD_HEAD_FILE__
#define __DEIRLYM_SERVICE_THREAD_HEAD_FILE__

#include <pthread.h>
#include <semaphore.h>
#include "IUnknownEx.h"
// /////////////////////////////////////////////////////////////////////////////////////////////////////
// define interface

// ThreadLock interface
class IThreadLock
{
public:
     //Lock Function
     virtual void Lock()=0;
     // Unlock Function
     virtual void UnLock()=0;
};
//Thread interface
class IServiceThread
{
public:
     // Check Thread State
     virtual bool IsRuning()=0;
     //Start thread
     virtual bool StartThread()=0;
     //Stop thread
     virtual bool StopThread(DWORD dwWait=INFINITE)=0;
     //Kill Thread
     virtual bool TerminateThread()=0;
     //Set Thread state
     virtual bool SetState(bool bState=false)=0;
};

// Mutex Lock class
class CThreadLock:public IThreadLock
{
     // Value define
private:
     pthread_mutex_t                   m_tmLock;                      // Mutex Value
     
     // Function define
public:     
     inline CThreadLock(){pthread_mutex_init(&m_tmLock,NULL);}
     inline virtual ~CThreadLock(){pthread_mutex_destroy(&m_tmLock);}
     
public:
     // Lock Function
     virtual inline void Lock(){pthread_mutex_lock(&m_tmLock);}   
     //Unlock Function
     virtual inline void UnLock(){pthread_mutex_unlock(&m_tmLock);}          
};

// Safe Lock Handle class
class CThreadLockHandle
{
private:
     int                                                  m_nLockCount;                             //Lock number
     IThreadLock                                          *m_pIThreadLock;                          //Lock object
     
     //Function define
public:
     CThreadLockHandle(IThreadLock * pIThreadLock,bool bAutoLock=true); 
     virtual ~CThreadLockHandle();
     
public: 
     // Lock Function
     void Lock();
     //UnLock Function    
     void UnLock();
     //Get Lock Count
     int inline GetLockCount(){return m_nLockCount;}
};

// 类声明
class  CServiceThread: public IServiceThread
{     
     //变量申明
protected:
     pthread_t                                 m_tThreadID;                                 //线程ID
     volatile bool                              m_bRun;                                        //运行状 态
     volatile bool                              m_bReturn;                                   //Return flags
          
     //函数定义
public:
     //构造函数
     CServiceThread();
     //析构函数
     virtual ~CServiceThread();
     
     //接口定义
public:
     // Get Thread State
     virtual bool IsRuning();
     // Start Thread
     virtual bool StartThread();
     // Stop Thread
     virtual bool StopThread(DWORD dwWait=1); 
     //Kill Thread
     virtual bool TerminateThread();
     //Set Thread state
     virtual bool SetState(bool bState=false){m_bRun=bState;return true;}
     //SuspendThread
     virtual bool SuspendThreads();
     //ResumeThread
     virtual bool ResumeThreads();
     
public:
     //Get Thread ID
     pthread_t GetThreadID(){return m_tThreadID;}    
          
     //Event Function
protected:
     //开始通知函数
     virtual bool OnThreadStartEvent(){return true;}
     //线程停止通知函数
     virtual bool OnThreadStopEvent(){return true;}
     
     //Inside Function
protected:
     //线程体函数
     virtual bool RepetitionRun()=0;
     //Thread Function
     static void * ThreadFunction(void * pParam);
};

#endif //__DEIRLYM_SERVICE_THREAD_HEAD_FILE__

