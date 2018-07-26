// //////////////////////////////////////////////////////////////////////////////////////////////////////////
//  file process class head file myFile.h
// /////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_FILE_PROCESS_MYFILE_HEAD_FILE__
#define __DEIRLYM_FILE_PROCESS_MYFILE_HEAD_FILE__

#include "IUnknownEx.h"

#define FILEBUFSIZE                     1024
// //////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMyFile
{
     //file open property
public:
     enum 
     {
          READ=0x0001,                  //Read file
          WRITE=0x0002,                 //Write to file
          CREATE=0x0004,               //Create new file
          APPEND=0x0008,              //Add to exists file          
     };
     
     //variable define 
private:
     FILE                               *m_pFile;                                   // file point
     TCHAR                              m_szFileName[MAX_PATH];						//file name
     char                               m_szBuffer[FILEBUFSIZE];					//file buffer
     DWORD                              m_dwLineLenght;                         
     DWORD                              m_dwLInes;                                   //line count
     DWORD                              m_dwFileSize;                                //file size
     DWORD                              m_dwWriteSize;                               // write size
     DWORD                              m_dwReadSize;                                //Read data size
     
private:
	DWORD								m_dwWriteCounter;							//写入计数，每万次左右刷新一次
	DWORD								m_dwNumberOfWritesToFFlush;					//写入指定次数刷新到磁盘
     //function define
public:     
     //construct function
     CMyFile(LPCTSTR lpszFileName=NULL);
     virtual ~CMyFile();
     
     //function
public:
	//设置刷新时的写入次数
	void SetNumOfWritesToFFlush(DWORD dwNumOfWritesToFFlush);
     // open file
     bool Open(LPCTSTR lpszFileName=NULL,DWORD dwFlags=READ);
     //close file
     void Close();
     //Write to file
     void WriteArgList(TCHAR * pszWrite,...);
     void Write(const void * pBuf,INT nSize);
	 void WriteStrIntervalFlush(const void * pBuf,INT nSize);
     void WriteStr(const TCHAR * pszWrite);
	 void WriteStrIntervalFlush(const TCHAR * pszWrite);	  
     //Read Data
     LONG Read(void * pBuf,INT nSize);
     //Open State
     bool IsOpen()const{return m_pFile!=NULL;}
     //Get a line
     char * GetLine(INT & nLength);
     //Delete file
     void DeleteFile();
	 //rename
	 void RenameFile(LPCTSTR lpszDstName);
     //Get File Size
     DWORD GetFileSize()const{return m_dwFileSize;}    
     //Get Write size
     DWORD GetWriteSize()const{return m_dwWriteSize;}
     //Get Line count
     DWORD GetLineCount()const{return m_dwLInes;}
     //Get Current FileName
     LPCTSTR GetFileName()const{return m_szFileName;} 	 

	 //Operator function
public:
	//Seek to first
	int SeekFirst();
	//Seek to mid
	int SeekMidFromFirst(DWORD dwMid);
	int SeekMidFromEnd(DWORD dwMid);
	//Seek to end
	int SeekEnd();

};

#endif //__DEIRLYM_FILE_PROCESS_MYFILE_HEAD_FILE__
