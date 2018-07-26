/////////////////////////////////////////////////////////////////////////////////
//		Windows 共享内存类头文件 WinShareMem.h
//		hubo 2013-10-28
//////////////////////////////////////////////////////////////////////////////////
#pragma once

class CWinShareMem
{
private:
	HANDLE				m_hShareMemory;			//共享内存
	void				*m_pShareMemData;		//共享内存数据

public:
	CWinShareMem(void);
	virtual ~CWinShareMem(void);

protected:
	//创建共享内存
	void * CreateShareMem(LPCTSTR lpszName,DWORD dwMemSize);
	//打开共享内存
	void * OpenShareMem(LPCTSTR lpszName);
};

//////////////////////////////////////////////////////////////////////
//共享内存模板
template<class TYPE,class ARG_TYPE=const TYPE &>
class CShareMemTemplate: public CWinShareMem
{
public:
	//构造函数
	CShareMemTemplate();
	//析构函数
	virtual ~CShareMemTemplate();

public:
	//创建共享内存
	TYPE * CreateShareMemory(LPCTSTR lpszName);
	//打开共享内存
	TYPE * OpenShareMemory(LPCTSTR lpszName);
};

///////////////////////////////////////////////////////////////////////
// 数组模板实现
///////////////////////////////////////////////////////////////////////
//构造函数
template<class TYPE,class ARG_TYPE>
CShareMemTemplate<TYPE,ARG_TYPE>::CShareMemTemplate()
{
}

//析构函数
template<class TYPE,class ARG_TYPE>
CShareMemTemplate<TYPE,ARG_TYPE>::~CShareMemTemplate()
{
}

//创建共享内存
template<class TYPE,class ARG_TYPE>
TYPE * CShareMemTemplate<TYPE,ARG_TYPE>::CreateShareMemory(LPCTSTR lpszName)
{
	DWORD dwSize=sizeof(TYPE);
	TYPE * pMemData=(TYPE *)CreateShareMem(lpszName,dwSize);
	return pMemData;
}

//打开共享内存
template<class TYPE,class ARG_TYPE>
TYPE * CShareMemTemplate<TYPE,ARG_TYPE>::OpenShareMemory(LPCTSTR lpszName)
{
	TYPE *pMemData=(TYPE *)OpenShareMem(lpszName);
	return pMemData;
}
