//////////////////////////////////////////////////////////////////////////////////////////
//		本地数据定义头文件 globaldef.h
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_OIT_GLOBAL_DEFINE_HEAD_FILE__
#define __DEIRLYM_OIT_GLOBAL_DEFINE_HEAD_FILE__

#include "IUnknownEx.h"
#include <list>
#include <map>
#include <vector>
#include <string>
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////
// define log file's categroy
#define LT_SYS				0x00000001								// system information log
#define LT_FAL				0x00000002								// fail log
#define LT_SUC				0x00000004								// success log
#define LT_GEO				0x00000008								// exception/error log
#define LT_REQ              0x00000010								// request log
#define LT_REP              0x00000020								// response log
#define SEND_VALUE          0xFFFFFFFF								// send flag value

#define ST_TIME             0x00000001								// set time
#define ST_LEN              0x00000002								// set file length

/////////////////////////////////////////////////////////
#define LOG_NOR_COLOR		RGB(255,255,255)		//日志正常颜色
#define LOG_WARN_COLOR		RGB(255,255,0)			//日志颜色
#define LOG_EXCP_COLOR		RGB(255,0,0)			//日志颜色
#define LOG_DEBUG_COLOR		RGB(0,0,255)			//日志颜色
#define LOG_FLUSH_COLOR		RGB(0,255,0)			//闪烁颜色
//////////////////////////////////////////////////////////////////////////////////////////
//版本号
#define MAKEVER(m,s)	((m<<8)|(s))	//生成版本号
//程序版本1.2
#define DVR_CAP_VER		MAKEVER(2,1)	//DVR 版本
#define CURR_NET_VER	MAKEVER(2,1)	//当前网络版本

#ifndef COUNTARRAY
#define COUNTARRAY
//计算数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
//计算字符长度
#define CountString(String) ((UINT)((lstrlen(String)+1)*sizeof(TCHAR)))
#endif

//协议包头结构
typedef struct CMD_Head
{
	union
	{
		DWORD					dwTotalLength;					//数据包总长
		struct
		{
			WORD				wPackLength;					//当前包长
			WORD				wPackNumber;					//当前包数(高8位包计数，低8位包总数)
		};
	};
	union
	{
		DWORD					dwCommandID;					//命令字
		struct
		{
			WORD				wMainCmd;						//主命令
			WORD				wSubCmd;						//字命令
		};
	};
	DWORD					dwSequenceID;						//序列号
	
}CMDHEAD;

//命令结构
typedef struct CMD_Command
{
	union
	{
		DWORD					dwCommandID;					//命令字
		struct
		{
			WORD				wMainCmd;						//主命令
			WORD				wSubCmd;						//字命令
		};
	};
	
	DWORD					dwSequenceID;						//序列号
	WORD					wPackNumber;						//当前包数(高8位包计数，低8位包总数)
	
}CMDCOMMAND;

//////////////////////////////////////////////////////////////////////////////////////////
//	网络数据定义
#define SOCKET_VER					2									//数据包版本
#define SOCKET_PACKAGE				(1024*4)    						//最大网络数据包
#define SOCKET_BUFFER				(SOCKET_PACKAGE+sizeof(CMD_Head)*2)	//网络缓冲大小

#define SOCKVALUE(v,i,r) {(v)=(i);(v)<<=16;(v)|=(r);}
#define SOCKINDEX(v,i,r) {(i)=(WORD)((v)>>16);(r)=(WORD)(v);}
#define SETSAHRE(v,s) {(v)=(v)|(((UNDWORD)s)<<32);}
#define DELSHARE(v) {(v)&=0xFFFFFFFF;}
#define SOCKDATA(v) ((v)&0xFFFFFFFF)
#define H64VALUE(v) ((v)>>32)

#define MAKEU64(h,l) ((((UNDWORD)h)<<32)|(l))
#define HIGHU64(v)	(v>>32)
#define LOWU64(v)	((DWORD)v)
////////////////////////////////////////////////////////////////////////////////////////
//定义网络命令字
//主命令字
#define ALL_MAIN_CMD				0x00						//通用主命令
#define SCT_MAIN_CMD				0x02						//状态控制服务器命令
#define AVS_MAIN_CMD				0x03						//媒体服务器命令
#define UPSVR_MAIN_CMD				0x04						//上传服务器主命令
#define CTLROOM_MAIN_CMD			0x05						//控制教室服务器命令

//////////////////////////////////////////////////////////////////////////////////////////
//子命令定义
//所有心跳测试子命令定义
#define ACITVE_TEST					0x0000						//心跳测试子命令
#define ACITVE_TEST_RESP			0x1000						//心跳测试子命令

