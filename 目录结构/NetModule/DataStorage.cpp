///////////////////////////////////////////////////////////////////////////
//		 DataStorage.cpp
///////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "DataStorage.h"
#include "custom_log.h"

#define VIDEO_BUFFER_SIZE		1024*1024*10
//BYTE g_cbVideoBuffer[VIDEO_BUFFER_SIZE];
#define CopyMemory	memcpy
//////////////////////////////////////////////////////////////////////////

//构造函数
CDataStorage::CDataStorage(void)
{
	m_dwDataSize=0;
	m_dwBufferSize=0;
	m_dwInsertPos=0;
	m_dwTerminalPos=0;
	m_dwDataQueryPos=0;
	m_dwDataPacketCount=0;
	m_pDataStorageBuffer=NULL;
}

//析构函数
CDataStorage::~CDataStorage(void)
{
	SafeDeleteArray(m_pDataStorageBuffer);
	return;
}

//负荷信息
bool CDataStorage::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	//设置变量
	BurthenInfo.dwDataSize=m_dwDataSize;
	BurthenInfo.dwBufferSize=m_dwBufferSize;
	BurthenInfo.dwDataPacketCount=m_dwDataPacketCount;

	return true;
}

//分配内存
bool CDataStorage::RenewDataBuffer(WORD wDataSize)
{
	try
	{
		//申请内存
		DWORD dwNewBufferSize=m_dwBufferSize+(wDataSize+sizeof(tagDataHead))*2L;
		BYTE * pNewQueueServiceBuffer=new BYTE [dwNewBufferSize];
		if (pNewQueueServiceBuffer==NULL) return false;

		//拷贝数据
		if (m_pDataStorageBuffer!=NULL) 
		{
			ASSERT(m_dwTerminalPos>=m_dwDataSize);
			ASSERT(m_dwTerminalPos>=m_dwDataQueryPos);
			DWORD dwPartOneSize=m_dwTerminalPos-m_dwDataQueryPos;
			if (dwPartOneSize>0L) CopyMemory(pNewQueueServiceBuffer,m_pDataStorageBuffer+m_dwDataQueryPos,dwPartOneSize);
			if (m_dwDataSize>dwPartOneSize) 
			{
				ASSERT((m_dwInsertPos+dwPartOneSize)==m_dwDataSize);
				CopyMemory(pNewQueueServiceBuffer+dwPartOneSize,m_pDataStorageBuffer,m_dwInsertPos);
			}
		}

		//调整变量
		m_dwDataQueryPos=0L;
		m_dwInsertPos=m_dwDataSize;
		m_dwTerminalPos=m_dwDataSize;
		m_dwBufferSize=dwNewBufferSize;
		SafeDeleteArray(m_pDataStorageBuffer);
		m_pDataStorageBuffer=pNewQueueServiceBuffer;

		return true;
	}
	catch (...)
	{}

	return false; 
}

//插入数据
bool CDataStorage::AddData(WORD wIdentifier,WORD wChannelID,DWORD dwTime, void * const pBuffer, WORD wDataSize)
{
	//初始化数据
	tagDataHead DataHead;
	DataHead.wDataSize=wDataSize;
	DataHead.wIdentifier=wIdentifier;	
//	DataHead.wChannelID=wChannelID;
//	DataHead.dwTimeSpan=dwTime;
	DWORD dwCopySize=sizeof(tagDataHead)+wDataSize;

	//效验缓冲区
	if ((m_dwDataSize+dwCopySize)>m_dwBufferSize)
	{
		if(!RenewDataBuffer(wDataSize)) return false;
	}
	else if((m_dwInsertPos==m_dwTerminalPos)&&((m_dwInsertPos+dwCopySize)>m_dwBufferSize)) 
	{
		if (m_dwDataQueryPos>=dwCopySize) 
		{
			m_dwInsertPos=0;
		}
		else 
		{
			if(!RenewDataBuffer(wDataSize))
				return false;
		}
	}
	else if ((m_dwInsertPos<m_dwTerminalPos)&&((m_dwInsertPos+dwCopySize)>m_dwDataQueryPos)) 
	{
		if(!RenewDataBuffer(wDataSize))
			return false;
	}	
	 
	try
	{
		//拷贝数据
		CopyMemory(m_pDataStorageBuffer+m_dwInsertPos,&DataHead,sizeof(tagDataHead));
		if (wDataSize>0) 
		{
			ASSERT(pBuffer!=NULL);
			CopyMemory(m_pDataStorageBuffer+m_dwInsertPos+sizeof(tagDataHead),pBuffer,wDataSize);
		}

		//调整数据
		m_dwDataPacketCount++;
		m_dwDataSize+=dwCopySize;
		m_dwInsertPos+=dwCopySize;
		m_dwTerminalPos=__max(m_dwTerminalPos,m_dwInsertPos);

		return true;
	}
	catch (...) { }

	return false;
}

//获取数据
bool CDataStorage::GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize)
{
	//效验变量
	if (m_dwDataSize==0L) return false;
	if (m_dwDataPacketCount==0L) return false;

	//调整参数
	if (m_dwDataQueryPos==m_dwTerminalPos)
	{
		m_dwDataQueryPos=0L;
		m_dwTerminalPos=m_dwInsertPos;
	}

	//获取数据指针
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+sizeof(tagDataHead)));
	BYTE *pSrcData=(BYTE *)(m_pDataStorageBuffer+m_dwDataQueryPos);
	tagDataHead * pDataHead=(tagDataHead *)pSrcData;
	ASSERT(wBufferSize>=pDataHead->wDataSize);

	//获取包大小
	WORD wPacketSize=sizeof(tagDataHead)+pDataHead->wDataSize;
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+wPacketSize));

	//判断缓冲
	WORD wCopySize=0;
	ASSERT(wBufferSize>=pDataHead->wDataSize);
	if (wBufferSize>=pDataHead->wDataSize) wCopySize=pDataHead->wDataSize;

	//拷贝数据
//	DataHead.dwTimeSpan=pDataHead->dwTimeSpan;
//	DataHead.wChannelID=pDataHead->wChannelID;
	DataHead.wDataSize=pDataHead->wDataSize;
	DataHead.wIdentifier=pDataHead->wIdentifier;
	if (DataHead.wDataSize>0)
	{
		pSrcData+=sizeof(tagDataHead);
		if (wBufferSize<pDataHead->wDataSize) DataHead.wDataSize=0;
		else CopyMemory(pBuffer,pSrcData,DataHead.wDataSize);
	}

	//效验参数
	//LOGE(TEXT("------DataStorage----GetData wPacketSize:%d,m_dwDataSize:%d,BufferSize:%d"),wPacketSize,m_dwDataSize,m_dwBufferSize);
	ASSERT(wPacketSize<=m_dwDataSize);
	ASSERT(m_dwBufferSize>=(m_dwDataQueryPos+wPacketSize));

	//设置变量
	m_dwDataPacketCount--;
	m_dwDataSize-=wPacketSize;
	m_dwDataQueryPos+=wPacketSize;

	return true;
}

//删除数据
void CDataStorage::RemoveData(bool bFreeMemroy)
{
	//设置变量
	m_dwDataSize=0;
	m_dwInsertPos=0;
	m_dwTerminalPos=0;
	m_dwDataQueryPos=0;
	m_dwDataPacketCount=0;

	//删除内存
	if (bFreeMemroy==true) 
	{
		m_dwBufferSize=0;
		SafeDeleteArray(m_pDataStorageBuffer);
	}

	return;
}
//////////////////////////////////////////////////////////////////////////
