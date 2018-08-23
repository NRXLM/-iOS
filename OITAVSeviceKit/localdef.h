///////////////////////////////////////////////////////////////////////////////////////
//			»´æ÷∂®“ÂÕ∑Œƒº˛ localdef.h
///////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_JNI_LOCALDEF_HEAD_FILE__
#define __BOWEN_HU_JNI_LOCALDEF_HEAD_FILE__
#define MACIOS	1

#include <iostream>
#if MACIOS
#include "globaldef.h"
#include "MacThread.h"
#include "ProtocolFunction.h"
#include "custom_log.h"

#else
#include "../../../../globaldef/globaldef.h"
#include "../../../../globaldef/LinuxThread.h"
#include "../../../../globaldef/ProtocolFunction.h"
#include "../../../../globaldef/MemManage.h"
#endif

#if !MACIOS
#define ANDROID_OS
#else
#define LOGE printf
//#undef LOGE
//#define LOGE
#endif

using namespace std;
    
#define IS_ANDROID			0				//AndoirdœµÕ≥

/////////////////////////////////////////////////////////////////////////////////////////////
#define V_WIDTH         640
#define V_HEIGHT        480

#define BUFF_SIZE		64				//ª∫≥Â¥Û–°
#define RECONN_TIME		5				//÷ÿ¡¨ ±º‰(s)
#define SVR_URL			"192.168.9.10"
#define SVR_PORT		8990

#define DT_NET_MODE		1				//Õ¯¬Á ˝æ›¥´ ‰ƒ£ Ω
#define DT_TO_FILE		0

///////////////////////////////////////////////////////////////////////////
#define IPLENGHT			32			//IP≥§∂»
#define LBSMAXNUM			4			//∏∫‘ÿ∑˛ŒÒ∆˜◊Ó¥Û ˝

//∂® ±∆˜∂®“Â
#define IDT_CONN_SERVER					0x9001					//¡¨Ω”µΩ∑˛ŒÒ∆˜id

////////////////////////////////////////////////////////////
//∏∫‘ÿ∑˛ŒÒ∆˜–≈œ¢
typedef struct tagLbsInfo
{
    WORD			wConnCount;				//¡¨Ω”¥Œ ˝
    WORD			wLbsPort;				//∏∫‘ÿ∑˛ŒÒ∆˜∂Àø⁄
    TCHAR			szLbsIp[IPLENGHT];		//∑˛ŒÒ∆˜IP
}LBSINFO;

////////////// FLV ∂®“Â ///////////////////////////////////////////////////
#define MYTRACE TRACE

#define DST_V_CX		1280//1600	//1280		//ƒø±Í ”∆µøÌ
#define DST_V_CY		720//900		//720			//ƒø±Í ”∆µ∏ﬂ

//////////////////////////////////////////////////////////////////////////////
#define SAMPLE_RATE		16000		//44100
#define CHANNEL_NUM		1
#define AAC_MAIN		1			//AAC-MAIN
#define AAC_LC			2			//AAC-LC

///////////////////////////////////////////////////////////////////////////////////////////////

//≈‰÷√–≈œ¢Ω·ππ
typedef struct tagConfigData
{
    int         nBusinessType;              //业务线类型
    int         vcx;
    int         vcy;
	MYBOOL		bHaveVoice;					//有音频
    DWORD       dwServiceCode;              //客服号
	DWORD		dwClientCode;				//客户号
    LBSINFO     mLbsInfo;                   //负载服务器信息
    WORD        wLbsCount;                  //负载服务器数量
    WORD        wReReqJobCount;             //重复派工次数
    WORD        wReqJobFaildCount;          //派工失败次数
    CHAR        szJobNum[WNAME_LEN];        //工号
    CHAR        szMobile[NAME_LEN];         //手机号
	CHAR		szCheckCode[CHECKCODE_MAXLEN];	//CheckCode
	CHAR		szServerUrl[BUFF_SIZE];		//µ«¬º∑˛ŒÒ∆˜URL
    LBSINFO		mLbsList[LBSMAXNUM];
    TCHAR       szExtVal[WNAME_LEN];

}CONFIGDATA,*PCONFIGDATA;

///////////////////////////////////////////////////////////////////////
#if !MACIOS
//µ˜”√Java ≤„œ˚œ¢¥¶¿Ì∫Ø ˝
bool OnJavaMsgSink(const char *pMsgJson,int nMsgType);
//µ˜”√Java ≤„∫⁄∞Â ¬º˛¥¶¿Ì∫Ø ˝
bool OnJavaBoardSink(const char *pMsgJson,int nMsgType);
//µ˜”√Java ≤„≤Ÿ◊˜ ¬º˛¥¶¿Ì∫Ø ˝
bool OnJavaOperateSink(const char *pMsgJson,int nMsgType);
//…œ¥´œﬂ ˝æ›(nSrcType=0ΩÃ ¶,nSrcType=1—ß…˙)
bool OnJavaLineData(const void * pLineData,int nDataSize,int nSrcType);
//∑¢ÀÕ ”∆µ ˝æ›µΩJava≤„
bool OnJavaVideoData(const void * pVideoData,int nDataSize,int nWidth,int nHeight);
//¥ÌŒÛÃ· æÕ®÷™Java≤„
bool OnJavaErrMsg(const char *pMsgJson);
#endif

//»’÷æ¥Ú”°
void NetMsgNotify(LPCTSTR lpszMsg,ITEOPTIONTYPE nOptType=OT_NOOPTION);
//œ˚œ¢Õ®÷™
void NetMsgAutoParam(ITEOPTIONTYPE nOptType,LPCTSTR lpszFormate,...);
/////////////////////////////////////////////////////////////////////////
//音视频数据接口
struct IMediaDataSink
{
    //“Ù∆µ ˝æ›ªÿµ˜
    virtual bool OnAacAudioData(BYTE *pAacData,int nDataSize)=0;
    // ”∆µ ˝æ›ªÿµ˜
    virtual bool On264VideoData(BYTE *pVideoData,int nDataSize)=0;
    // ”∆µSPS+PPSÕ∑ ˝æ›
    virtual bool OnS264SpsPpsData(BYTE *pHeadData,int nDataSize)=0;
    
};




//音视频头数据接口
struct IVideoAudioHead
{
    //ªÒ»°x264Õ∑
    virtual int GetSpsPpsData(BYTE *pHeadData,int nBufferSize)=0;
    //ªÒ»°AACÕ∑
    virtual int GetAacHeadData(BYTE *pHeadData,int nBufferSize)=0;
};

//网络通知接口
struct INetNotifySink
{
    //ø™ º ”∆µ≤…ºØ
    virtual bool StartAVCapThread()=0;
    //ø™ º∆Ù∂Ø“Ù∆µ◊Èº˛
    virtual bool StartAudioService()=0;
	
	virtual bool StopAVCapThread()=0;
	virtual bool StopAudioSevice()=0;
	
};


//网络数据通知接口
struct INetDataSink
{
	//音频数据通知
	virtual bool AudioDataNotify(const BYTE *pAudioData,DWORD dwDataSize)=0;
	//网络状态通知
	virtual bool NetStatusNotify(int nEventID)=0;
};



//////////////////////////////////////////////////////////////////////////////////
extern CONFIGDATA	g_GlobalData;			//≈‰÷√≤Œ ˝

#endif //__BOWEN_HU_JNI_LOCALDEF_HEAD_FILE__
