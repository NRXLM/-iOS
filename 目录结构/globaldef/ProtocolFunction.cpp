///////////////////////////////////////////////////////////////////////////////////////////////
//	协议处理函数实现文件 ProtocolFunction.cpp
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <Windows.h>
#endif
#include "ProtocolFunction.h"
#ifdef _WIN32
#include <WinSock.h>
#endif
#include "globaldef.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//读BYTE
BYTE * ReadByte(BYTE * pData,BYTE & cbValue)
{
	ASSERT(pData!=NULL);
	cbValue=pData[0];
	return pData+1;
}
const BYTE * ReadByte(const BYTE * pData,BYTE & cbValue)
{
	ASSERT(pData!=NULL);
	cbValue=pData[0];
	return pData+1;
}
//读BYTE
BYTE * ReadByte(BYTE * pData,BYTE & cbValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	cbValue=pData[0];
	wReadLen+=1;
	return pData+1;
}
const BYTE * ReadByte(const BYTE * pData,BYTE & cbValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	cbValue=pData[0];
	wReadLen+=1;
	return pData+1;
}

//写BYTE
BYTE * WriteByte(BYTE * pData,BYTE cbValue)
{
	pData[0]=cbValue;
	return pData+1;
}

BYTE * WriteByte(BYTE * pData,BYTE cbValue,WORD & wWriteLen)
{
	pData[0]=cbValue;
	wWriteLen+=1;
	return pData+1;
}

//读字
BYTE * ReadWord(BYTE * pData,WORD & wValue)
{
	ASSERT(pData!=NULL);
	memcpy(&wValue,pData,2);
	wValue=ntohs(wValue);
	return pData+2;
}
const BYTE * ReadWord(const BYTE * pData,WORD & wValue)
{
	ASSERT(pData!=NULL);
	memcpy(&wValue,pData,2);
	wValue=ntohs(wValue);
	return pData+2;
}
BYTE * ReadWord(BYTE * pData,WORD & wValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&wValue,pData,2);
	wValue=ntohs(wValue);
	wReadLen+=2;
	return pData+2;
}
const BYTE * ReadWord(const BYTE * pData,WORD & wValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&wValue,pData,2);
	wValue=ntohs(wValue);
	wReadLen+=2;
	return pData+2;
}

//写字
BYTE * WriteWord(BYTE * pData,WORD wValue)
{
	wValue=htons(wValue);
	memcpy(pData,&wValue,2);
	pData+=2;
	return pData;
}

BYTE * WriteWord(BYTE * pData,WORD wValue,WORD & wWriteLen)
{
	wValue=htons(wValue);
	memcpy(pData,&wValue,2);
	pData+=2;
	wWriteLen+=2;
	return pData;
}

//读双字
BYTE * ReadDword(BYTE * pData,DWORD & dwValue)
{
	ASSERT(pData!=NULL);
	memcpy(&dwValue,pData,4);
	dwValue=ntohl(dwValue);
	return pData+4;
}
const BYTE * ReadDword(const BYTE * pData,DWORD & dwValue)
{
	ASSERT(pData!=NULL);
	memcpy(&dwValue,pData,4);
	dwValue=ntohl(dwValue);
	return pData+4;
}
BYTE * ReadDword(BYTE * pData,DWORD & dwValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&dwValue,pData,4);
	dwValue=ntohl(dwValue);
	wReadLen+=4;
	return pData+4;
}
const BYTE * ReadDword(const BYTE * pData,DWORD & dwValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&dwValue,pData,4);
	dwValue=ntohl(dwValue);
	wReadLen+=4;
	return pData+4;
}

//读int16
BYTE * ReadInt16(BYTE * pData,__int16 & nValue)
{
	ASSERT(pData!=NULL);
	memcpy(&nValue,pData,2);
	nValue=ntohs(nValue);
	return pData+2;
}
const BYTE * ReadInt16(const BYTE * pData,__int16 & nValue)
{
	ASSERT(pData!=NULL);
	memcpy(&nValue,pData,2);
	nValue=ntohs(nValue);
	return pData+2;
}
BYTE * ReadInt16(BYTE * pData,__int16 & nValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&nValue,pData,2);
	nValue=ntohs(nValue);
	wReadLen+=2;
	return pData+2;
}

//读int32
BYTE * ReadInt32(BYTE * pData,__int32 & nValue)
{
	ASSERT(pData!=NULL);
	memcpy(&nValue,pData,4);
	nValue=ntohl(nValue);
	return pData+4;
}
const BYTE * ReadInt32(const BYTE * pData,__int32 & nValue)
{
	ASSERT(pData!=NULL);
	memcpy(&nValue,pData,4);
	nValue=ntohl(nValue);
	return pData+4;
}
BYTE * ReadInt32(BYTE * pData,__int32 & nValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&nValue,pData,4);
	nValue=ntohl(nValue);
	wReadLen+=4;
	return pData+4;
}

const BYTE * ReadInt32(const BYTE * pData,__int32 & nValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&nValue,pData,4);
	nValue=ntohl(nValue);
	wReadLen+=4;
	return pData+4;
}

//写双字
BYTE * WriteDword(BYTE * pData,DWORD dwValue)
{
	ASSERT(pData!=NULL);
	dwValue=htonl(dwValue);
	memcpy(pData,&dwValue,4);
	pData+=4;
	return pData;
}

