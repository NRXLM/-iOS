//////////////////////////////////////////////////////////////////////////////////////
//		网络客户端回调管理类实现文件 SocketSinkManage.cpp
//		bowen 2014-2-20
//////////////////////////////////////////////////////////////////////////////////////
#include "SocketSinkManage.h"
#include "mul_timer.h"

////////////////////////////////////////////////////////////////////////////////////
CONFIGDATA g_ConfigParam;
CSocketSinkManage *CSocketSinkManage::m_pSelfObject=NULL;
//////////////////////////////////////////////////////////////////////////////////////
CSocketSinkManage::CSocketSinkManage(void)
{
	m_pTeachSockClient=NULL;
	m_pIKernelDataSink=NULL;
	m_pSelfObject=NULL;
	m_dwLastTime=0;
}

CSocketSinkManage::~CSocketSinkManage(void)
{
	CMulTimer::ReleaseMulTimerInstance();
}

//初始化网络回调类
bool CSocketSinkManage::InitSocketSinkManage(LPCTSTR lpszLBSvrIp,WORD wLBSvrPort)
{
	g_ConfigParam.wTeachSvrPort=wLBSvrPort;
	lstrcpy(g_ConfigParam.szTeachSvrUrl,lpszLBSvrIp);

	if(!m_TeachSocketSink.InitSocketSink())
	{
		NetMsgNotify(TEXT("初始化教学服务器网络客户端失败"));
		return false;
	}

	IMediaDataManage *pIMediaDataManage=(IMediaDataManage *)&m_MediaDataManage;

	ISockSinkManage *pISockSinkManage=(ISockSinkManage *)this;
	
	m_TeachSocketSink.InitTeachSockClient(pIMediaDataManage);
	
	m_pTeachSockClient=m_TeachSocketSink.GetSocketInterface();
	ASSERT(m_pTeachSockClient!=NULL);

	g_ConfigParam.wClientType=CT_STUDENT_IOS;

	bool bInited=m_MediaDataManage.InitMediaManage();

	g_ConfigParam.dwDstRoomID=11;
	g_ConfigParam.dwUserID=1000;
	g_ConfigParam.wClientType=UT_TEACHER;
	lstrcpy(g_ConfigParam.szUserName,TEXT("mav-demo"));
	lstrcpy(g_ConfigParam.szCheckCode,TEXT("12312312312312311231231231231231"));

//	/*#ifdef ANDROID_OS
//	bInited=m_AudioManager.InitAudioManage();
//	IAudioManager *pIAudioManager=(IAudioManager *)&m_AudioManager;
//	m_MediaDataManage.SetAudioManage(pIAudioManager);
//	IAudioDataSink *pIAudioDataSink=(IAudioDataSink *)this;
//	m_AudioManager.SetAudioDataSink(pIAudioDataSink);
//	#endif*/
//
//	return bInited;
//}
// 
////开始网络连接
//bool CSocketSinkManage::ConnectToServer()
//{	
//	NetMsgNotify("--------CSocketSinkManage::ConnectToServer....");
//	
//	return m_TeachSocketSink.ConnectToTeachServer();
//}
//
////开始网络服务
//bool CSocketSinkManage::StartNetService()
//{
//	if(m_pTeachSockClient==NULL) return false;
//	bool bSuccess=m_pTeachSockClient->StartSocketThread();
//	if(!bSuccess) return false;
//	bSuccess=m_MediaDataManage.StartMediaManage();
//
//	NetMsgAutoParam(OI_EXCEPTION,"---->>>>>>----CSocketSinkManage::StartNetService result:%d",bSuccess);
//
//	return true;
//}
//
////停止网络服务
//bool CSocketSinkManage::StopNetService()
//{
//	if(m_pTeachSockClient!=NULL)
//	{
//		m_pTeachSockClient->StopSocketThread();
//	}
//	
//	NetMsgNotify(TEXT("--------->>>>>---Start Stop	MediaDataManage ...... "));
//	m_MediaDataManage.StopMediaManage();
//	
//	NetMsgNotify(TEXT("--------->>>>>---Start Stop	CSocketSinkManage end"));
//	return true;
//}
//
////停止网连接
//void CSocketSinkManage::CloseSocketConnect()
//{
//	if(m_pTeachSockClient!=NULL && m_pTeachSockClient->GetConnectState()!=SocketState_NoConnect)
//	{
//		m_pTeachSockClient->CloseSocket(false);
//	}
//}
//
////发送数据到AVSERVER
//bool CSocketSinkManage::SendH264Data(const void * pH264Data,WORD wDataSize,H264VTYPE vType)
//{
//	if(m_pTeachSockClient==NULL) return false;
//	WORD wSubCmd=TSVR_STUDENT_VIDEO;
//	if(vType==HVT_SPSPPS)
//	{
//		wSubCmd=TSVR_S_H264_SPS;
//	}
//
//	return m_TeachSocketSink.SendData(TEACH_MAIN_CMD,wSubCmd,0,(void *)pH264Data,wDataSize);
//}
//
////发送数据到消息服务器
//bool CSocketSinkManage::SendTextData(const void * pData,WORD wDataSize)
//{
//	return m_TeachSocketSink.SendData(TEACH_MAIN_CMD,TSVR_SHARE_SCREEN,0,(void *)pData,wDataSize);
//}
//
////发送音频数据
//bool CSocketSinkManage::SendAudioData(const void * pAudioData,WORD wDataSize)
//{
//	return m_TeachSocketSink.SendData(TEACH_MAIN_CMD,TSVR_TEACHER_AUDIO,0,(void *)pAudioData,wDataSize);
//}
//
////创建实例
//CSocketSinkManage *CSocketSinkManage::GetInstance()
//{
//	if(m_pSelfObject!=NULL) return m_pSelfObject;
//
//	m_pSelfObject=new CSocketSinkManage();
//
//	return m_pSelfObject;
//}
//
////释放实例
//void CSocketSinkManage::ReleaseInstance()
//{
//	if(m_pSelfObject==NULL) return;
//
//	delete m_pSelfObject;
//	m_pSelfObject=NULL;
//}
//
////释放网络对象
//void CSocketSinkManage::FreeSockSinkManage()
//{
//	CSocketSinkManage::FreeSockSinkManage();
//}
//  
////检测网络连接
//void CSocketSinkManage::CheckNetConnect()
//{
//	m_TeachSocketSink.OnProcActiveTest(m_pTeachSockClient);
//}
//
////退出登录
//void CSocketSinkManage::LoginOut()
//{
//	m_TeachSocketSink.CloseTeachConnect();
//}
//
////登录状态
//bool CSocketSinkManage::LoginStatus()
//{
//	return m_TeachSocketSink.GetLoginStatus();
//}
//
////设置底层回调接口
//bool CSocketSinkManage::SetKernelDataSink(void * pIKernelDataSink)
//{
//	return m_TeachSocketSink.SetKernelDataSink(pIKernelDataSink);
//}
//
//////////////////////////////////////////////////////////////////
//#ifdef __IPHONE_4_0
//ISockSinkManage *AfxCreateSinkManage()
//{
//    CSocketSinkManage *pSockManage=CSocketSinkManage::GetInstance();
//    return (ISockSinkManage *)pSockManage;
//}
//#endif	//__IPHONE_4_0
//
///////////////////////////////////////////////////////////////////////////////

