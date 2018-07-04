/////////////////////////////////////////////////////////////////////////////
//	数据存储类头文件 DataStorage.h
////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_DATA_STORAGE_HEAD_FILE__
#define __DEIRLYM_DATA_STORAGE_HEAD_FILE__

#include "localdef.h"

////////////////////////////////////////////////////////////////////////////
//struct tagDataHead
//{
//	WORD							wIdentifier;
//	WORD							wChannelID;
//	WORD							wDataSize;
//	DWORD							dwTimeSpan;
//};
//
//struct tagDataBuffer
//{
//	WORD							wDataSize;
//	WORD							wBufferSize;
//	BYTE							*pDataBuffer;
//};

//////////////////////////////////////////////////////////////////////////
//数据存储类
class CDataStorage
{
	//数据变量
protected:
	DWORD							m_dwDataSize;						//数据大小
	DWORD							m_dwBufferSize;						//缓冲区长度
	DWORD							m_dwInsertPos;						//数据插入位
	DWORD							m_dwTerminalPos;					//数据结束位
	DWORD							m_dwDataQueryPos;					//数据查询位
	DWORD							m_dwDataPacketCount;				//数据包数目
	BYTE							*m_pDataStorageBuffer;				//数据指针

	//函数定义
public:
	//构造函数
	CDataStorage(void);
	//析构函数
	virtual ~CDataStorage(void);

	//功能函数
public:
	//负荷信息
	bool GetBurthenInfo(tagBurthenInfo & BurthenInfo);
	//插入数据
	bool AddData(WORD wIdentifier,WORD wChannelID,DWORD dwTime, void * const pBuffer, WORD wDataSize);
	//获取数据
	bool GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize);
	//删除数据
	void RemoveData(bool bFreeMemroy);

private:
	//分配内存
	bool RenewDataBuffer(WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif	//__DEIRLYM_DATA_STORAGE_HEAD_FILE__
