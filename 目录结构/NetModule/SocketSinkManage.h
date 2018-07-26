////////////////////////////////////////////////////////////////////////////////////////
////		网络客户端回调管理类头文件 SocketSinkManage.h
////		bowen 2014-2-20
////////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_SOCKET_SINK_MANAGE_HEAD_FILE__
#define __BOWEN_HU_SOCKET_SINK_MANAGE_HEAD_FILE__

#include "TeachSockClientSink.h"
#include "MediaDataManage.h"

#define HAVE_BBEVETN		1				//有黑板事件处理对象
////////////////////////////////////////////////////////////////////////////////////////
class CSocketSinkManage: public ISockSinkManage
{
private:
	bool						m_bInited;						//初始化标志
	BOOL						m_bLogined;						//创建标志
	BOOL						m_bBuilded;						//生成完成标志
	BOOL						m_bMainClose;					//主进程关闭标志
	HWND						m_hRecordWnd;					//录屏窗口
	int							m_nWndCX;						//窗口宽
	int							m_nWndCY;						//窗口高
	DWORD						m_dwAvsConnNum;					//Avserver连接次数
	CONNECTSVRTYPE				m_cst;							//连接服务器类型

private:
	CSnapVideoManageHelper		m_SnapVideoManageHelper;		//截屏视频辅助类对象
	ISnapVideoManage*			m_pISnapVideoManage;			//截屏接口

private:
	CSingleSockClientHelper		m_SocketClientHelper;			//网络组件
	IClientSocket				*m_pIClientSocket;				//网络接口
	IShowLogToWnd				*m_pIShowLogToWnd;				//日志接口
	IAudioModuleManage			*m_pIAudioModuleManage;			//音频管理接口

private:
	int							m_nIndex;						//文件索引
	DWORD						m_dwStartTime;					//开始时间
	HANDLE						m_hVideoFile;					//视频文件
	CThreadLock					m_ThreadLock;					//锁对象
	CFlvToFile					m_FlvToFile;					//Flv文件对象

private:
	VIDEOCWDATA					m_videoCwData;					//视频课件数据
	TCHAR						m_szFileName[MAX_PATH];			//文件名

private:
	BYTE						*m_pSpsData;					//Sps数据
	DWORD						m_dwSpsSize;					//Sps数据大小
	POINT						m_veOffset;						//图像偏移量

private:
	//请求服务器地址
	bool RequestSvrIPPort();
	//登录到配置服务器
	bool LoginToAvsSvr();
	//创建视频文件
	bool WriteVideoToFile(const void *pVideoData,DWORD dwDataSize,AVCPTYPE avType);
	//切换负载IP
	void SwitchLoadServerIpPort();
	//开始音频模块
	bool StartAudioModule();


};

#endif //__BOWEN_HU_SOCKET_SINK_MANAGE_HEAD_FILE__

