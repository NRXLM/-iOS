// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   file process class implment file MyFile.cpp
// //////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <Windows.h>
#endif
#include <stdarg.h>
#include "myFile.h"
#ifdef _UNICODE
	#include <wchar.h>
#endif

#ifndef ASSERT
#define ASSERT(f) ((void)0)
#endif

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//construct function
CMyFile::CMyFile(LPCTSTR lpszFileName)
{
     m_pFile=NULL;
     m_szFileName[0]=0;
     if(lpszFileName!=NULL)  
		 lstrcpy(m_szFileName,lpszFileName);
     
     m_dwLineLenght=0;
     m_dwLInes=0;
     m_dwFileSize=0;
     m_dwWriteSize=0;     
     m_dwReadSize=0;

	 m_dwWriteCounter = 0;
	 m_dwNumberOfWritesToFFlush = 10;
}
CMyFile::~CMyFile()
{
     Close();
}
//设置刷新时的写入次数
void CMyFile::SetNumOfWritesToFFlush(DWORD dwNumOfWritesToFFlush) 
{
	m_dwNumberOfWritesToFFlush = dwNumOfWritesToFFlush;
}
// open file
bool CMyFile::Open(LPCTSTR lpszFileName,DWORD dwFlags)
{
     if(m_pFile)
          Close();
     if(lpszFileName!=NULL)
          lstrcpy(m_szFileName,lpszFileName);
     if(m_szFileName[0]==0) return false;
  
	 int nErrCode=0;
     if(dwFlags==(READ|WRITE|CREATE|APPEND) || 
        dwFlags==(WRITE|CREATE|APPEND) || dwFlags==(CREATE|APPEND) ||
        dwFlags==(WRITE|APPEND) || dwFlags==APPEND ) 
	 {
#ifdef _UNICODE 
#ifdef _WIN32
		 nErrCode=_wfopen_s(&m_pFile,m_szFileName,TEXT("ab"));
#else
		 nErrCode=fopen_s(&m_pFile,m_szFileName,"ab"); 
#endif
#else
#ifdef _WIN32
         nErrCode=fopen_s(&m_pFile,m_szFileName,"ab");  
#else
		 m_pFile=fopen(m_szFileName,"ab");  
#endif
#endif
	 }
     else if(dwFlags==CREATE || dwFlags==WRITE || 
             dwFlags==(READ|WRITE) ||
             dwFlags==(WRITE|CREATE) || dwFlags==(READ|WRITE|CREATE))
	 {
#ifdef _UNICODE
#ifdef _WIN32
		 nErrCode=_wfopen_s(&m_pFile,m_szFileName,TEXT("wb"));
#else
		 m_pFile=fopen(m_szFileName,"wb");  
#endif
#else
#ifdef _WIN32
         nErrCode=fopen_s(&m_pFile,m_szFileName,"wb");
#else
		 m_pFile=fopen(m_szFileName,"wb");  
#endif
#endif
	 }
     else if(dwFlags==READ || dwFlags==(READ|CREATE))
	 {
#ifdef _UNICODE
#ifdef _WIN32
		 nErrCode=_wfopen_s(&m_pFile,m_szFileName,TEXT("rb"));
#else
		 m_pFile=fopen(m_szFileName,"rb"); 
#endif
#else
#ifdef _WIN32
         nErrCode=fopen_s(&m_pFile,m_szFileName,"rb");
#else
		 m_pFile=fopen(m_szFileName,"rb");  
#endif
#endif
	 }
     
     m_dwLineLenght=0;
     m_dwLInes=0;
     m_dwFileSize=0;
     m_dwWriteSize=0;     
     m_dwReadSize=0;
     if(m_pFile)
     {
          fseek(m_pFile,0,SEEK_END);   //point file end
          m_dwFileSize=ftell(m_pFile);  //get file size
          fseek(m_pFile,0,SEEK_SET);    //point file head                   
     }
     return (m_pFile!=NULL);
}
//close file
void CMyFile::Close()
{
     if(m_pFile)
     {
		  fflush(m_pFile);
          if(m_pFile!=stdout || m_pFile!=stderr)
               fclose(m_pFile);
          m_pFile=NULL;
          m_dwFileSize=0;
     }
     m_dwLineLenght=0;
     m_dwLInes=0;     
     m_dwWriteSize=0;     
     m_dwReadSize=0;
	 m_dwWriteCounter = 0;
}
//Write to file
void CMyFile::WriteArgList(TCHAR * pszWrite,...)
{
     if(pszWrite==NULL || m_pFile==NULL)
          return ;
     
     TCHAR szBuffer[1024];     
     va_list arglist;
     va_start(arglist,pszWrite);
#ifdef _UNICODE
#ifdef _WIN32
	 vswprintf_s(szBuffer,2048,pszWrite,arglist);
#else
	 vsprintf(szBuffer,pszWrite,arglist);
#endif
#else
#ifdef _WIN32
	 _vsnprintf_c(szBuffer,1024,pszWrite,arglist);
#else
	 vsprintf(szBuffer,pszWrite,arglist);
#endif
#endif
     va_end(arglist);
     //Write to file
     m_dwWriteSize+=(DWORD)lstrlen(szBuffer);   
#ifdef _UNICODE
#ifdef _WIN32
	 fwprintf_s(m_pFile,szBuffer);
#else
	 fprintf(m_pFile,szBuffer);
#endif
#else
     fprintf(m_pFile,szBuffer);
#endif
     fflush(m_pFile);
}
void CMyFile::WriteStr(const TCHAR * pszWrite)
{
     if(pszWrite==NULL) return;
	 int nStrLen=lstrlen(pszWrite);
	 if(nStrLen==0) return;
     m_dwWriteSize+=(DWORD)nStrLen;
#ifdef _UNICODE
#ifdef _WIN32
	 fwprintf_s(m_pFile,pszWrite);
#else
	 fprintf(m_pFile,szBuffer);
#endif
#else
#ifdef _WIN32
     fprintf_s(m_pFile,pszWrite);
#else
	 fprintf(m_pFile,pszWrite);
#endif
#endif
     fflush(m_pFile);
}
void CMyFile::WriteStrIntervalFlush(const TCHAR * pszWrite)
{
	if(pszWrite==NULL) return;
	m_dwWriteSize+=(DWORD)lstrlen(pszWrite);
#ifdef _UNICODE
#ifdef _WIN32
	fwprintf_s(m_pFile,pszWrite);
#else
	fprintf(m_pFile,szBuffer);
#endif
#else
#ifdef _WIN32
	fprintf_s(m_pFile,pszWrite);
#else
	fprintf(m_pFile,pszWrite);
#endif
#endif
	if(++m_dwWriteCounter > m_dwNumberOfWritesToFFlush)
	{
		m_dwWriteCounter = 0;
		fflush(m_pFile);
	}
} 
void CMyFile::Write(const void * pBuf,INT nSize)
{
     if(pBuf==NULL || m_pFile==NULL) return;
     m_dwWriteSize+=nSize;
     fwrite(pBuf,nSize,1,m_pFile);
     fflush(m_pFile);
}
void CMyFile::WriteStrIntervalFlush(const void * pBuf,INT nSize)
{
	if(pBuf==NULL || m_pFile==NULL) return;
	m_dwWriteSize+=nSize;
	fwrite(pBuf,nSize,1,m_pFile);
	if(++m_dwWriteCounter > m_dwNumberOfWritesToFFlush)
	{
		m_dwWriteCounter = 0;
		fflush(m_pFile);
	}
}
//Read Data
LONG CMyFile::Read(void * pBuf,INT nSize)
{
     ASSERT(pBuf!=NULL);
     ASSERT(m_pFile!=NULL);
     if(pBuf==NULL || m_pFile==NULL) return -1;
     if(nSize>INT(m_dwFileSize-m_dwReadSize))
          nSize=m_dwFileSize-m_dwReadSize;
     INT nCount=(INT)fread(pBuf,nSize,1,m_pFile);     
     nSize*=nCount;
     m_dwReadSize+=nSize;
     return nSize;     
}
//Get a line
char * CMyFile::GetLine(INT & nLength)
{     
     if(m_pFile==NULL) return NULL;
     char * lpString=NULL;
               
     while(true)
     {
          if((lpString=fgets(m_szBuffer,FILEBUFSIZE,m_pFile))==NULL)
               break;
          //Trim right space
          INT nIndex=(INT)strlen(lpString)-1;
          while(nIndex>0 && (lpString[nIndex]=='\n' || lpString[nIndex]=='\r'))
               lpString[nIndex--]=0;
          nLength=nIndex+1;
          m_dwLInes++;     
          m_dwLineLenght=nLength;
          return lpString;
     }
     Close();
     return NULL;
}
//Delete file
void CMyFile::DeleteFile()
{
     Close();
#ifdef _UNICODE
#ifdef _WIN32
	 _wremove(m_szFileName);
#else
	 unlink(m_szFileName);
#endif
#else
     unlink(m_szFileName);
#endif
     ZeroMemory(m_szFileName,sizeof(m_szFileName));
}

//rename
void CMyFile::RenameFile(LPCTSTR lpszDstName)
{
#ifdef _UNICODE
#ifdef _WIN32
	_wrename(m_szFileName,lpszDstName);
#else
	::rename(m_szFileName,lpszDstName);
#endif
#else
	::rename(m_szFileName,lpszDstName);
#endif
}

//Seek to first
int CMyFile::SeekFirst()
{
	ASSERT(m_pFile!=NULL);
	if(m_pFile==NULL) return -1;
	return fseek(m_pFile,0,SEEK_SET);   
}
//Seek to mid
int CMyFile::SeekMidFromFirst(DWORD dwMid)
{
	ASSERT(m_pFile!=NULL);
	if(m_pFile==NULL) return -1;
	return fseek(m_pFile,dwMid,SEEK_SET);   
}
int CMyFile::SeekMidFromEnd(DWORD dwMid)
{
	ASSERT(m_pFile!=NULL);
	if(m_pFile==NULL) return -1;
	return fseek(m_pFile,-(long)dwMid,SEEK_END);  
}
//Seek to end
int CMyFile::SeekEnd()
{
	ASSERT(m_pFile!=NULL);
	if(m_pFile==NULL) return -1;
    return fseek(m_pFile,0,SEEK_END);  
}