/////////////////////////////////////////////////////////////////////////////////////////////
//流媒体服务器子命令
#define TSVR_SERVICE_LOGIN			0x0201						//客服录到教学服务器
#define TSVR_SERVICE_LOGIN_RESP		0x1201						//客服登录到教学服务器Response
#define TSVR_USER_LOGIN				0x0202						//用户登录到教学服务器
#define TSVR_USER_LOGIN_RESP		0x1202						//用户登录到教学服务器Response
#define TSVR_T_BB_EVENT				0x0203						//客服黑板事件
#define TSVR_T_BB_EVENT_RESP		0x1203						//客服黑板事件Response
#define TSVR_S_BB_EVENT				0x0204						//用户黑板事件
#define TSVR_S_BB_EVENT_RESP		0x1204						//用户黑板事件Response
#define TSVR_SERVICE_AUDIO			0x0205						//客服音频
#define TSVR_SERVICE_AUDIO_RESP		0x1205						//客服音频Response
#define TSVR_USER_AUDIO				0x0206						//用户音频
#define TSVR_USER_AUDIO_RESP		0x1206						//用户音频Response
#define TSVR_H264_SPS				0x0207						//客服h264 sps pps Request
#define TSVR_H264_SPS_RESP			0x1207						//客服h264 sps pps Response
#define TSVR_SERVICE_VIDEO			0x0208						//客服视频指令Request
#define TSVR_SERVICE_VIDEO_RESP		0x1208						//客服视频指令Response
#define TSVR_SHARE_SCREEN			0x0209						//屏幕共享通知Request
#define TSVR_SHARE_SCREEN_RESP		0x1209						//屏幕共享通知Response
#define TSVR_USER_SOCK_DATA			0x020A						//用户Socket数据
#define TSVR_USER_SOCK_DATA_RESP	0x120A						//用户Socket数据Response
#define TSVR_SET_BB_SIZE			0x020B						//设置黑板大小
#define TSVR_SET_BB_SIZE_RESP		0x120B						//设置黑板大小Response
#define TSVR_T_OPT_EVENT			0x020C						//客服操作事件
#define TSVR_T_OPT_EVENT_RESP		0x120C						//客服操作事件Response
#define TSVR_S_H264_SPS				0x020D						//用户h264 sps pps Request
#define TSVR_S_H264_SPS_RESP		0x120D						//用户h264 sps pps Response
#define TSVR_USER_VIDEO				0x020E						//用户视频指令Request
#define TSVR_USER_VIDEO_RESP		0x120E						//用户视频指令Response
#define TSVR_SWITCH_ROOM			0x020F						//切换教室指令
#define TSVR_SWITCH_ROOM_RESP		0x120F						//切换教室指令Response
#define TSVR_TEACH_EXIT_ROOM		0x0210						//客服退出教室指令
#define TSVR_TEACH_EXIT_ROOM_RESP	0x0210						//客服退出教室指令Response
#define TSVR_USER_EXIT_ROOM			0x0211						//用户用户退出教室指令
#define TSVR_USER_EXIT_ROOM_RESP	0x1211						//用户用户退出教室指令Response
#define TSVR_HELPER_CONFIRM			0x0212						//请求辅助确认
#define TSVR_HELPER_CONFIRM_RESP	0x1212						//请求辅助确认Response
#define TSVR_STOP_HELPER			0x0213						//请停辅助服务
#define TSVR_STOP_HELPER_RESP		0x0213						//请停辅助服务Response
#define TSVR_USER_TXTMSG			0x0214						//用户文本消息
#define TSVR_USER_TXTMSG_RESP		0x1214						//用户文本消息


//负载均衡服务器子命令
#define LBS_SVR_LOGIN				0x0300						//负载均衡服务器登录
#define LBS_SVR_LOGIN_RESP			0x1300						//负载均衡服务器登录Response
#define LBS_SC_REQ_SVRIP			0x0301						//客服请求IP地址
#define LBS_SC_REQ_SVRIP_RESP		0x1301						//客服请求IP地址Response
#define LBS_SVR_LB_DATA				0x0302						//服务器负载数据
#define LBS_SVR_LB_DATA_RESP		0x1302						//服务器负载数据Response
#define LBS_BSVR_REGISTER			0x0303						//业务服务器注册
#define LBS_BSVR_REGISTER_RESP		0x1303						//业务服务器注册Response
#define LBS_SC_LOGIN_NOTIFY			0x0304						//客服端登录通知
#define LBS_SC_LOGIN_NOTIFY_RESP	0x1304						//客服端登录客服端登录通知
#define LBS_LB_DATA_NOTIFY			0x0305						//负载服务器间数据通知
#define LBS_LB_DATA_NOTIFY_RESP		0x1305						//负载服务器间数据通知
#define LBS_CLIENT_RES_SVRIP		0x0306						//客户端请求IP地址
#define LBS_CLIENT_RES_SVRIP_RESP	0x1306						//客户端请求IP地址
#define LBS_LB_REMOVE_DATA			0x0307						//负载服务器间移除数据通知
#define LBS_LB_REMOVE_DATA_RESP		0x1307						//负载服务器间移除数据通知
#define LBS_SC_EXIT_NOTIFY			0x0308						//客服端退出通知
#define LBS_SC_EXIT_NOTIFY_RESP		0x1308						//客服端退出通知
#define LBS_CC_EXIT_NOTIFY			0x0309						//客户退出通知
#define LBS_CC_EXIT_NOTIFY_RESP		0x1309						//客户退出通知Response

//上传服务器子命令
#define ULF_SVR_CREATE_FILE			0x0401						//创建文件请求
#define ULF_SVR_CREATE_FILE_RESP	0x1401						//创建文件Response
#define ULF_SVR_FILE_CONTENT		0x0402						//文件内容
#define ULF_SVR_FILE_CONTENT_RESP	0x1402						//文件内容Response
#define ULF_SVR_FILE_FINISH			0x0403						//文件上传完成
#define ULF_SVR_FILE_FINISH_RESP	0x1403						//文件上传完成Response

