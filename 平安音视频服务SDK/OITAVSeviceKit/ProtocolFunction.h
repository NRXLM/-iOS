///////////////////////////////////////////////////////////////////////////////////////////////
//	协议处理函数头文件 ProtocolFunction.h
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_PROTOCOL_FUNCTION_HEAD_FILE__
#define __DEIRLYM_PROTOCOL_FUNCTION_HEAD_FILE__

#include "globaldef.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//读BYTE
BYTE * ReadByte(BYTE * pData,BYTE & cbValue);
const BYTE * ReadByte(const BYTE * pData,BYTE & cbValue);
BYTE * ReadByte(BYTE * pData,BYTE & cbValue,WORD & wReadLen);
const BYTE * ReadByte(const BYTE * pData,BYTE & cbValue,WORD & wReadLen);

//写BYTE
BYTE * WriteByte(BYTE * pData,BYTE cbValue);
BYTE * WriteByte(BYTE * pData,BYTE cbValue,WORD & wWriteLen);


//读字
BYTE * ReadWord(BYTE * pData,WORD & wValue);
const BYTE * ReadWord(const BYTE * pData,WORD & wValue);
BYTE * ReadWord(BYTE * pData,WORD & wValue,WORD & wReadLen);
const BYTE * ReadWord(const BYTE * pData,WORD & wValue,WORD & wReadLen);

//写字
BYTE * WriteWord(BYTE * pData,WORD wValue);
BYTE * WriteWord(BYTE * pData,WORD wValue,WORD & wWriteLen);


//读双字
BYTE * ReadDword(BYTE * pData,DWORD & dwValue);
const BYTE * ReadDword(const BYTE * pData,DWORD & dwValue);
BYTE * ReadDword(BYTE * pData,DWORD & dwValue,WORD & wReadLen);
const BYTE * ReadDword(const BYTE * pData,DWORD & dwValue,WORD & wReadLen);

//读int16
BYTE * ReadInt16(BYTE * pData,__int16 & nValue);
const BYTE * ReadInt16(const BYTE * pData,__int16 & nValue);
BYTE * ReadInt16(BYTE * pData,__int16 & nValue,WORD & wReadLen);

//读int32
BYTE * ReadInt32(BYTE * pData,__int32 & nValue);
const BYTE * ReadInt32(const BYTE * pData,__int32 & nValue);
BYTE * ReadInt32(BYTE * pData,__int32 & nValue,WORD & wReadLen);
const BYTE * ReadInt32(const BYTE * pData,__int32 & nValue,WORD & wReadLen);

//写双字
BYTE * WriteDword(BYTE * pData,DWORD dwValue);
BYTE * WriteDword(BYTE * pData,DWORD dwValue,WORD & wWriteLen,bool bHtohl=true);

//写int16
BYTE * WriteInt16(BYTE * pData,__int16 nValue);
BYTE * WriteInt16(BYTE * pData,__int16 nValue,WORD & wWriteLen);
//写int32
BYTE * WriteInt32(BYTE * pData,__int32 nValue);
BYTE * WriteInt32(BYTE * pData,__int32 nValue,WORD & wWriteLen);


//读64位数
BYTE * ReadUNDword(BYTE * pData,UNDWORD & undwValue);
const BYTE * ReadUNDword(const BYTE * pData,UNDWORD & undwValue);
BYTE * ReadUNDword(BYTE * pData,UNDWORD & undwValue,WORD & wReadLen);
const BYTE * ReadUNDword(const BYTE * pData,UNDWORD & undwValue,WORD & wReadLen);

//写64位数
BYTE * WriteUNDword(BYTE * pData,UNDWORD undwValue);
BYTE * WriteUNDword(BYTE * pData,UNDWORD undwValue,WORD & wWriteLen);

//读取指定长度
BYTE * ReadToBuffer(BYTE * pData,void * pDest,WORD wLength);
const BYTE * ReadToBuffer(const BYTE * pData,void * pDest,WORD wLength);
BYTE * ReadToBuffer(BYTE * pData,void * pDest,WORD wLength,WORD & wReadLen);
const BYTE * ReadToBuffer(const BYTE * pData,void * pDest,WORD wLength,WORD & wReadLen);

//写指定长度
BYTE * WriteFromBuffer(BYTE * pDestBuffer,const void *pSrcData,WORD wLength);
BYTE * WriteFromBuffer(BYTE * pDestBuffer,const void *pSrcData,WORD wLength,WORD & wWriteLen);

//生成命令
CMD_Command BuildCommand(WORD wMainCmd,WORD wSubCmd,DWORD dwSequenceID);
//生成命令字
DWORD BuildCommandID(WORD wMainCmd,WORD wSubCmd);


#endif //__DEIRLYM_PROTOCOL_FUNCTION_HEAD_FILE__
