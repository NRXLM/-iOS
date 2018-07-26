///////////////////////////////////////////////////////////////////////////////////
//		globaldef.cpp
///////////////////////////////////////////////////////////////////////////////////
#include "globaldef.h"
#ifdef _WIN32
#include <winsock2.h>
#endif
#include <iostream>

#define KEY_LEN		9
BYTE g_EncodeKey[]="bOwEn5878";
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////64bit网络字节序///////////////
//网络字节序处理
union NetByeOrder
{
	DWORD	dwValue;
	BYTE	cbByte[4];
};
struct tagInt
{
    unsigned long nVal1;
    unsigned long nVal2;
};

union tagLong
{
    UNDWORD ulVal;
    struct tagInt st;
};

union tagDouble
{
	double ulVal;
    struct tagInt st;
};

UNDWORD htonl64(UNDWORD ulSrc)
{
	NetByeOrder nbo;
	nbo.dwValue=1;
	if(nbo.cbByte[0]==0)
		return ulSrc;

	tagLong t1={0};
	tagLong t2={0};
    t1.ulVal=ulSrc;    
    t2.st.nVal1=htonl(t1.st.nVal2);
    t2.st.nVal2=htonl(t1.st.nVal1);
    return t2.ulVal;
}

UNDWORD ntohl64(UNDWORD ulSrc)
{
	NetByeOrder nbo;
	nbo.dwValue=1;
	if(nbo.cbByte[0]==0)
		return ulSrc;

	tagLong t1={0};
	tagLong t2={0};
    t1.ulVal=ulSrc;
    t2.st.nVal1=ntohl(t1.st.nVal2);
    t2.st.nVal2=ntohl(t1.st.nVal1);
    return t2.ulVal;
}

double ntohdb64(double ulSrc)
{
	NetByeOrder nbo;
	nbo.dwValue=1;
	if(nbo.cbByte[0]==0)
		return ulSrc;

	tagDouble t1={0};
	tagDouble t2={0};
    t1.ulVal=ulSrc;
    t2.st.nVal1=ntohl(t1.st.nVal2);
    t2.st.nVal2=ntohl(t1.st.nVal1);
    return t2.ulVal;
}

//转换IP地址
DWORD ConvertUrlToIpAddr(LPCSTR lpszUrl)
{ 
	DWORD dwServerIP=inet_addr(lpszUrl);
	if(dwServerIP==INADDR_NONE)
	{
		//转化地址
		LPHOSTENT lpHost=gethostbyname(lpszUrl);
		if (lpHost==NULL) return INADDR_NONE;
		dwServerIP=((in_addr*)lpHost->h_addr)->s_addr;
	}

	return dwServerIP;
}

int EncCheckCode(DWORD dwID,LPCSTR pszUserName,int nLength,CHAR * pCheckCode)
{
	const int nNameLen=128;
	if(nLength>=nNameLen) return 0;
	CHAR szUserName[nNameLen];
	memcpy(szUserName,pszUserName,nLength);
	szUserName[nLength]=0;
	DWORD dwTmp='ABCD';
	dwTmp+=dwID;
	BYTE *pCode=(BYTE *)(&dwTmp);

	for(int n=0;n<nLength;n++)
	{
		int nLatter=szUserName[n];
		nLatter^=pCode[0];
		nLatter^=pCode[1];
		nLatter^=pCode[2];
		nLatter^=pCode[3];
		szUserName[n]=nLatter;
	}

	return EncCheckCode(szUserName,nLength,pCheckCode);
}

int EncCheckCode(LPCSTR pszUserName,int nLength,CHAR * pCheckCode)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	int nMonth=st.wMonth+'A';
	int nDay=st.wDay+'A';
	int nHour=st.wHour+'A';
	int nMinute=st.wMinute+'A';
	int nSecond=st.wSecond+'A';
	return EncCheckCode(pszUserName,nLength,pCheckCode,nMonth,nDay,nHour,nMinute,nSecond);
}