/////////////////状态控制服务器//////////////////////////////////////////////////////////
//状态控制服务器子命令
#define SCSVR_SG_REGISTER			0x0601						//服务器守护进程注册
#define SCSVR_SG_REGISTER_RESP		0x1601						//服务器守护进程注册Response
#define SCSVR_REQ_CONFIG			0x0602						//请求配置信息
#define SCSVR_REQ_CONFIG_RESP		0x1602						//请求配置信息Response
#define SCSVR_UPDATE_STATUS			0x0603						//更新服务器状态
#define SCSVR_UPDATE_STATUS_RESP	0x1603						//更新服务器状态Response
#define SCSVR_UPDATE_SLSD			0x0606						//更新服务器统计数据
#define SCSVR_UPDATE_SLSD_RESP		0x1606						//更新服务器统计数据Response
#define SCSVR_CTRL_ASSSVR			0x0607						//控制媒体流服务器
#define SCSVR_CTRL_ASSSVR_RESP		0x1607						//控制媒体流服务器Response


//数据交换服务器命令
#define DSS_SUBSVR_LOGIN			0x0701						//子服务器登录
#define DSS_SUBSVR_LOGIN_RESP		0x1701						//子服务器登录Response
#define DSS_USER_LOGIN_NOTIFY		0x0702						//用户登录通知
#define DSS_USER_LOGIN_NOTIFY_RESP	0x1702						//用户登录通知Response
#define DSS_USER_EXIT_NOTIFY		0x0703						//用户退出通知
#define DSS_USER_EXIT_NOTIFY_RESP	0x1703						//用户退出通知Response
#define DDS_SWITCH_USER_MSG			0x0704						//交换好友消息
#define DDS_SWITCH_USER_MSG_RESP	0x1704						//交换好友消息Response
#define DDS_SWITCH_GROUP_MSG		0x0705						//交换群组消息
#define DDS_SWITCH_GROUP_MSG_RESP	0x1705						//交换群组消息Response
#define DDS_SWITCH_DATA				0x0706						//交换数据
#define DDS_SWITCH_DATA_RESP		0x1706						//交换数据Response
#define DDS_SWITCH_GDATA			0x0707						//交换群组数据
#define DDS_SWITCH_GDATA_RESP		0x1707						//交换群组数据Response

//用户关系服务器命令
#define URS_SUBSVR_LOGIN			0x0801						//子服务器登录
#define URS_SUBSVR_LOGIN_RESP		0x1801						//子服务器登录Response
#define URS_ULOGIN_NOTIFY			0x0802						//子服务器用户登录通知
#define URS_ULOGIN_NOTIFY_RESP		0x1802						//子服务器用户登录通知Response
#define URS_UEXIT_NOTIRY			0x0803						//子服务器用户退出通知
#define URS_UEXIT_NOTIRY_RESP		0x1803						//子服务器用户退出通知Response
#define URS_ADD_GROUP				0x0804						//添加群组
#define URS_ADD_GROUP_RESP			0x1804						//添加群组Response
#define URS_DEL_GROUP				0x0805						//删除群组
#define URS_DEL_GROUP_RESP			0x1805						//删除群组Response
#define URS_ADD_GUSER				0x0806						//添加群组用户
#define URS_ADD_GUSER_RESP			0x1806						//添加群组用户Response
#define URS_DEL_GUSER				0x0807						//删除群组用户
#define URS_DEL_GUSER_RESP			0x1807						//删除群组用户Response
#define URS_REQ_GROUP_USER			0x0808						//请求群用户信息
#define URS_REQ_GROUP_USER_RESP		0x1808						//请求群用户信息Response
#define URS_REQ_USER_INFO			0x0809						//请求用户信息
#define URS_REQ_USER_INFO_RESP		0x1809						//请求用户信息Response
#define URS_TRSDATA_REQ_USER		0x080A						//数据传输请求用户信息
#define URS_TRSDATA_REQ_USER_RESP	0x180A						//数据传输请求用户信息Response
#define URS_TRSDATA_REQ_GUSRE		0x080B						//数据传输请求群组用户信息
#define URS_TRSDATA_REQ_GUSRE_RESP	0x180B						//数据传输请求群组用户信息Response

////////////////////////////////////////////////////////////////////
#define SYS_MSG_BASE			60								//系统消息类型基数
#define SYS_ADVICE_MSG			50								//咨询消息
#define NOTIFY_MSG_BASE			200								//通知消息类型基数
///////////////////////////////////////////////////////////////////////////////////////
//错误码
#define SVR_MODULE_NOEXIST			1							//模块不存在
#define DB_SVR_EXPECT				-1							//服务器异常
#define NAME_PASS_ERR				-2							//用户名或密码错误
#define CTRL_LOGIN_ERR				-3							//服务器验证错误
#define ADD_BSVR_FAILD				-4							//添加业务服务器失败
#define CREATE_ROOM_FAILD			-5							//创建教室失败
#define ADD_TO_ROOM_FAILD			-6							//添加到教室失败
#define CREATE_FILE_FAILD			-7							//创建文件失败
#define WRITE_FILE_FAILD			-8							//写文件失败
#define CLIENT_TYPE_ERR				-9							//客户端类型错误
#define ROOM_NO_CLASS				-10							//教室没有上课
#define SUB_SVR_IPPORT_ERR			-11							//子服务器IP或端口非法
#define ADD_USER_FAILD				-12							//添加用户失败
#define ADD_GROUP_FAILD				-13							//添加群失败
#define USER_IS_OFFLINE				-16							//用户不在线
#define GROUP_NOT_EXIST				-17							//群不存在
#define SERVICE_NOT_EXIST			-18							//客服不存在
#define SERVICE_WORKING				-19							//客服正忙
#define NO_ENOUGH_MEM				-20							//没有足够的内存

