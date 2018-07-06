////////////////////////////////////////////////////////////////////////////////////
//			黑板事件数据类头文件 BoardEventData.h
//			2016-08-13 Bowen.hu
/////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_BOARD_EVENT_DATA_HEAD_FILE__
#define __BOWEN_HU_BOARD_EVENT_DATA_HEAD_FILE__

#include "ClientModule.h"
#include "localdef.h"

/////////////////////////////////////////////////////////////////////////////////////
class CBoardEventData: public IBlackboardManage
{
private:
	IBoardEventSink				*m_pIBoardEventSink;		//黑板事件接口

public:
	CBoardEventData(void);
	~CBoardEventData(void);

public:
	//设置黑板数据回调接口
    bool SetBoardDataSink(IBoardEventSink *pIBoardEventSink);

public:
	//黑板事件数据
	virtual bool __cdecl OnBlackboardEventData(WORD beid,void *pEventData,DWORD dwDataSize,DWORD dwTimeStamp);
	//操作事件数据
	virtual bool __cdecl OnRevOperateEventData(OPEREVENTID beid,void *pEventData,DWORD dwDataSize,DWORD dwTimeStamp);

private:
	//网络添加黑板元素
	void NetAddBoardElement(const void *pEventData,DWORD dwDataSize);
	//网络添加线型元素
	void NetChangelLineStyle(const void *pEventData,DWORD dwDataSize);
	//图片处理
	void NetProcImageData(BOARDEVENTID beid,const void *pEventData,DWORD dwDataSize);
	//处理黑板事件
	void NetProcNoDataEvent(int beid);
	//处理整型参数事件
	void NetProcIntEvent(int beid,const void *pEventData,DWORD dwDataSize);
	//处理黑板大小
	void NetProcBoardSize(BOARDEVENTID beid,const void *pEventData,DWORD dwDataSize);
	//处理DWORD型参数事件
	void NetProcDwordEvent(int beid,const void *pEventData,DWORD dwDataSize);
	//处理选择学生提问事件
	void NetProcSelectedUser(int beid,const void *pEventData,DWORD dwDataSize);
	//处理客户端播放事件
	void NetProcClientPlay(int eid,const void *pEventData,DWORD dwDataSize);
	//处理开始上课操作
	void OnProcStartClass(int eid,const void *pEventData,DWORD dwDataSize);
	//课件播放开始上课
	bool OnPlaycwdStartClass(const void *pEventData,DWORD dwDataSize);
};

#endif	//__BOWEN_HU_BOARD_EVENT_DATA_HEAD_FILE__

////////////////////////////////////////////////////////////////////////////////////

