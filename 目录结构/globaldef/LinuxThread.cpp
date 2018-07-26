// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Service thread class file ServiceThread.cpp
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "LinuxThread.h"

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Start Parameter
struct tagThreadParameter
{
     bool                                    bSuccess;                                     //Is Error
     sem_t                                 hSem_t;                                        //Event Handle
     CServiceThread                *pServiceThread;                          //Thread point
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
     if(m_tThreadID>0)
     {
          m_tThreadID=0;          
     }
     
     tagThreadParameter threadParam;     
     threadParam.bSuccess=false;
     threadParam.pServiceThread=this;
     int nRet=sem_init(&threadParam.hSem_t,0,0);
     if(nRet!=0) return false;
     
     m_bRun=true;     
     nRet=pthread_create(&m_tThreadID,NULL,ThreadFunction,&threadParam);    
     if(nRet!=0) return false;     
     //Wait thread finished
     sem_wait(&threadParam.hSem_t); 
     sem_destroy(&threadParam.hSem_t); 
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
     if(m_tThreadID>0)
     {
          m_tThreadID=0;
     }
     return true;
}
//Kill Thread
bool CServiceThread::TerminateThread()
{          
    if(m_bRun==false) return true;
    pthread_kill(m_tThreadID, SIGSTOP);
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
     sem_post(&pThreadParam->hSem_t);
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


