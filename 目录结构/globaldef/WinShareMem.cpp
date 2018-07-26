/////////////////////////////////////////////////////////////////////////////////
//		Windows 共享内存类实现文件 WinShareMem.cpp
//		hubo 2013-10-28
//////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "WinShareMem.h"

////////////////////////////////////////////////////////////////////////////////
CWinShareMem::CWinShareMem(void)
{
	m_hShareMemory=NULL;
	m_pShareMemData=NULL;
}

CWinShareMem::~CWinShareMem(void)
{
	//共享内存
	if(m_pShareMemData!=NULL)
	{
		::UnmapViewOfFile(m_pShareMemData);
		m_pShareMemData=NULL;
	}
	if(m_hShareMemory!=NULL)
	{
		::CloseHandle(m_hShareMemory);
		m_hShareMemory=NULL;
	}
}

//创建共享内存
void * CWinShareMem::CreateShareMem(LPCTSTR lpszName,DWORD dwMemSize)
{
	if(lpszName==NULL || lpszName[0]==0) return NULL;
	if(dwMemSize==0) return NULL;

	m_hShareMemory=(HANDLE)CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,dwMemSize,lpszName);
	DWORD dwError=GetLastError();
	if ((m_hShareMemory==NULL)||(dwError==ERROR_ALREADY_EXISTS))
	{
		return NULL;
	}
	m_pShareMemData=MapViewOfFile(m_hShareMemory,FILE_MAP_ALL_ACCESS,0,0,0);

	return m_pShareMemData;
}

//打开共享内存
void * CWinShareMem::OpenShareMem(LPCTSTR lpszName)
{
	if(lpszName==NULL || lpszName[0]==0) return NULL;

	m_hShareMemory=::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,lpszName);
	
	if(m_hShareMemory==NULL) return NULL;
	//映射到共享内存
	m_pShareMemData=::MapViewOfFile(m_hShareMemory,FILE_MAP_ALL_ACCESS,0,0,0);

	return m_pShareMemData;
}
