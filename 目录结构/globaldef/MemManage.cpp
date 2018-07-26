// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Memory manager class implement file
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MemManage.h"

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //distibute memory
CMyPlex * CMyPlex::Create(CMyPlex *&pHead,UINT nMax,UINT nElementSize)
{
     size_t nSize=sizeof(CMyPlex)+nMax*nElementSize;
     BYTE *pMem=(BYTE *)malloc(nSize);
 
     if(pMem==NULL) return NULL;     
     CMyPlex * p=(CMyPlex * )pMem;
     //Add new memory to link
     p->pNext=pHead;
     pHead=p;
     return p;
}
 //Free all memory
void CMyPlex::FreeDataChain()
{
     CMyPlex * p=this;
     while(p!=NULL)     
     {
          BYTE *pBytes=(BYTE *)p;
          CMyPlex * pNext=p->pNext;
          free(pBytes);
          p=pNext;
     }
}
