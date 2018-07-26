// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Memory manager class head file MemManage.h
//   hupo      2008-1-10
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_MEMORY_MANGER_HEAD_FILE__
#define __DEIRLYM_MEMORY_MANGER_HEAD_FILE__

#ifdef _WIN32
#include "ServiceThread.h"
#else
#if MACIOS
#include "MacThread.h"
#else
#include "LinuxThread.h"
#endif
#endif
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Memory manager struct
struct CMyPlex
{
     CMyPlex  *pNext;                                         //Next note point
     void * data(){return this+1;}                   //reality data point
     //distibute memory
     static CMyPlex * Create(CMyPlex *&pHead,UINT nMax,UINT nElementSize);
     //Free all memory
     void FreeDataChain();
};

//Memory manage class
template <class TYPE,class ARG_TYPE=const TYPE&>
class CMemManage
{
     //struct define
public:
     struct CAssoc
     {
          CAssoc                          *pNext;                                 //Next struct node
          TYPE                               Data;                                   //Struct Data          
     };
     
     //variable define
protected:
     struct CMyPlex                     *m_pBlocks;                        //Memory block head point
     UINT                                       m_nBlockCount;                  //Memory block count
     UINT                                       m_nCount;                          //Used CAssoc count
     CAssoc                                  *m_pFreeList;                      //Free CAssoc list
     
protected:     
     CThreadLock                        m_Lock;                               //synchronization lock
     
     //function define
public:
     //construct function
     CMemManage(UINT nBlockCount=500);
     virtual ~CMemManage();
     //Get Used elments count
     UINT GetCount() const{return m_nCount;}
     //Get memory block node number
     UINT GetBlockCount() const{return m_nBlockCount;}     
     //initialize function
     bool InitMemory(UINT nBlockCount,bool bAllocNow=true);
     //Get a CAssoc point
     TYPE * NewAssoc();
     //Free a CAssoc point
     void FreeAssoc(TYPE * pData);
     //Free memory point
     void FreeProint();
     //Clare all node point
     bool ClearAll();    
     //设置内存块个数
	 bool SetMemBlockCount(UINT nBlockCount){m_nBlockCount=nBlockCount;return true;}
};

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// construct function
template <class TYPE,class ARG_TYPE>
CMemManage<TYPE,ARG_TYPE>::CMemManage(UINT nBlockCount)
{
     m_pBlocks=NULL;
     m_nBlockCount=nBlockCount;
     m_nCount=0;   
     m_pFreeList=NULL;  
}
template <class TYPE,class ARG_TYPE>      
CMemManage<TYPE,ARG_TYPE>::~CMemManage()
{
     m_nCount=0;
     m_pFreeList=NULL;
     
     if(m_pBlocks)
          m_pBlocks->FreeDataChain();
     m_pBlocks=NULL;
}
//Free memory point
template <class TYPE,class ARG_TYPE>
void CMemManage<TYPE,ARG_TYPE>::FreeProint()
{
     CThreadLockHandle LockHandle(&m_Lock);
     CMyPlex * p=m_pBlocks;
     while(p!=NULL)
     {
          CAssoc * pAssoc=(CAssoc *)p->data();
          UINT i;
          for(i=0;i<m_nBlockCount;i++,pAssoc++)
          {
               try
               {
                    pAssoc->Data.~TYPE();
               }
               catch(...){}
          }
          
          CMyPlex * pNext=p->pNext;
          p=pNext;
     }
}

// initialize function
template <class TYPE,class ARG_TYPE>
bool CMemManage<TYPE,ARG_TYPE>::InitMemory(UINT nBlockCount,bool bAllocNow)
{
     CThreadLockHandle LockHandle(&m_Lock);
     m_nBlockCount=nBlockCount;
     if(bAllocNow)
     {
          CMyPlex * pNewBlock=CMyPlex::Create(m_pBlocks,m_nBlockCount,sizeof(CAssoc));
          ASSERT(pNewBlock!=NULL);
          if(pNewBlock==NULL) return false;
          CAssoc * pAssoc=(CAssoc *)pNewBlock->data();
          memset(pAssoc,0,m_nBlockCount*sizeof(CAssoc));
          pAssoc+=m_nBlockCount-1;
          INT i;
          for(i=m_nBlockCount-1;i>=0;i--,pAssoc--)
          {
               pAssoc->pNext=m_pFreeList;
               m_pFreeList=pAssoc;
               //printf("Add node to list \n");               
          }
     }
          
     return true;
}
// Get a free point
template <class TYPE,class ARG_TYPE>
TYPE * CMemManage<TYPE,ARG_TYPE>::NewAssoc()
{
     CThreadLockHandle LockHandle(&m_Lock);
     if(m_pFreeList==NULL)
     {
          CMyPlex * pNewBlock=CMyPlex::Create(m_pBlocks,m_nBlockCount,sizeof(CAssoc));
          ASSERT(pNewBlock!=NULL);
          if(pNewBlock==NULL) return NULL;
          CAssoc * pAssoc=(CAssoc *)pNewBlock->data();
          memset(pAssoc,0,m_nBlockCount*sizeof(CAssoc));
          pAssoc+=m_nBlockCount-1;
          INT i;
          for(i=m_nBlockCount-1;i>=0;i--,pAssoc--)
          {
               pAssoc->pNext=m_pFreeList;
               m_pFreeList=pAssoc;
          }          
     }
     CAssoc * pAssoc=m_pFreeList;
     m_pFreeList=m_pFreeList->pNext;
     m_nCount++;
     return &pAssoc->Data;
}
//Free a point
template <class TYPE,class ARG_TYPE>
void CMemManage<TYPE,ARG_TYPE>::FreeAssoc(TYPE * pData)
{
     ASSERT(pData);
     CThreadLockHandle LockHandle(&m_Lock);     
     memset(pData,0,sizeof(TYPE));
     CAssoc * pAssoc=NULL;
     pAssoc=CONTAINING_RECORD(pData,CAssoc,Data);
     ASSERT(pAssoc);     
     
     //add to free list
     pAssoc->pNext=m_pFreeList;
     m_pFreeList=pAssoc;
     m_nCount--;
}          
//clear all node
template <class TYPE,class ARG_TYPE>
bool CMemManage<TYPE,ARG_TYPE>::ClearAll()
{
     CThreadLockHandle LockHandle(&m_Lock);
     m_pFreeList=NULL;
     CMyPlex * p=m_pBlocks;
     while(p!=NULL)
     {
          CAssoc * pAssoc=(CAssoc *)p->data();
          ASSERT(pAssoc!=NULL);
          pAssoc+=m_nBlockCount-1;
          INT i;
          for(i=m_nBlockCount-1;i>=0;i--,pAssoc--)
          {
               pAssoc->pNext=m_pFreeList;
               m_pFreeList=pAssoc;
          }
          CMyPlex * pNext=p->pNext;
          p=pNext;
     }
     return true;
}

#endif //__DEIRLYM_MEMORY_MANGER_HEAD_FILE__
