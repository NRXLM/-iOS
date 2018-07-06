///////////////////////////////////////////////////////////////////////////////////////
//			全局定义头文件 localdef.h
///////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_JNI_LOCALDEF_HEAD_FILE__
#define __BOWEN_HU_JNI_LOCALDEF_HEAD_FILE__
#define MACIOS	1

#include <iostream>
#if MACIOS
#include "../globaldef/globaldef.h"
#include "../globaldef/MacThread.h"
#include "../globaldef/ProtocolFunction.h"
#include "custom_log.h"
#else
#include "../../../../globaldef/globaldef.h"
#include "../../../../globaldef/LinuxThread.h"
#include "../../../../globaldef/ProtocolFunction.h"
#include "../../../../globaldef/MemManage.h"
#endif

#if !MACIOS
#define ANDROID_OS
#endif

using namespace std;
    
#define IS_ANDROID			1				//Andoird系统
///////////////////////////////////////////////////////////////////////////////////////////////

//配置信息结构
typedef struct tagConfigData
{
	MYBOOL		bUpHanding;					//教师要求举手标志
	DWORD		dwUserID;					//用户ID
	DWORD		dwDstRoomID;				//目标教室ID
	DWORD		dwRoomIndex;				//教室SOCK索引
	DWORD		dwTeacherID;				//教师ID
	MYBOOL		bIsSavePwd;					//是否保存密码
	MYBOOL		bOpenGlEs20;				//是否使用GLES
	WORD		wUserType;					//用户类型(0=学生,1=教师,2=家长)
	WORD		wClientType;				//客户端类型
	WORD		wServerPort;				//登录服务器端口号
	WORD		wCtrlSvrPort;				//控制服务器端口
	WORD		wTeachSvrPort;				//教学服务器端口
	WORD		wLBSvrPort;					//负载均衡服务器端口
	DWORD		dwCtrlSvrIP;				//控制服务器IP地址
	CHAR		szCtrlSvrUrl[BUFF_SIZE];	//控制服务器Url
	CHAR		szTeachSvrUrl[BUFF_SIZE];	//教学服务器Url
	CHAR		szLBSvrUrl[BUFF_SIZE];		//负载均衡服务器URL
	CHAR		szCheckCode[CHECKCODE_MAXLEN];	//CheckCode
	CHAR		szServerUrl[BUFF_SIZE];		//登录服务器URL		
	CHAR		szUserName[BUFF_SIZE];		//用户名
	CHAR		szPassword[BUFF_SIZE];		//登录密码
	CHAR		szRealName[NAME_LEN];		//用户真实姓名
	TCHAR		szwRealName[NAME_LEN];		//用户真实姓名

}CONFIGDATA,*PCONFIGDATA;

///////////////////////////////////////////////////////////////////////
#if !MACIOS
//调用Java 层消息处理函数
bool OnJavaMsgSink(const char *pMsgJson,int nMsgType);
//调用Java 层黑板事件处理函数
bool OnJavaBoardSink(const char *pMsgJson,int nMsgType);
//调用Java 层操作事件处理函数
bool OnJavaOperateSink(const char *pMsgJson,int nMsgType);
//上传线数据(nSrcType=0教师,nSrcType=1学生)
bool OnJavaLineData(const void * pLineData,int nDataSize,int nSrcType);
//发送视频数据到Java层
bool OnJavaVideoData(const void * pVideoData,int nDataSize,int nWidth,int nHeight);
//错误提示通知Java层
bool OnJavaErrMsg(const char *pMsgJson);
#endif

//日志打印
void NetMsgNotify(LPCTSTR lpszMsg,ITEOPTIONTYPE nOptType=OT_NOOPTION);
//消息通知
void NetMsgAutoParam(ITEOPTIONTYPE nOptType,LPCTSTR lpszFormate,...);
/////////////////////////////////////////////////////////////////////////
//黑板管理接口
struct IBlackboardManage
{
	//黑板事件数据
	virtual bool __cdecl OnBlackboardEventData(WORD beid,void *pEventData,DWORD dwDataSize,DWORD dwTimeStamp)=0;
	//操作事件数据
	virtual bool __cdecl OnRevOperateEventData(OPEREVENTID beid,void *pEventData,DWORD dwDataSize,DWORD dwTimeStamp)=0;
};

//音视频数据接口
struct IMediaDataManage
{
	//音频数据
	virtual bool __cdecl OnAudioRTData(const void *pAudioData,DWORD dwDataSize,DWORD dwTimeStamp)=0;
	//视频H264SpsPps数据
	virtual bool __cdecl OnH264SpsPpsData(DWORD dwChannelID,const void * pSpsPps,DWORD dwDataSize)=0;
	//H264视频编码数据
	virtual bool __cdecl OnVideoH264CodeData(DWORD dwChannelID,const void *pH264Data,DWORD dwDataSize)=0;
	//发送H264编码数据
	virtual bool __cdecl SendH264CodeData(const void *pH264Data,DWORD dwDataSize)=0;
	//错误提示信息
	virtual bool __cdecl OnErrMsgNotify(int nErrCode,LPCTSTR lpszMsgTxt)=0;
};

//////////////////////////////////////////////////////////////////////////////////
extern CONFIGDATA	g_ConfigParam;			//配置参数

#endif //__BOWEN_HU_JNI_LOCALDEF_HEAD_FILE__