int EncCheckCode(LPCSTR pszUserName,int nLength,CHAR * pCheckCode,int nMonth,int nDay,int nHour,int nMinute,int nSecond)
{
	if(pCheckCode==NULL)
		return 0;
	if(pszUserName==NULL)
		return 0;

	const int nDstLength=128;
	const int nTimeLen=5;
	int nLen = (int)strlen(pszUserName);
	if(nLen>=nDstLength) return 0;
	TCHAR szDstName[nDstLength+6];

	for(int i=0;i<nLen;i++)
	{
		int nLatter=pszUserName[i];
		nLatter^=nMonth;
		nLatter^=nDay;
		nLatter^=nHour;
		nLatter^=nMinute;
		nLatter^=nSecond;
		szDstName[i]=nLatter;
	}
	int nIndex = nLen;
	szDstName[nIndex++]=nMonth;
	szDstName[nIndex++]=nDay;
	szDstName[nIndex++]=nHour;
	szDstName[nIndex++]=nMinute;
	szDstName[nIndex++]=nSecond;
	for(int i=0;i<nTimeLen;i++)
	{
		int nTemp=szDstName[i];
		szDstName[i]=szDstName[nIndex-1-i];
		szDstName[nIndex-1-i]=nTemp;
	}
	CHAR * pEncCode=pCheckCode;
	for(int i=0;i<nLen+nTimeLen;i++)
	{
		int nLatter=szDstName[i];
		for(int j=0;j<KEY_LEN;j++)
		{
			int nPass=g_EncodeKey[j];
			nLatter^=nPass;
		}
#ifndef _WIN32
		sprintf(pEncCode,"%02x",(BYTE)nLatter);
#else
		_snprintf_s(pEncCode,3,3,"%02x",(BYTE)nLatter);
#endif
		pEncCode+=2;
	}

	return (nLen+nTimeLen)*2;
}

void EncCheckCode(UNDWORD undwMobile,CHAR * pCheckCode)
{
	if(pCheckCode==NULL)
		return ;
	if(undwMobile==0)
		return;

	SYSTEMTIME st;
	GetLocalTime(&st);

	int nMonth=st.wMonth+'A';
	int nDay=st.wDay+'A';
	int nHour=st.wHour+'A';
	int nMinute=st.wMinute+'A';
	int nSecond=st.wSecond+'A';

	BYTE szDstMobile[24];
#ifndef _WIN32
	int64tochar(undwMobile,(char*)szDstMobile,24);
#else
	_ui64toa_s(undwMobile,(char*)szDstMobile,24,10);
#endif
	int nLength=11;
	for(int i=0;i<nLength;i++)
	{
		int nLatter=szDstMobile[i];
		nLatter^=nMonth;
		nLatter^=nDay;
		nLatter^=nHour;
		nLatter^=nMinute;
		nLatter^=nSecond;
		szDstMobile[i]=nLatter;
	}
	int nIndex = 11;
	szDstMobile[nIndex++]=nMonth;
	szDstMobile[nIndex++]=nDay;
	szDstMobile[nIndex++]=nHour;
	szDstMobile[nIndex++]=nMinute;
	szDstMobile[nIndex++]=nSecond;
	for(int i=0;i<5;i++)
	{
		int nTemp=szDstMobile[i];
		szDstMobile[i]=szDstMobile[15-i];
		szDstMobile[15-i]=nTemp;
	}
	CHAR * pEncCode=pCheckCode;
	for(int i=0;i<16;i++)
	{
		int nLatter=szDstMobile[i];
		for(int j=0;j<KEY_LEN;j++)
		{
			int nPass=g_EncodeKey[j];
			nLatter^=nPass;
		}
#ifndef _WIN32
		sprintf(pEncCode,"%02x",nLatter);
#else
		_snprintf_c(pEncCode,3,"%02x",nLatter);
#endif
		pEncCode+=2;
	}
}

int GetHex(int nHigh,int nLow)
{
	if (nHigh >= 'a' && nHigh <= 'z')
    {
        nHigh -= 'a';
        nHigh += 10;
    }
    else
    {
        nHigh -= '0';
    }

    if (nLow >= 'a' && nLow <= 'z')
    {
        nLow -= 'a';
        nLow += 10;
    }
    else
    {
        nLow -= '0';
    }

    return nHigh * 16 + nLow;
}

