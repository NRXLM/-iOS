// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Service thread class file ServiceThread.cpp
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dispatch/dispatch.h>
#include <dispatch/semaphore.h>
#include "MacThread.h"

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Start Parameter
struct tagThreadParameter
{
     bool                   bSuccess;                                     //Is Error
     dispatch_semaphore_t   hSem_t;                                        //Event Handle
     CServiceThread         *pServiceThread;                          //Thread point
};

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
CThreadLockHandle::CThreadLockHandle(IThreadLock * pIThreadLock,bool bAutoLock)
{
     assert(pIThreadLock!=NULL);
     m_nLockCount=0;
     m_pIThreadLock=pIThreadLock;
     if(bAutoLock) Lock();
}
CThreadLockHandle::~CThreadLockHandle()
{
     while(m_nLockCount>0) UnLock();
}
// Lock Function
 void CThreadLockHandle::Lock()
{
    assert(m_nLockCount>=0);    
    assert(m_pIThreadLock!=NULL);
         
     //if(m_pIThreadLock==NULL) return;
     //Lock object
     m_nLockCount++;
     m_pIThreadLock->Lock();
}
//UnLock Function
void CThreadLockHandle::UnLock()
{
     assert(m_nLockCount>0);
     assert(m_pIThreadLock!=NULL);
     //if(m_nLockCount<=0 || m_pIThreadLock==NULL) return;
     
     //UnLock object
     m_nLockCount--;
     m_pIThreadLock->UnLock();
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////
CServiceThread::CServiceThread()
{
     m_bRun=false;
     m_tThreadID=0;
     m_bReturn=true;
}

CServiceThread::~CServiceThread()
{
     StopThread(INFINITE);
}
bool CServiceThread::IsRuning()
{
     return !m_bReturn; 
}
// Start Thread
bool CServiceThread::StartThread()
{
    if(IsRuning())      return false;
    if(m_tThreadID!=0)
    {
        m_tThreadID=0;          
    }

    tagThreadParameter threadParam;     
    threadParam.bSuccess=false;
    threadParam.pServiceThread=this;
    threadParam.hSem_t=0;
    threadParam.hSem_t=dispatch_semaphore_create(0);
    if(threadParam.hSem_t==0)
    {
        printf("Start thread faild,error is [%s]\n",strerror(errno));
        return false;
    }

    m_bRun=true;     
    int nRet=pthread_create(&m_tThreadID,NULL,ThreadFunction,&threadParam);    
    if(nRet!=0) return false;     
    //Wait thread finished
    dispatch_time_t timeout=dispatch_time(DISPATCH_TIME_FOREVER,0);
    dispatch_semaphore_wait(threadParam.hSem_t,timeout);
    if(threadParam.bSuccess==false)     
    {
        StopThread();
        return false;
    }
    m_bReturn=false;
    return true;
}
// Stop Thread
bool CServiceThread::StopThread(DWORD dwWait)
{
     if(IsRuning()==true)
     {
          m_bRun=false;
          sleep(dwWait);
          int nRes=pthread_join(m_tThreadID,NULL);
          if(nRes!=0)
               return false;
     }
     if(m_tThreadID!=0)
     {
          m_tThreadID=0;
     }
     return true;
}
//Kill Thread
bool CServiceThread::TerminateThread()
{
     if(IsRuning()==true)               
     {
          pthread_cancel(m_tThreadID);
     } 
     if(m_tThreadID!=0)
          m_tThreadID=0;
     
     return true;    
}
//SuspendThread
bool CServiceThread::SuspendThreads()
{
    if(m_bRun==false) return true;
    pthread_kill(m_tThreadID, SIGSTOP);
    return true;
}
//ResumeThread
bool CServiceThread::ResumeThreads()
{
    if(m_bRun==false) return true;
    pthread_kill(m_tThreadID, SIGCONT);
    return true;
}
  //Thread Function
void * CServiceThread::ThreadFunction(void * pParam)
{     
     assert(pParam!=NULL);
     tagThreadParameter *pThreadParam=(tagThreadParameter*)pParam;
     CServiceThread *pThread=pThreadParam->pServiceThread;
     
     try
     {
          pThreadParam->bSuccess=pThread->OnThreadStartEvent();   
     }          
     catch(...)
     {
          pThreadParam->bSuccess=false;          
     }
     //Set param
     bool bSuccess= pThreadParam->bSuccess;
     dispatch_semaphore_signal(pThreadParam->hSem_t);
     //runing
     if(bSuccess==true)
     {
          while(pThread->m_bRun)
          {
               try
               {
                    if(pThread->RepetitionRun()==false) break;
               }
               catch(...){}
          }
     }
     //Stop Event
     try
     {
          pThread->OnThreadStopEvent();
     }
     catch(...){}
     pThread->m_bReturn=true;
     pthread_exit(NULL);
}