////////////////////////////////////////////////////////////////////////////////////////
#define MAX_CHANNEL				4								//最大视频通道数
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#ifndef AfxDebugBreak
#ifndef MYASSERT
#define MYASSERT
#define AfxDebugBreak()    _asm { int 3 }
#ifndef _DEBUG
#define ASSERT(f) ((void)0)
#else
#define ASSERT(f) \
if (!(f)) \
((void)0);
#endif
#define VERIFY(f)          ASSERT(f)
#endif
#endif
#endif
//////////////////////////////////////////////////////////////
#define RECTWIDTH(rc) (rc.right-rc.left)
#define RECTHEIGHT(rc) (rc.bottom-rc.top)
/////////////////////////////////////////////////////////////////////
//字符串结构
struct my_str_t
{
	int      len;
	BYTE     *data;
};

#define CLEAR_MY_STR(str) {(str).data=NULL;(str).len=0;}
#define FREE_MY_STR(str) {if(str.data!=NULL){delete []str.data;str.data=NULL;str.len=0;}}
#define NEW_MY_STR(str,size) {if(str.len<size){if(str.data!=NULL){delete[] str.data;str.data=NULL;	\
str.len=0;}str.data=new BYTE[size];if(str.data==NULL) return; str.len=size;}}
#define SET_MY_STR(str,dat,size) {if(str.data!=NULL){	delete []str.data;	str.data=NULL;str.len=0;} \
str.data=new BYTE[size+1];if(str.data!=NULL){str.len=size; memcpy(str.data,dat,size);str.data[size]=0;}}

//////////////////////////////////////////////////////////////////////////////////////////

#ifndef INVALID_DATA_DFINE
#define INVALID_DATA_DFINE
#define INVALID_WORD				((WORD)(-1))						//无效WORD 数据
#define INVALID_DWORD				((DWORD)(-1))						//无效DWORD 数据
#define INVALID_UNDWORD				((UNDWORD)(-1))						//无效UNDWORD 数据
#endif

#ifndef AFXVALIDADDRESS
#define AFXVALIDADDRESS
//公共宏定义
#define AfxIsValidAddress(a,s) (a!=NULL)
#endif

//转换IP地址
DWORD ConvertUrlToIpAddr(LPCSTR lpszUrl);
//64bit网络字节序操作函数
UNDWORD htonl64(UNDWORD ulSrc);
UNDWORD ntohl64(UNDWORD ulSrc);
double ntohdb64(double ulSrc);
int EncCheckCode(LPCSTR pszUserName,int nLength,CHAR * pCheckCode,int nMonth,int nDay,int nHour,int nMinute,int nSecond);
int EncCheckCode10(LPCSTR pszUserName,int nLength,CHAR * pCheckCode,int nMonth,int nDay,int nHour,int nMinute,int nSecond);
int EncCheckCode(LPCSTR pszUserName,int nLength,CHAR * pCheckCode);
int EncCheckCode(DWORD dwID,LPCSTR pszUserName,int nLength,CHAR * pCheckCode);
void EncCheckCode(UNDWORD undwMobile,CHAR * pCheckCode);
void DecCheckCode(const CHAR * pCheckCode,UNDWORD &undwMobile);
bool DecCheckCode(const CHAR * pCheckCode,LPSTR pszUserName);
bool DecCheckCode(DWORD dwID,const CHAR * pCheckCode,LPSTR pszUserName);
#ifdef _WIN32
int watoi(const WCHAR *lpszNum);
//UTF8 转为 GBK
int UTF8ToGBK(const char * utf8Str,char * pGBKBuffer,int nGBKBuffSize);
//GBK 转为 UTF8
int GBKToUTF8(const char * gbkStr,char *pUTF8Buffer,int nUTF8Size);
//Unicode转UTF8
int UnicodeToUTF8(const wchar_t *pwStr,char *pUTF8Buffer,int nUTF8Size);
//Utf8转为Unicode
int UTF8ToUnicode(const char *pUTF8Buffer,wchar_t *pwStr,int nBufferSize);

void OutTextByColor(LPCTSTR lpszText,WORD wColor);
#else
//获取当前时间的毫秒计数
DWORD GetTickCount();
void GetLocalTime(PSYSTEMTIME pst);
void int64tochar(ULONGLONG lnSrc,char *pszDest,int nSize);
UDWORD CharToUInt64(char *pszSrc);
#endif
//检查是否是数字
bool CheckDigit(char *data);
//把字符串转换为全小写
void ToLowerCase(TCHAR * s);
//把字符串转换为全大写
void ToUpperCase(char * s);
//创建目录
void CreateSubDir(LPCTSTR lpszPath);

////////// 类型定义 /////////////////////////////////////////////////////////////////////////////
#define CWD_CHK_SIZE			12					//课件验证码大小
#define NAME_LEN				32
#define NAME_LEN32				32
#define WNAME_LEN				64
#define MAX_NAME				128					//最大名称长度
#define CHECKCODE_MAXLEN		128
#define BUFF_SIZE				64
#define URL_LEN					128
#define MSG_BUFF_SIZE			512					//消息缓冲大小
#define UINTLEN					24					//整型数长
///////////////////////////////////////////////////////////////////////
#define SHARE_MEM_NAME		TEXT("deir_ite_app_share")
#define MAX_BUFF_SIZE		1356
#define MSG_MAX_SIZE		1024					//最大文本消息长