bool DecCheckCode(DWORD dwID,const CHAR * pCheckCode,LPSTR pszUserName)
{
	const int nNameLen=128;
	int nLength=(int)strlen(pszUserName);
	if(nLength>=nNameLen) return false;
	CHAR szUserName[nNameLen];
	memcpy(szUserName,pszUserName,nLength);
	szUserName[nLength]=0;
	DWORD dwTmp='ABCD';
	dwTmp+=dwID;
	BYTE *pCode=(BYTE *)(&dwTmp);

	for(int n=0;n<nLength;n++)
	{
		int nLatter=szUserName[n];
		nLatter^=pCode[0];
		nLatter^=pCode[1];
		nLatter^=pCode[2];
		nLatter^=pCode[3];
		szUserName[n]=nLatter;
	}

	return DecCheckCode(pCheckCode,szUserName);
}

bool DecCheckCode(const CHAR * pCheckCode,LPSTR pszUserName)
{
	if(pCheckCode==NULL || pszUserName == NULL)
		return false;

	const int nCodeSize=256;
	const int nTimeLen=5;
	int nCodeLength=(int)strlen(pCheckCode);
	if(nCodeLength>=nCodeSize) return false;

	BYTE szEncCode[nCodeSize]; 
	nCodeLength/=2;
	for(int i=0;i<nCodeLength;i++)
	{
		int nHigh=pCheckCode[i<<1];
		int nLow=pCheckCode[(i<<1)+1];
		int nLatter=GetHex(nHigh,nLow);
		for(int j=0;j<KEY_LEN;j++)
		{
			int cbPass=g_EncodeKey[j];
			nLatter^=cbPass;
		}
		szEncCode[i]=nLatter;
	}
	for(int i=0;i<nTimeLen;i++)
	{
		int nTemp=szEncCode[i];
		szEncCode[i]=szEncCode[nCodeLength-1-i];
		szEncCode[nCodeLength-1-i]=nTemp;
	}

	int nIndex = nCodeLength-nTimeLen;
	nCodeLength=nIndex;
	int nVar1 = szEncCode[nIndex++];
	int nVar2 = szEncCode[nIndex++];
	int nVar3 = szEncCode[nIndex++];
	int nVar4 = szEncCode[nIndex++];
	int nVar5 = szEncCode[nIndex++];

	for(int i=0;i<nCodeLength;i++)
	{
		int nLatter=szEncCode[i];
		nLatter^=nVar1;
		nLatter^=nVar2;
		nLatter^=nVar3;
		nLatter^=nVar4;
		nLatter^=nVar5;
		szEncCode[i]=nLatter;
	}
	
	int nDstNameLen=(int)strlen(pszUserName);
	if(nDstNameLen!=nCodeLength) return false;

	return (strncmp(pszUserName,(CHAR *)szEncCode,nDstNameLen) == 0);
}

void DecCheckCode(const CHAR * pCheckCode,UNDWORD &undwMobile)
{
	undwMobile=0;

	if(pCheckCode==NULL)
		return;

	BYTE szEncCode[16]; 
	for(int i=0;i<16;i++)
	{
		int nHigh=pCheckCode[i<<1];
		int nLow=pCheckCode[(i<<1)+1];
		int nLatter=GetHex(nHigh,nLow);
		for(int j=0;j<KEY_LEN;j++)
		{
			int cbPass=g_EncodeKey[j];
			nLatter^=cbPass;
		}
		szEncCode[i]=nLatter;
	}
	for(int i=0;i<5;i++)
	{
		int nTemp=szEncCode[i];
		szEncCode[i]=szEncCode[15-i];
		szEncCode[15-i]=nTemp;
	}

	int nIndex = 11;
    int nVar1 = szEncCode[nIndex++];
    int nVar2 = szEncCode[nIndex++];
    int nVar3 = szEncCode[nIndex++];
    int nVar4 = szEncCode[nIndex++];
    int nVar5 = szEncCode[nIndex++];

	CHAR szMobile[12];
	for(int i=0;i<11;i++)
	{
		int nTmpVar=szEncCode[i];
		nTmpVar^=nVar1;
		nTmpVar^=nVar2;
		nTmpVar^=nVar3;
		nTmpVar^=nVar4;
		nTmpVar^=nVar5;
		szMobile[i]=nTmpVar;
	}
	szMobile[11]=0;
#ifndef _WIN32
	undwMobile=CharToUInt64(szMobile);
#else
	undwMobile=_atoi64(szMobile);
#endif
}

