//////////////////////////////////////////////////////////////////////////////////////////
//		Õ¯¬Á ˝æ›Õ¨≤Ω¿‡Õ∑Œƒº˛ NetDataSyncSink.h
//		Author:	bowen.hu
//		Time:	2016-06-09
///////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ClientSocket.h"
#include "localdef.h"
#include "TimerEngine.h"

///////////////////////////////////////////////////////////////////////////////////////////
class CNetDataSyncSink: public IClientSocketSink,public ITimerSink,public IMediaDataSink
{
private:
	MYBOOL						m_bLogined;					//µ«¬º±Í÷æ
	DWORD						m_dwChannelID;				//∆µµ¿ID
	DWORD						m_dwRoomIndex;				//∑øº‰ID
	DWORD						m_dwUserID;					//”√ªßID
	DWORD						m_dwAvsConnNum;					//Avserver¡¨Ω”¥Œ ˝
	CONNECTSVRTYPE				m_cst;						//¡¨Ω”∑˛ŒÒ∆˜¿‡–Õ
	WORD						m_wVssPort;					// ”∆µ∑˛ŒÒ∆˜∂Àø⁄
	char						m_szVssIp[IPLENGHT];		// ”∆µ∑˛ŒÒ∆˜ip
	char						m_szMediaPath[MAX_PATH];	// ”≤ΩŒƒº˛¬∑æ∂
	
private:
	CClientSocket				m_ClientSocket;				//Õ¯¬Á∂‘œÛ
	IClientSocket				*m_pIClientSocket;			//Õ¯¬ÁΩ”ø⁄
	INetNotifySink				*m_pINetNotifySink;			//Õ¯¬ÁÕ®÷™Ω”ø⁄
	INetDataSink                *m_pINetDataSink;           //网络通知接口
	
private:
	BYTE						*m_pSpsData;					//Sps ˝æ›
	DWORD						m_dwSpsSize;					//Sps ˝æ›¥Û–°
	POINT						m_veOffset;						//ÕºœÒ∆´“∆¡ø
	
	
public:
	CNetDataSyncSink(void);
	~CNetDataSyncSink(void);
	
	
public:
	//Socket connection message
	virtual bool OnSocketConnect(int iErrorCode,LPCTSTR pszErrorDesc,IClientSocket *pIClientSocket);
	//Socket read message
	virtual bool OnSocketRead(CMD_Command Command,void * pBuffer,DWORD dwDataSize,IClientSocket *pIClientSocket);
	//Socket close message
	virtual bool OnSocketClose(IClientSocket *pIClientSocket,bool bCloseByServer);
	//∂® ±∆˜ ¬º˛
	virtual bool OnEventTimer(WORD wTimerID,WPARAM wBindParam);
	
public:
	//≥ı ºªØœﬂ≥Ã
	bool InitDataSyncThread(INetNotifySink *pINetNotifySink,const char *pMediaPath);
	//¡¨Ω”µΩLB∑˛ŒÒ∆˜
	bool ConnectToServer(CONNECTSVRTYPE cst);
	//∆Ù∂ØÕ¯¬Áœﬂ≥Ã
	bool StartNetThread();
	//πÿ±’Õ¯¬Á¡¨Ω”
	bool CloseConnect();
	//∑¢ÀÕ ”∆µ
	bool SendVideoData(BYTE *pVideoData,int nDataSize);
	//∑¢ÀÕ“Ù∆µ
	bool SendAudioData(BYTE *pAudioData,int nDataSize);
	//∑¢ÀÕŒƒ±æ ˝æ›
	bool SendTextData(const char *pTxtData);
	//设置网络数据接口
	bool SetNetDataSink(INetDataSink *pINetDataSink);
    //状态通知
    bool OnNotifyScreenCap(int nEventID);
	
public:
	//“Ù∆µ ˝æ›ªÿµ˜
	virtual bool OnAacAudioData(BYTE *pAacData,int nDataSize);
	// ”∆µ ˝æ›ªÿµ˜
	virtual bool On264VideoData(BYTE *pVideoData,int nDataSize);
	// ”∆µSPS+PPSÕ∑ ˝æ›
	virtual bool OnS264SpsPpsData(BYTE *pHeadData,int nDataSize);
	
private:
	//«Î«Û∑˛ŒÒ∆˜µÿ÷∑
	bool RequestSvrIPPort();
	//µ«¬ºµΩ∑˛ŒÒ∆˜
	bool LoginToAvsSvr();
	//«–ªª∏∫‘ÿIP
	void SwitchLoadServerIpPort();
	
	
	//œ˚œ¢¥¶¿Ì∫Ø∑È
private:
	//¥¶¿Ì–ƒÃ¯≤‚ ‘
	bool OnProcActiveTest(IClientSocket * pIClientSocket);
	//¥¶¿Ì«Î«Û∑˛ŒÒ∆˜µÿ÷∑Response
	bool OnProcRequestSvrIPPortResp(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//¥¶¿Ìµ«¬º∑˛ŒÒ∆˜Response
	bool OnProcLoginToServerResp(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//¥¶¿Ì‘∂≥Ãøÿ÷∆ ¬º˛ ˝æ›
	bool OnProcCtrlEventData(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//¥¶¿ÌÕ£÷π∏®÷˙∑˛ŒÒÕ®÷™
	bool OnProcStopHelperNotify(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//¥¶¿ÌøÕªß“Ù∆µ
	bool OnProcSerivceAudio(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
};