/////////////////////////////////////////////////////////////////////////
#define MAX_ST_SIZE 2
#define LINE_MAX_POINT			250					//线的最大点数
#define TXT_MAX_LEN				512					//输入文本最大长度
#define MAX_LBS					5					//最大LB服务器数
#define MAX_LBS_URL				3					//最大LB服务器url数
////////////////////////////////////////////////////////////////////////
//连接类型
typedef enum emConnectSvrType
{
	CST_LBSVR=1,				//负载均衡服务器
	CST_SWTSVR,					//登录服务器
	CST_MSGSVR,					//消息服务器
	CST_VERSVR,					//版本检测服务器
}CONNECTSVRTYPE;

///////////////////////////////////////////////////////////
//服务器类型
typedef enum enIteServerType
{
	ST_NONESVR=0,					//不是服务器
	ST_LBSVR=(1<<0),				//负载服务器
	ST_SWTSVR=(1<<1),				//交换服务器
	ST_MAX_SVR=(ST_SWTSVR+1),		//最大服务器
	ALL_SERVER=(ST_LBSVR|ST_SWTSVR),			//所有服务器
}ITESERVERTYPE;

//客户端类型
typedef enum emIteClientType
{
	CT_UNKNOWN = 0,					//未知客户端(演示客户端)
	CT_SERVICE = 1,					//客服客户端
	CT_USER = 2,					//用户PC客户端
	CT_USER_IOS = 4,				//用户ios客户端
	CT_USER_ANDR = 8,			//用户ANDROID客户端
	CT_USER_DEVID=16,			//用户设备ID
	CT_STUCENT_ALL = (CT_USER | CT_USER_IOS | CT_USER_ANDR |CT_USER_DEVID),
	
}ITECLIENTTYPE;

//教室状态
typedef enum emIteRoomStatus
{
	RS_INVALID,				//无效
	RS_VALID,				//有效
}ITEROOMSTATUC;

//音频操作类型
typedef enum emAudioOptType
{
	AOT_CUSTOM,				//自定义操作
	AOT_ALLOW,				//允许通话
	AOT_FORBID,				//禁止通话
}AUDIOOPTTYPE;

//黑板事件ID
typedef enum emBoardEventID
{
	BEID_LINE_STYLE=1,					//线样式事件
	BEID_ELEMENT,						//黑板元素
	BEID_ADD_IMAGE,						//添加图片
	BEID_ZOOM_IMAGE,					//缩放图片
	BEID_DEL_IMAGE,						//删除图片
	BEID_INPUT_TEXT,					//输入文件
	BEID_CLS_BOARD,						//擦除黑板
	BEID_UNDO_ELEMENT,					//撤销元素
	BEID_REDO_ELEMENT,					//恢复元素
	BEID_SWITCH_PAGE,					//切换黑板(指令后为切换到的黑板页索引)
	BEID_BOARD_SIZE,					//黑板大小
	BEID_AUDIO,							//音频数据
	BEID_VIDEO,							//视频数据
	BEID_PLAY_CWD,						//播放课件
	BEID_END_CWD,						//播放课件完成
	BEID_PLAY_TIME,						//播放时间
	
	EBID_RES_LIST=1000,					//资源列表
}BOARDEVENTID;

//操作事件ID
typedef enum emOperateEventID
{
	OEID_QUESTION=2000,					//在线提问
	OEID_AUDIO_CTRL,					//音频通话控制,(0=禁止通话,1=允许通话)
	OEID_WRITE_CTRL,					//书写控制,(0=禁止书写,1=允许书写)
	OEID_CLIENT_PLAY,					//客户端播放(指令后为播放url)
	OEID_CLASS_BEGIN,					//上课
	OEID_CLASS_END,						//下课
	OEID_CLASS_PAUSE,					//课间休息
	OEID_CLASS_RESUME,					//恢复上课
	OEID_CLASS_PREPARE,					//课前准备
	OEID_USER_UPHAND,					//用户举手操作
	OEID_SELECT_USER,					//选择用户回答问题
	OEID_VIDEO_CTRL,					//视频控制
}OPEREVENTID;

//消息类型
typedef enum emMsgType
{
	TMT_TXT,				//文本消息
	TMT_IMG,				//图片消息
	TMT_AUDIO,				//声音消息
	TMT_VIDEO,				//视频消息
	TMT_CWD,				//课件文件消息
	TMT_FILE,				//文件消息
	TMT_WECHAT=10,			//微信通知(发送到微信接口)
	TMT_MSGRESP=59,			//消息Response
	TMT_JSON=60,			//JSON对象
	TMT_TINROOM,			//客服进入教室
	TMT_SINROOM,			//用户进入教室
	
}MSGTYPE;

//文件类类型
typedef enum emTransFileType
{
	TFT_UNKNOWN,			//未知
	TFT_IMAGE,				//图片
	TFT_AUDIO,				//音频
	TFT_VIDEO,				//视频
	TFT_OITCW,				//OIT课件
}TRANSFILETYPE;

//用户类型
typedef enum emUserType
{
	UT_USER,				//用户
	UT_SERVICE,				//客服
	UT_PARENT,				//家长
}USERTYPE;