#ifdef _WIN32
int watoi(const WCHAR *lpszNum)
{
    if(lpszNum==NULL) return 0;
    int nResult=0;    
    WCHAR cSign=*lpszNum;
	bool bSingle=false;
    if(cSign==L'-' || cSign==L'+')
	{
		if(cSign=='-') bSingle=true;
        lpszNum++;
	}
    int nNumber=0;
    while(*lpszNum)
    {
        nNumber=(int)((*lpszNum)-L'0');
        nResult=nResult*10+nNumber;
        lpszNum++;
    }
	if(bSingle) nResult=-nResult;
  
    return nResult;
}

//UTF8 转为 GBK
int UTF8ToGBK(const char * utf8Str,char * pGBKBuffer,int nGBKBuffSize)
{
	if(utf8Str==NULL)
		return 0;

	int nSize=::MultiByteToWideChar(CP_UTF8,0,utf8Str,-1,NULL,0);
	WCHAR * wSrc=new WCHAR[nSize+1];
	::MultiByteToWideChar(CP_UTF8,0,utf8Str,-1,wSrc,nSize);	
	nSize=::WideCharToMultiByte(CP_ACP,0,wSrc,-1,NULL,0,NULL,NULL);
	if(nGBKBuffSize<nSize+1)
	{
		return 0;
	}
	
	::WideCharToMultiByte(CP_ACP,0,wSrc,-1,pGBKBuffer,nSize,NULL,NULL);	
	delete [] wSrc;

	return nSize;
}

//GBK 转为 UTF8
int GBKToUTF8(const char * gbkStr,char *pUTF8Buffer,int nUTF8Size)
{
	if(gbkStr==NULL)
		return 0;

	WCHAR * strSrc=NULL;

	int nSize=::MultiByteToWideChar(CP_ACP,0,gbkStr,-1,NULL,0);
	strSrc=new WCHAR[nSize+1];
	::MultiByteToWideChar(CP_ACP,0,gbkStr,-1,strSrc,nSize);

	nSize=::WideCharToMultiByte(CP_UTF8,0,strSrc,-1,NULL,0,NULL,NULL);
	if(nUTF8Size<nSize+1)
	{
		return 0;
	}
 
	::WideCharToMultiByte(CP_UTF8,0,strSrc,-1,pUTF8Buffer,nSize,NULL,NULL);

	delete [] strSrc;

	return nSize;
}

//Unicode转UTF8
int UnicodeToUTF8(const wchar_t *pwStr,char *pUTF8Buffer,int nUTF8Size)
{
	if(pwStr==NULL || pUTF8Buffer==NULL) return 0;

	int nSize=::WideCharToMultiByte(CP_UTF8,0,pwStr,-1,NULL,0,NULL,NULL);
	if(nUTF8Size<nSize+1)
		return 0;

	::WideCharToMultiByte(CP_UTF8,0,pwStr,-1,pUTF8Buffer,nUTF8Size,NULL,NULL);
	return nSize;
}

//Utf8转为Unicode
int UTF8ToUnicode(const char *pUTF8Buffer,wchar_t *pwStr,int nBufferSize)
{
	if(pwStr==NULL || pUTF8Buffer==NULL) return 0;

	int nSize=::MultiByteToWideChar(CP_UTF8,0,pUTF8Buffer,-1,NULL,0);
	if(nBufferSize<nSize+1)
		return 0;

	::MultiByteToWideChar(CP_UTF8,0,pUTF8Buffer,-1,pwStr,nBufferSize);
	return nSize;
}