BYTE * WriteDword(BYTE * pData,DWORD dwValue,WORD & wWriteLen,bool bHtohl)
{
	ASSERT(pData!=NULL);
	if(bHtohl)
	{
		dwValue=htonl(dwValue);
	}
	memcpy(pData,&dwValue,4);
	pData+=4;
	wWriteLen+=4;
	return pData;
}


//写int16
BYTE * WriteInt16(BYTE * pData,__int16 nValue)
{
	ASSERT(pData!=NULL);
	nValue=htons(nValue);
	memcpy(pData,&nValue,2);
	pData+=2;
	return pData;
}

BYTE * WriteInt16(BYTE * pData,__int16 nValue,WORD & wWriteLen)
{
	ASSERT(pData!=NULL);
	nValue=htons(nValue);
	memcpy(pData,&nValue,2);
	pData+=2;
	wWriteLen+=2;
	return pData;
}

//写int32
BYTE * WriteInt32(BYTE * pData,__int32 nValue)
{
	ASSERT(pData!=NULL);
	nValue=htonl(nValue);
	memcpy(pData,&nValue,4);
	pData+=4;
	return pData;
}

BYTE * WriteInt32(BYTE * pData,__int32 nValue,WORD & wWriteLen)
{
	ASSERT(pData!=NULL);
	nValue=htonl(nValue);
	memcpy(pData,&nValue,4);
	pData+=4;
	wWriteLen+=4;
	return pData;
}

//读64位数
BYTE * ReadUNDword(BYTE * pData,UNDWORD & undwValue)
{
	ASSERT(pData!=NULL);
	memcpy(&undwValue,pData,8);
	undwValue=ntohl64(undwValue);
	return pData+8;
}
const BYTE * ReadUNDword(const BYTE * pData,UNDWORD & undwValue)
{
	ASSERT(pData!=NULL);
	memcpy(&undwValue,pData,8);
	undwValue=ntohl64(undwValue);
	return pData+8;
}
BYTE * ReadUNDword(BYTE * pData,UNDWORD & undwValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&undwValue,pData,8);
	undwValue=ntohl64(undwValue);
	wReadLen+=8;
	return pData+8;
}
const BYTE * ReadUNDword(const BYTE * pData,UNDWORD & undwValue,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(&undwValue,pData,8);
	undwValue=ntohl64(undwValue);
	wReadLen+=8;
	return pData+8;
}

//写64位数
BYTE * WriteUNDword(BYTE * pData,UNDWORD undwValue)
{
	ASSERT(pData!=NULL);
	undwValue=htonl64(undwValue);
	memcpy(pData,&undwValue,8);
	return pData+8;
}
BYTE * WriteUNDword(BYTE * pData,UNDWORD undwValue,WORD & wWriteLen)
{
	ASSERT(pData!=NULL);
	undwValue=htonl64(undwValue);
	memcpy(pData,&undwValue,8);
	wWriteLen+=8;
	return pData+8;
}


//读取指定长度
BYTE * ReadToBuffer(BYTE * pData,void * pDest,WORD wLength)
{
	ASSERT(pData!=NULL);
	memcpy(pDest,pData,wLength);
	return pData+wLength;
}
const BYTE * ReadToBuffer(const BYTE * pData,void * pDest,WORD wLength)
{
	ASSERT(pData!=NULL);
	memcpy(pDest,pData,wLength);
	return pData+wLength;
}
BYTE * ReadToBuffer(BYTE * pData,void * pDest,WORD wLength,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(pDest,pData,wLength);
	wReadLen+=wLength;
	return pData+wLength;
}
const BYTE * ReadToBuffer(const BYTE * pData,void * pDest,WORD wLength,WORD & wReadLen)
{
	ASSERT(pData!=NULL);
	memcpy(pDest,pData,wLength);
	wReadLen+=wLength;
	return pData+wLength;
}

//写指定长度
BYTE * WriteFromBuffer(BYTE * pDestBuffer,const void *pSrcData,WORD wLength)
{
	ASSERT(pDestBuffer!=NULL);
	memcpy(pDestBuffer,pSrcData,wLength);
	return pDestBuffer+wLength;
}
BYTE * WriteFromBuffer(BYTE * pDestBuffer,const void *pSrcData,WORD wLength,WORD & wWriteLen)
{
	ASSERT(pDestBuffer!=NULL);
	memcpy(pDestBuffer,pSrcData,wLength);
	wWriteLen+=wLength;
	return pDestBuffer+wLength;
}

//生成命令
CMD_Command BuildCommand(WORD wMainCmd,WORD wSubCmd,DWORD dwSequenceID)
{
	CMD_Command cmd;
	cmd.wMainCmd=wMainCmd;
	cmd.wSubCmd=wSubCmd;
	cmd.dwSequenceID=dwSequenceID;

	return cmd;
}

union __unCMDDword
{
	DWORD					dwCommandID;					//命令字
	struct
	{
        WORD				wMainCmd;						//主命令
		WORD				wSubCmd;						//字命令
	};
};

//生成命令字
DWORD BuildCommandID(WORD wMainCmd,WORD wSubCmd)
{
	__unCMDDword unDword;
	unDword.wMainCmd=wMainCmd;
	unDword.wSubCmd=wSubCmd;

	return unDword.dwCommandID;
}