//操作类型
typedef enum emIteOptionType
{
	OT_NOOPTION,				//没有操作
	OT_CONNECT_FAILD,			//连接失败
	OT_LOGIN_FAILD,				//登录失败
	OT_LOGIN_TEACHSVR_FAILD,	//登录客服服务器服务失败
	OT_LOGIN_CTRLSVR_FAILD,		//登录控制服务器失败
	OT_SHOW_ROOMLIST,			//显示教室列表窗口
	OT_LOGIN_CTRL_SUCCESS,		//登录控制服务器成功
	OT_LOGIN_TEACHSVR_SUCCESS,	//登录客服服务器服务成功
	OI_EXCEPTION,				//异常操作
}ITEOPTIONTYPE;

//视频数据类型
typedef enum emH264VIDEOTYPE
{
	HVT_SPSPPS,					//H264SpsPps数据
	HVT_FRAME_DATA,				//帧数据
}H264VTYPE;

//负载服务器间数据类型
enum emLBSDATATYPE
{
	LBSDT_SC_LOGIN=1,			//客服注册数据
	LBSDT_BS_REG,				//业务服务器数据
	LBSDT_BS_LBD,				//业务服务器负载数据
};

//数据队列头
struct tagDataHead
{
	WORD							wDataSize;							//数据大小
	WORD							wIdentifier;						//类型标识
};

//数据结构
struct tagDataBuffer
{
	WORD							wBufferSize;
	BYTE							*pDataBuffer;
};

//负荷信息
struct tagBurthenInfo
{
	DWORD							dwDataSize;							//数据大小
	DWORD							dwBufferSize;						//缓冲区长度
	DWORD							dwDataPacketCount;					//数据包数目
};

/////////////////////////////////////////////////////////////////////////
//共享数据结构
typedef struct tagShareMemData
{
	HWND					hMainWnd;					//主进程主窗口句柄
	HWND					hParentWnd;					//ie 父窗口句柄
	HWND					hWebWnd;					//ie进程Web主窗口句柄
	WORD					wDataSize;					//数据大小
	BYTE					cbBuffer[MAX_BUFF_SIZE];	//数据缓冲区
}SHAREMEMDATA;

//客服教室数据
typedef struct tagClassRoomData
{
	DWORD			dwRoomCount;			//教室数(1/3)
	DWORD			dwRoomID;				//教室ID
	DWORD			dwSERVICEID;			//客服ID
	DWORD			dwStartTime;			//开始时间
	DWORD			dwEndTime;				//结束时间
	int				nTotalPeriod;			//总课时
	int				nUsedPeriod;			//已用课时
	int				nCurrPeriod;			//当前课时
	int				nRoomStatus;			//教室状态
	int				nShelves;				//上架、下架标志
	TCHAR			szRoomName[BUFF_SIZE];	//教室名
	TCHAR			szRoomDiscrible[1024];	//教室描述
	TCHAR			szRealName[NAME_LEN];	//客服名
	
}CLASSROOMDATA;

typedef struct tagUserClassRoom :public tagClassRoomData
{
	WORD			wCtrlSvrPort;
	WORD			wTeachSvrPort;
	CHAR			szCtrlSvrUrl[NAME_LEN];
	CHAR			szTeachSvrUrl[NAME_LEN];
	TCHAR			szSERVICEName[NAME_LEN];
}USERCLASSROOM;

//教室数据
typedef struct tagClassRoomInfo
{
	DWORD			dwRoomID;				//教室ID
	DWORD			dwSERVICEID;			//客服ID
	DWORD			wUserCount;				//用户数
	WORD			wIndex;					//客服Socket索引
	WORD			wRoundID;				//客服Socket重用计数
	int				nPageCount;				//黑数
	SIZE			bbSize;					//黑板大小
}CLASSROOMINFO;

//元素类型
typedef enum ElementType
{
	ET_LINE,			//线段
	ET_TEXT,			//文本
	ET_IMAGE,			//图片
	ET_SHAPE,			//几何图形
}ELEMENTTYPE;


//用户数据
typedef struct tagUSERData
{
	DWORD			dwClassRoomID;			//教室ID
	DWORD			dwUSERID;			//用户ID
	CHAR			szUSERName[NAME_LEN];//用户名
	CHAR			szHeadUrl[URL_LEN];		//头像URL
}USERDATA;

//用户类型
typedef enum emOitsUserType
{
	OIT_UT_USER,					//用户
	OIT_UT_SERVICE,					//客服
	OIT_UT_PARENT,					//家长
}OITSUSERTYPE;

//开始上课类型
typedef enum emStartClassType
{
	SCT_NORMAL,						//正常上课
	SCT_DOWNIMG,					//下载图片上课
	STC_PLAYCWD,					//播放课件上课
	STC_LOCALCWD,					//播放本地
	STC_BROWSER,					//浏览器
}STARTCLASSTYPE;

//父服务器用户数据
typedef struct tagSvrUserData
{
	DWORD			dwUserID;				//用户ID
	WORD			wUserType;				//用户类型
	WORD			wServerPort;			//服务器端口
	DWORD			dwServerIP;				//所在服务器IP
}SVRUSERDATA;

//用户数据
typedef struct tagUserClientData
{
	DWORD			dwUserID;				//用户ID
	WORD			wUserType;				//用户类型
	WORD			wIndex;					//网络索引
	WORD			wRoundID;				//网络重用计数
	WORD			wServerPort;			//服务器端口
	DWORD			dwServerIP;				//所在服务器IP
	DWORD			dwRegTime;				//注册时间
	CHAR			szUserName[NAME_LEN];	//用户名
}USERCLIENTDATA;