//用指定彦色输出到控制台
//FOREGROUND_BLUE, FOREGROUND_GREEN, FOREGROUND_RED,
//FOREGROUND_INTENSITY, BACKGROUND_BLUE, BACKGROUND_GREEN,
//BACKGROUND_RED, and BACKGROUND_INTENSITY.
void OutTextByColor(LPCTSTR lpszText,WORD wColor)
{
	HANDLE hControl=::GetStdHandle(STD_OUTPUT_HANDLE);
	if(hControl==NULL) return;
	SetConsoleTextAttribute(hControl,wColor);
	std::cout<<lpszText<<std::endl;
	wColor=FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hControl,wColor);
}
#else
//获取当前时间的秒计数
DWORD GetTickCount()
{
	struct timeval tv;  
    struct timezone tz;
    gettimeofday(&tv,&tz);
	DWORD dwTime=tv.tv_sec*1000+tv.tv_usec/1000; 
	return dwTime;
}

void GetLocalTime(PSYSTEMTIME pst)
{
     ASSERT(pst!=NULL);
     if(pst==NULL) return;
     struct tm * pTp;
     time_t tTime=time(NULL);
     pTp=localtime(&tTime);     
     if(pTp==NULL) return;
     pst->wYear=pTp->tm_year+1900;
     pst->wMonth=pTp->tm_mon+1;
     pst->wDay=pTp->tm_mday;
     pst->wHour=pTp->tm_hour;
     pst->wMinute=pTp->tm_min;
     pst->wSecond=pTp->tm_sec;   	
}

void int64tochar(ULONGLONG lnSrc,char *pszDest,int nSize)
{
	if(pszDest==NULL || nSize<=0) return;

    if(lnSrc==0)
    {
        *pszDest='0';
        pszDest++;
        *pszDest=0;
        return;
    }
    char *pNum=pszDest;    
    char *pFirst=pNum;    
    DWORD dwTemp=0;  
    char cbTemp;
    while(lnSrc && nSize)
    {        
        dwTemp=(DWORD)(lnSrc%10);
        *pNum++=(char)('0'+dwTemp);
        lnSrc/=10;
        nSize--;
    }    
    *pNum--=0;

    do
    {
        cbTemp=*pNum;
        *pNum=*pFirst;
        *pFirst=cbTemp;
        --pNum;
        ++pFirst;        

    }while(pNum>pFirst);
}

UDWORD CharToUInt64(char *pszSrc)
{
	ASSERT(pszSrc!=NULL);
    if(pszSrc==NULL) return 0;
    UDWORD udwResult=0;    
    char cSign=*pszSrc;
    if(cSign=='-' || cSign=='+')
        pszSrc++;
    int nNumber=0;
    while(*pszSrc)
    {
        nNumber=(int)((*pszSrc)-'0');
        udwResult=udwResult*10+nNumber;
        pszSrc++;
    }  
  
    return udwResult;
}

#endif
//check if the char array is digit char
bool CheckDigit(char *data)
{
	for (; *data; data++)
	{
		if (isdigit(*data))
			continue;
		return false;
	}
	return true;
}
/********************************************
 功能：把字符串转换为全小写
 ********************************************/
void ToLowerCase(TCHAR * s)
{
    while(*s)
    {
        if(*s>=TEXT('A') && *s<=TEXT('Z'))
            *s = (*s+0x20);
        s++;
    }
}

void MyThreadWait(DWORD dwTime)
{
#if 0
	struct timeval delay;
	delay.tv_sec = dwTime/1000;
	delay.tv_usec = (dwTime%1000)* 1000; // 20 ms
	select(0, NULL, NULL, NULL, &delay);
#else
	struct timespec delay;
	delay.tv_sec = dwTime/1000;
	delay.tv_nsec = (dwTime%1000)* 1000000; // 20 ms
	nanosleep(&delay,NULL);
#endif
}

/********************************************
 功能：把字符串转换为全大写
 ********************************************/
void ToUpperCase(char * s)
{
    while(*s)
    {
        if(*s>='a' && *s<='z')
            *s = (*s&0xDF);
        s++;
    }
}
