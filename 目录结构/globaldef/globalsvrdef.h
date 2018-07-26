//////////////////////////////////////////////////////////////////////////////////////////
//		本地数据定义头文件 globaldef.h
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_OIT_SVR_GLOBAL_DEFINE_HEAD_FILE__
#define __DEIRLYM_OIT_SVR_GLOBAL_DEFINE_HEAD_FILE__

#include "IUnknownEx.h"


////////////////////////////////////////////////////////////////////////////////////////////////
//获取用户数据管理接口
struct IUserDataManage
{
	//获取当前本机注册的用户列表
	virtual DWORD __cdecl GetLocalRegUserList(void * pUserList)=0;
	//获取用户总数
	virtual bool __cdecl GetUserCount(DWORD &dwUserCount)=0;
	//打印用户信息
	virtual void __cdecl PrintUserInfo(DWORD dwUserID)=0;
};

#endif //__DEIRLYM_OIT_SVR_GLOBAL_DEFINE_HEAD_FILE__