typedef struct tagLbSvrData
{
	WORD			wConnCount;					//连接次数
	WORD			wPort;						//端口
	DWORD			dwServerIp;					//IP地址
}LBSVRDATA;

//媒体元数据结构
typedef struct tagAvMetaData
{
	MYBOOL			bHaveVideo;				//有视频
	MYBOOL			bHaveAudio;				//有音频
	int				nVCX;					//视频宽
	int				nVCY;					//视频宽
	int				nFrameRate;				//视频帧率
	int				nChannels;				//声道数
	DWORD			dwSampleRate;			//音频采样率
	DWORD			dwStartTime;			//开始时间
	TCHAR			szSrcCode[NAME_LEN];	//原号码
	TCHAR			szDstCode[NAME_LEN];	//目标号码
	SYSTEMTIME		sTime;					//开始时间
}AVMETADATA;

//Flv媒体文件描述
typedef struct tagFlvFileDesc
{
	DWORD			dwDuration;				//时长
	DWORD			dwFileSize;				//文件大小
	AVMETADATA		avMetaData;				//媒体数据
	
}FLVFILEDESC;

///////////////////////////////////////////////////////////////////

//交互指令函数指针
typedef void (*CmdFuncCall)(int nType,LPCTSTR lpszArgs);

//函数调用类型
typedef enum emFuncCallType
{
	FCT_NONE,						//没有调用
	FCT_HELP,						//帮助信息
	FCT_EXIT,						//退出程序
	FCT_PRINT_LB_SVR,				//打印负载均衡服务器列表
	FCT_PRINT_LB_MINSVR,			//打印负载均衡最小负载服务器列表
	FCT_PRINT_LB_CONN,				//打印负载均衡服务器连接信息
	PCT_PRINT_DATASIZE,				//打印传输数据大小,
	PCT_PRINT_ROOM_LIST,			//打印教室列表
	PCT_PRINT_ROOM_USER,			//打印教室用户列表
	PCT_PRINT_USER_INFO,			//打印用户信息
	PCT_STOP_REFRESH,				//停止刷新服务器状态
	PCT_AUTO_REFRESH,				//自动动刷新服务器状态
	PCT_CHECK_LB_CONN,				//检测到负载服务器连接状态
	PCT_TEST_MSG,					//发送消息测试
	PCT_JPUSH_LOG,					//打开/关闭Jpush日志
	PCT_CLOSE_WS,					//关闭WebSocket
	PCT_LOAD_DLL,					//加载DLL模块
	PCT_FREE_DLL,					//释放DLL模块
	PCT_TEST_CMD2DEV,				//测试发送控制指令到设备端
}FUNCCALLTYPE;

//调用指定结构
typedef struct tagFuncCallCmd
{
	FUNCCALLTYPE		fct;			//调用类型
	TCHAR				szCmdName[24];	//指令名称
	TCHAR				szCmdDesc[64];	//指令描述
}FUNCCALLCMD;

//AVS服务器状态
enum emAVSSRunStatus
{
	ARS_UNINSTALL,			//已卸载
	ARS_STOP,				//停止
	ARS_RUNING,				//运行中
	ARS_RELOAD,				//重载配置
};

//状态控制操作命令
enum emWerOptCtrlCmd
{
	WOCC_RELOAD=1,				//重载配置
	WOCC_START,					//开始服务器
	WOCC_STOP,					//停止Agent
	WOCC_UNINSTALL,				//卸载
};

//负载统计数据
typedef struct tagTransLoadData
{
	UNDWORD			udwAllDuration;			//总时长
	UNDWORD			udwAllDataSize;			//总数据大小
	DWORD			dwAssistCount;			//当前服务数
	DWORD			dwUserCount;			//用户数
	DWORD			dwServiceCount;			//客服数
	DWORD			dwConnCount;			//服务连接数
	DWORD			dwCloseCount;			//网络断开数
}TRANSLOADDATA;

//负载统计共享内存数据
typedef struct tagLDSShareMem
{
	MYBOOL			bRuning;				//运行中标志
	DWORD			dwProcessID;			//行程ID
	DWORD			dwLastRunTime;			//最后运行时间
	WORD			wListenPort;			//监听端口
	WORD			wServerType;			//服务器类型
	TRANSLOADDATA	mLoadData;				//负载
}LDSSHAREMEM;

//流媒体服务器进程数据
typedef struct tagAvsProcData
{
	DWORD			dwModuleID;					//模块ID
	WORD			wServerType;				//服务器类型
	WORD			wIndex;						//网络索引
	WORD			wRoundID;					//引用计数
	int				nStatus;					//客户端状态
	TCHAR			szHostName[WNAME_LEN];		//主机名
	TCHAR			szUserName[WNAME_LEN];		//用户名
	TCHAR			szClientIP[NAME_LEN32];		//ip地址
	TCHAR			szSysVer[NAME_LEN32];		//系统版本
	TCHAR			szAgentVer[NAME_LEN32];		//守护程序版本
	
	TRANSLOADDATA	lsd;						//负载统计数据
}AVSPROCDATA;

//∞¸¿‡–Õ
enum PACK_TYPE
{
	PT_VIDEO=100,			// ”∆µ
	PT_AUDIO,				//“Ù∆µ
	PT_TEXT,				//Œƒ±æ
	PT_SPSPPS,				//SPS ˝æ›
	PT_AACHEAD,				//aac“Ù∆µÕ∑
};

//////////////////////////////////////////////////
#ifdef _UNICODE
#define mysprintf _swprintf_c
#else
#define mysprintf _snprintf_c
#endif

////////////////////////////////////////////////////////////////////////////////
typedef list<string>					CDevList;					//设备列表
typedef list<string>::iterator			CDevListIt;					//设备列表
typedef vector<string>					CStringVec;					//字符串Vec
typedef vector<string>::iterator		CStringVecIt;				//字符串Vec
typedef vector<DWORD>					CDwordVec;
typedef vector<DWORD>::iterator			CDwordVecIt;
typedef map<UNDWORD,DWORD>							CUNDwordMap;					//长整型列表
typedef map<UNDWORD,DWORD>::iterator				CUNDwordMapIt;					//长整型列表
typedef map<DWORD,DWORD>							CDwordMap;						//整型map
typedef map<DWORD,DWORD>::iterator					CDwordMapIt;					//整型map
typedef map<DWORD,void *>							CDwordVoidMap;					//整型指针map
typedef map<DWORD,void *>::iterator					CDwordVoidMapIt;				//整型指针map
//////////////////////////////////////////////////////
//获取负载数据接口
struct IServerLoadData
{
	//获取服务器负载数据
	virtual bool __cdecl GetLoadDataOfServer(TRANSLOADDATA &loadData)=0;
	//获取所有客服列表
	virtual DWORD __cdecl GetAllServiceList(CDwordVec &serviceList)=0;
	//打印客服列表
	virtual void __cdecl PrintAllSerivceInfo()=0;
};

void MyThreadWait(DWORD dwTime);

////////////////////////////////////////////////////
//¥ÌŒÛ¬Î
#define SVR_MODULE_NOEXIST			1							//ƒ£øÈ≤ª¥Ê‘⁄
#define DB_SVR_EXPECT				-1							//∑˛ŒÒ∆˜“Ï≥£
#define NAME_PASS_ERR				-2							//”√ªß√˚ªÚ√‹¬Î¥ÌŒÛ
#define CTRL_LOGIN_ERR				-3							//∑˛ŒÒ∆˜—È÷§¥ÌŒÛ
#define ADD_BSVR_FAILD				-4							//ÃÌº”“µŒÒ∑˛ŒÒ∆˜ ß∞‹
#define CREATE_ROOM_FAILD			-5							//¥¥Ω®ΩÃ “ ß∞‹
#define ADD_TO_ROOM_FAILD			-6							//ÃÌº”µΩΩÃ “ ß∞‹
#define CREATE_FILE_FAILD			-7							//¥¥Ω®Œƒº˛ ß∞‹
#define WRITE_FILE_FAILD			-8							//–¥Œƒº˛ ß∞‹
#define CLIENT_TYPE_ERR				-9							//øÕªß∂À¿‡–Õ¥ÌŒÛ
#define ROOM_NO_CLASS				-10							//ΩÃ “√ª”–…œøŒ
#define SUB_SVR_IPPORT_ERR			-11							//◊”∑˛ŒÒ∆˜IPªÚ∂Àø⁄∑«∑®
#define ADD_USER_FAILD				-12							//ÃÌº””√ªß ß∞‹
#define ADD_GROUP_FAILD				-13							//ÃÌº”»∫ ß∞‹
#define USER_IS_OFFLINE				-16							//”√ªß≤ª‘⁄œﬂ
#define GROUP_NOT_EXIST				-17							//»∫≤ª¥Ê‘⁄
#define SERVICE_NOT_EXIST			-18							//øÕ∑˛≤ª¥Ê‘⁄
#define SERVICE_WORKING				-19							//øÕ∑˛’˝√¶
#define NO_ENOUGH_MEM				-20							//√ª”–◊„πªµƒƒ⁄¥Ê

//jni状态码
enum emJniStatusCode
{
	JNI_ERR_CODE=1000,						//错误码
	JNI_SC_1001,							//负载服务器地址无效
	JNI_SC_1002,							//创建Jni媒体服务实例失败
	JNI_SC_1003,							//没不创建Jni媒体服务实例
	JNI_SC_1004,							//启动定时器失败
	JNI_SC_1005,							//启动屏屏通通知线程失败
	JNI_SC_1006,							//启动音频组件失败
	JNI_SC_1007,							//启动视频编码线程失败
	JNI_SC_1008,							//网络连接线程失败
	JNI_SC_1009,							//网络连接失败
	JNI_SC_1010,							//视频服务器地址无效
	JNI_SC_1011,							//连接到负载服务器失败
	JNI_SC_1012,							//连接到视频服务器失败
	JNI_SC_1013,							//登录到视频服务器失败
	JNI_SC_1014,							//获取ServiceCode失败
	JNI_SC_1015,							//客服工号为空
	JNI_SC_1016,							//手机号为空
	JNI_SC_1017,							//客服工号太长
	JNI_SC_1018,							//手机号号太长
	
	JNI_SC_STATE=2000,						//jni处理状态
	JNI_SC_2001,							//开始连接到lbs
	JNI_SC_2002,							//开始连接到vss
	JNI_SC_2003,							//开始请求VSS IP:PORT
	JNI_SC_2004,							//请求VSS IP:PORT成功
	JNI_SC_2005,							//开始登录到VSS服务器
	JNI_SC_2006,							//登录到VSS服务器成功
	JNI_SC_2007,							//客服端停止辅助服务
};

#endif //__DEIRLYM_OIT_GLOBAL_DEFINE_HEAD_FILE__
