//////////////////////////////////////////////////////////////////////////////////////////
//		PC客户端全局定义头文件 pcclientdef.h
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_PC_CLIENT_GLOBAL_DEFINE_HEAD_FILE__
#define __DEIRLYM_PC_CLIENT_GLOBAL_DEFINE_HEAD_FILE__

//////////////////////////////////////////////////////////////////////////
//对话框背景色
#define PROCESS_IMG_SIZE	50							//进度图片大小
#define DLG_BG_COLOR		RGB(128,128,128)
#define TOOLBAR_BG_COLOR	RGB(247,247,247)			//工具栏背景
#define	TOOLBAR_LINE_COLOR	RGB(161,161,161)			//工具栏线颜色
#define WND_BG_COLOR		RGB(200,200,200)			//窗口背景色
#define NOLOGIN_USER_COLOR	RGB(128,128,128)			//未登录线色
#define LOGIN_USER_COLOR	RGB(64,64,255)				//登录后线色
#define MSG_WND_BG_COLOR	RGB(0,0,0)					//文本消息窗口背景色
#define LOGIN_LINE_COLOR	RGB(206,214,218)			//登录窗口中线色
#define DEFAULT_FONT_SIZE	40							//默认字体大小

#define MSG_WND_TOP_OFFSET	3							//消息状态窗口top偏移量
#define MSG_ITEM_WIDTH		980							//留言项宽
#define MSG_ITEM_HEIGHT		85							//留言项高
#define MSG_ITEM_SAPCE		10							//留言间隔
#define LINE_POS_SPACE		2.0							//线的两点间最小距离

////////////////////////////////////////////////////////////////////
#define WND_BORDER			6							//边框宽
#define MAIN_GB_COLOR		RGB(114,161,195)			//主窗口背景色
///////Tab窗口配置 //////////////////////////////////////////////////////////
#define TAB_LEFT_CX			1				//TAB窗口左边距
#define TAB_SPACE			1				//TAB项间隔
#define CLASS_MAXTAB_NUM	8				//上课最大TAB数
#define MSG_TAB_COUNT		2				//消息TAB数
#define SEL_TXT_COLOR		RGB(8,74,108)		//选择项文本颜色
#define NOR_TXT_COLOR		RGB(255,255,255)	//正常项文本颜色
#define HAVE_TXT_COLOR		RGB(98,200,240)		//旋停项文本颜色
#define MID_BG_COLOR		RGB(238,238,238)	//中间背景色
#define RE_TEXT_COLOR		RGB(0,28,0)			//RichEdit 文本框字体颜色
#define RE_WARING_COLOR		RGB(128,128,24)		//警告颜色
#define RE_EXCEPT_COLOR		RGB(128,24,24)		//异常颜色
#define RE_DEBUG_COLOR		RGB(24,128,24)		//调试颜色
#define RE_HIGH_COLOR		RGB(128,128,255)	//高亮颜色
#define VIDEO_BG_COLOR		RGB(28,28,28)		//视频背景色
#define LIST_TXT_COLOR		RGB(254,254,254)
#define TEMP_DLG_BG_COLOR	RGB(42,102,148)		//模板对话框背景色
#define TIMER_TXT_COLOR		RGB(255,255,0)		//时间文本色
#define TITLE_TXT_COLOR		RGB(64,64,64)		//标题文本色
////////////////////////////////////////////////////////////////////
//自定义消息
#define WM_CLOSE_STATUSWND			(WM_USER + 97)			//关闭状态窗口
#define WM_START_IN_ROOM			(WM_USER + 98)			//开始进入教室
#define WM_START_LOGIN				(WM_USER + 99)			//开始登录消息
#define WM_LINE_PARAM_CHANGE		(WM_USER + 100)			//线条参数变便
#define WM_BOARD_OPT_EVENT			(WM_USER + 101)			//黑板操作事件
#define WM_USER_OPT_EVENT			(WM_USER + 102)			//用户操作事件

#define WM_SHOW_ASK_BOARD			(WM_USER + 103)			//显示答题板
#define WM_SHOW_WRITE_BOARD			(WM_USER + 104)			//显示手写板

#define WM_CLOSE_PROCESS_DLG		(WM_USER + 106)			//关闭进度对话框消息
#define WM_STUDENT_LIST_CLOSE		(WM_USER + 107)			//学生举手列表关闭消息
#define WM_USER_SETUP_OPT			(WM_USER + 108)			//用户设置消息
#define WM_BEGIN_CLASS				(WM_USER + 109)			//开始上课消息
#define WM_TXT_CHAT_MSG				(WM_USER + 111)			//文本聊天消息
#define WM_DOWNLOAD_FINISH			(WM_USER + 112)			//下载文件完成
#define WM_UPFILE_PROCESS			(WM_USER + 113)			//上传图片进度消息
#define WM_ROOM_EVENT_MSG			(WM_USER + 114)			//教室事件消息
#define WM_NEW_REPLY_MSG			(WM_USER + 116)			//新建或回复留言消息
#define WM_REPLY_DLG_CLOSE			(WM_USER + 117)			//回复留言对话框关闭消息
#define WM_MSG_PAGE_CHANGE			(WM_USER + 118)			//留言分页消息
#define WM_ROOM_PAGE_CHANGE			(WM_USER + 119)			//教室分页消息
#define WM_FLASH_BUTTON				(WM_USER + 120)			//按钮闪烁消息
#define WM_UPDATE_VER				(WM_USER + 120)			//版本升级消息
#define WM_SHOW_NOTIFY_MSG			(WM_USER + 121)			//显示通知消息窗口
#define	WM_LOAD_ROOM_MSG			(WM_USER + 122)			//按类别加载教室消息
#define WM_UPDATE_GIFT_NUM			(WM_USER + 123)			//更新礼品数消息
#define WM_TOOLBAR_OPT_MSG			(WM_USER + 124)			//工具栏操作消息
#define WM_MPLAY_PROGRESS			(WM_USER + 125)			//媒体播入进度消息
#define WM_PAGE_NUM_CHANGE			(WM_USER + 126)			//页数改变消息
#define WM_DOWN_CR_RES				(WM_USER + 127)			//下载教室资源消息
#define WM_TAB_SWITCH				(WM_USER + 128)			//tab切换消息
#define WM_CW_PROC_END_MSG			(WM_USER+129)			//课件处理结束通知消息
#define WM_ADD_CW_TO_LIST			(WM_USER+130)			//添加课件到列表
#define WM_LOAD_DATA_FINISHED		(WM_USER+131)			//加载数据完成消息
#define WM_USE_CW_TO_CLASS			(WM_USER+132)			//使用课件数据上课消息
#define WM_SWITCH_BB_OR_BROWSER		(WM_USER+133)			//切换黑板或浏览器
#define WM_ADD_PPTIMG_TO_BOARD		(WM_USER+134)			//添加ppt图片到黑板
#define WM_END_CLASS				(WM_USER+135)			//下课消息
#define WM_SNAP_BTN_MSG				(WM_USER+136)			//截屏按钮消息

///////////////////////////////////////////////////////////////////////
//定时器ID
#define IDT_CONN_SERVER					0x0001					//连接定登录服务器定时器ID
#define IDT_SVR_CLOSE					0x0002					//服务器断开(关闭)
#define IDT_STATUS_ANIMATION			0x0003					//状态窗口动画定时器
#define IDT_START_AUDIO					0x0004					//启动音频定时器
#define IDT_READ_AACDATA				0x0005					//读取AAC数据
#define IDT_SHOWSTUDENTLISTWND			0x0006					//显示学生列表窗口定时器ID
#define IDT_ANSWER_STATUS_ID			0x0007					//语音回答状态定时器
#define IDT_SHOE_ERRMSG_ID				0x0008					//显示出错消息定时器
#define IDT_AUDIO_GRAPHICS				0x0009					//声音图形
#define IDT_SHOW_CHAT_ROOM				0x000A					//显示聊天窗口定时器
#define IDT_UPLOAD_BBCW					0x000B					//上传课件黑板页定时器
#define IDT_ADD_BBC_IMG					0x000C					//添加课件黑板页图片定时器
#define IDT_UPLOAD_COURSE_FILE			0x000D					//上传课件文件定时器
#define IDT_UPLOAD_COURSE_FILE1			0x000E					//上传课件文件定时器
#define IDT_DOWNLOAD_IMAGE				0x000F					//下载图片定时器
#define IDT_LOOP_PLAY_CW				0x0010					//循环播放课件
#define IDT_MSG_CONN_TEST				0x0011					//消息服务器连接测试定时器
#define IDT_TEACH_CONN_TEST				0x0012					//教学服务器连接测试定时器
#define IDT_LOADING_ANIMATE				0x0013					//加载动画定时器
#define IDT_CLASS_DURATION				0x0014					//上课时长定时器
#define IDT_AUTO_INROOM					0x0015					//自动进入教室定时器
#define IDT_SNAPSHOT_SHOW				0x0016					//截屏窗口显示定时器
#define IDT_LINK_TIMEOUT				0x0017					//连接超时定时器
#define IDT_DOWN_CR_RES					0x0018					//下载教室资源
#define IDT_EDIT_PAGE_SYNC				0x0019					//更新页面同步数据记录
#define IDT_DOWNLOAD_CW_FILE			0x001A					//下载课件文件定时器

//Http API 指令
#define HAPI_COURSE_LIST				10001					//下载课程列表
#define HAPI_STUDENT_LIST				10002					//下载学生列表
#define HAPI_TEACH_CW_LIST				10003					//获取教师课件列表
#define HAPI_CW_BG_IMG_LIST				10004 					//获取课件背景图列表
#define HAPI_TEACH_RES_LIST				10005			 		//获取教师资源列表
#define HAPI_ADD_CW_BG_IMG				10006 					//添加课件背景图
#define HAPI_ADD_CW_FILE				10007 					//添加课件接口
#define HAPI_ADD_TRES_FILE				10008			 		//添加教师资源文件
#define HAPI_DEL_TRES_FILE				10009			 		//删除教师资源
#define HAPI_UPDATE_CW_DATA				10010					//更新课件记录数据
#define HAPI_DEL_CW_BG_IMG				10011					//删除课件背景图片
#define HAPI_DEL_CW_REC_DATA			10012					//删除课件记录
#define HAPI_ADD_PREPARE_BG_ULR			10013					//添加课前准备图片背景url
#define HAPI_EDIT_PAGE_SYNC				10014					//修改页数据同步记录指令
#define HAPI_EDIT_CW_LOCAL_PATH			10016					//修改课件本地路径
#define HAPI_GET_UESR_CW_LIST			10017					//获取学生课件列表

////////////////////////////////////////////////////////////////////////////////////////
//按钮ID定义
enum emToolButtonID
{
	BTN_BASE =	32767,
	BTN_MIN_ID,				//最小化按钮
	BTN_MAX_ID,				//最大化按钮
	BTN_MAX2_ID,			//恢复按钮
	BTN_CLOSE_ID,			//关闭按钮
	BTN_INROOM_ID,			//进入教室按钮
	BTN_ADDTOROOM_ID,		//加入教室按钮
	
	BTN_LOGIN_CLOSE_ID,		//登录窗口关闭按钮
	BTN_LOGIN_ID,			//登录按钮
	BTN_SAVEPWD_ID,			//保存密码按钮

	BTN_SLW_ID,				//设置线宽按钮
	BTN_SLC_ID,				//设置线颜色按钮
	BTN_SBGC_ID,			//设置背景色按钮
	BTN_UNDO_ID,			//撤销按钮
	BTN_CLEAR_ID,			//清除按钮

	BTN_CLASSES_INROOM,		//进入教室
	BTN_CLASSES_BEGIN,		//开始上课
	BTN_FINISH_CLASS,		//下课

	BTN_CLASS_PAUSE_ID,		//上课暂停按钮

	BTN_SHAREFILE_ID,		//共享文件按钮
	BTN_SHARESCREEN_ID,		//共享屏幕按钮

	BTN_QUESTION_END_ID,	//老师结束问题
	BTN_HANDSASK_ID,		//举手回答按钮
	BTN_ROOMCHAT_ID,		//教室聊天按钮
	BTN_ASK_FINISH_ID,		//回答完成按钮
	BTN_SUHL_CLOSE,			//举手界面关闭按钮
	BTN_S_REQUESTION_ID,	//学生提问按钮ID
	BTN_S_MUTE_ID,			//学生静音按钮ID
	

	BTN_ROOM_CLOSE_ID,		//房间关闭按钮
	BTN_ROOM_CONNECT_ID,	//房间连接按钮

	BTN_PROCESS_CANCEL_ID,	//进度对话框取消按钮

	BTN_INPUT_ENTER_ID,		//文本输入确定按钮
	BTN_HIDE_CHAT_WND_ID,	//隐藏聊天窗口按钮ID
	BTN_CHAT_NOTIFY_ID,		//聊天消息通知按钮ID	
	BTN_COURSE_LIST_ID,		//课件列表按钮ID

	IDC_INPUT_TEXT_EDIT,	//文本输入控件ID

	BTN_PAGE_FIST_ID,		//第一页按钮ID
	BTN_PAGE_PREV_ID,		//分页上一页按钮ID
	BTN_PAGE_NEXT_ID,		//分页下一页按钮ID
	BTN_PAGE_LAST_ID,		//最后一页按钮ID
	BTN_PAGE_ADD_ID,		//添加页按钮ID

	CBB_PAGE_GOTO_ID,		//分页转到页下拉框ID
	BTN_MSG_LIST_WND_ID,	//留言列表窗口按钮ID
	BTN_NEW_QUESTION_ID,	//新建留言按钮ID
	BTN_ROOM_PREV_PAGE_ID,	//教室上一页按钮ID
	BTN_ROOM_NEXT_PAGE_ID,	//教室下一页按钮ID
	BTN_BACK_ID,			//返回按钮ID
	BTN_STUDENT_LIST_ID,	//学生列表按钮ID
	BTN_ROOM_CATEGORY_ID,	//教室类别按钮ID

	BTN_TOPBAR_AUDIO_ID,	//音频配置按钮
	BTN_TOPBAR_VIDEO_ID,	//视频配置按钮
	BTN_TOPBAR_MYCOUSRE_ID,	//我的课件按钮ID
	BTN_TOPBAR_RECORD_ID,	//录制课件按钮ID
	BTN_TOPBAR_HOMEWORK_ID,	//作业按钮ID
	BTN_TOPBAR_REST_ID,		//休息按钮ID
	BTN_TOPBAR_SCREENSHARE_ID,//屏幕共享
	BTN_TOPBAR_CLASS_ID,	//上下课按钮ID

	BTN_ALL_NO_SPEAK_ID,	//全部禁言按钮ID
	BTN_ALL_INPUT_MSG_ID,	//全部可输入按钮ID
	BTN_ONLINE_REQUESTION_ID,//在线提问按钮
	BTN_CLASS_PREPARE_ID,	//课前准备

	BTN_PLAY_ID,			//播放按钮id
	BTN_STOP_ID,			//停止按钮id

	BTN_ID_MAX_VALUE,		//按钮最大ID号
};

//留言数据类型
typedef enum emLeaveMsgType
{
	MT_TEXT,			//文字
	MT_IMAGE,			//图片
	MT_AUDIO,			//音频
	MT_VIDEO,			//视频
}LEAVEMSGTYPE;

//下载文件类型
typedef enum emDownFileType
{
	DFT_NETIMG=1,			//网络接收图片
	DFT_CWFILE,				//课件文件
	DFT_PPTFILE,			//PPT文件
}DOWNFILETYPE;

//菜单id枚举
enum emMenuItemID
{
	IDM_PLAY_COURSEWARE=BTN_ID_MAX_VALUE+1,	//播放课件
	IDM_LOOP_PLAY_COURSEWARE,				//循环播放课件
	IDM_INSERT_PLAY_COURSEWARE,				//插播课件
	IDM_PAUSE_COURSEWARE,					//暂停播放课件
	IDM_RESUME_COURSEWARE,					//恢复播放课件
	IDM_STOP_COURSEWARE,					//停止播放
	IDM_UPLOAD_VIDEOCOURSEWARE,				//上传视频课件
	IDM_INSERT_IMAGE,						//插入图片
	IDM_INSERT_SNAP_IMG,					//插入屏幕截图图片
	IDM_DEL_IMAGE,							//删除图片
	IDM_ZOOM_IMAGE,							//缩放图片
	IDM_ZOOM_FINISH,						//缩放完成
	IDM_SCREEN_SNAPSHOT,					//屏幕截图
	IDM_OPEN_OTHER_FILE,					//打开其他文件

	IDM_REFRESH_CW,							//刷新课件
	IDM_ADD_MY_CW,							//添加我的课件
	IDM_EDIT_CW_PPT,						//修改课件PPT
	IDM_ADD_CW_PPT,							//添加课件PPT
	IDM_DELETE_CW,							//删除课件
	IDM_UPLOAD_CW_FILE,						//上传课件文件
	IDM_USE_CW_PPT,							//使用PPT上课
	IDM_PLAY_CW_FILE,						//播放课件

	IDM_ADD_CLASS_RES,						//添加上课资源
	IDM_DEL_CLASS_RES,						//删除上课资源
	IDM_CLIENT_PLAY,						//客户端播放
	IDM_BBOARD_PLAY,						//黑板上播放

	IDM_DELETE_COURSE,						//删除课件菜单项
	IDM_INPUT_FONT_SIZE,					//输入字体大小
	IDM_ALLOW_VOICE,						//允许语音通话
	IDM_FORBID_VOICE,						//禁止语音通话
	IDM_ADD_PPT_FILE,						//加载PPT文件
	IDM_OPEN_COURSE_DIR,					//打开课件目录
	IDM_ANSWERBB_SNAPSHOT,					//答题板截图
	IDM_ANSWERBB_REVIEWS,					//答题板点评
	IDM_ADD_MP3_FILE,						//加载mp3音频文件
	IDM_SET_TIME_PLAY,						//设置定时播放菜单
	IDM_INSERT_NEW_PAGE,					//插入新空白页
};

//留言数据结构
typedef struct tagMsgData
{
	UNDWORD		nMsgID;						//留言ID
	UNDWORD		nRoomID;					//教室ID
	UNDWORD		nTeacherID;					//教师ID
	UNDWORD		nStudentID;					//学生ID
	int			nQtype;						//提问类型
	int			nAtype;						//回答类型
	int			nMsgUserType;				//提问用户类型(0=学生,1=教师)
	char		szRoomName[BUFF_SIZE];		//教室名称
	char		szTeacherName[BUFF_SIZE];	//教师名称
	char		szStudentName[BUFF_SIZE];	//学生名称
	char		szQTime[NAME_LEN];			//提问时间
	char		szATime[NAME_LEN];			//答复时间
	int			nQLength;					//问题内容长
	char		*pQuestion;					//提问内容
	int			nALength;					//回答内容长
	char		*pAnswer;					//答复内容
}MSGDATA;

typedef struct tagEditMsgData
{
	UNDWORD		nMsgID;						//留言ID
	UNDWORD		nRoomID;					//教室ID
	UNDWORD		nUserID;					//用户ID
	int			nQLength;					//提问长
	TCHAR		*pQuestion;					//提问题内容

}EDITMSGDATA;

typedef struct tagChatMsgItem
{
	WORD		wMsgType;					//消息类型
	int			nItemHeight;				//项高
	DWORD		dwUserID;					//用户ID号
	TCHAR		szUserName[64];
	TCHAR		szTime[32];
	TCHAR		szMsgContent[MSG_MAX_SIZE];
}CHATMSGITEM;

#ifdef _WIN32
//聊天类型定义
typedef CMemManage<CHATMSGITEM>				CTxtMsgMem;				//文本聊天消息内存对象
#endif

//下载参数结构
typedef struct tagDownParam
{
	WPARAM				wParam;
	my_str_t			mJson;
	int					ldt;
	void				*pIHttpEventSink;
	CHAR				szDownUrl[URL_LEN];
}DOWNPARAM;

//课程数据(课程表及上课标题--目录)
typedef struct tagCourseData
{
	DWORD				dwCourseID;				//课程ID
	DWORD				dwRoomID;				//教室ID
	int					nPeriodNum;				//课时
	DWORD				dwStartTime;			//开始时间
	DWORD				dwEndTime;				//结束时间
	TCHAR				szPeriodName[URL_LEN];	//课时名称
}COURSEDATA;

//教师上课课件数据
typedef struct tagCoursewareData
{
	DWORD				dwCWID;					//课件ID
	DWORD				dwRoomID;				//教室ID
	int					nPeriodNum;				//课时
	BOOL				bHavePpt;				//是否有PPT
	int					nStatus;				//课件状态(0=新建、1=可上课(有ppt)、2=可播放(有课件))
	DWORD				dwCreateTime;			//创建时间
	TCHAR				szRoomName[NAME_LEN];	//教室名称
	TCHAR				szCwName[NAME_LEN];		//课件名
	CHAR				szCWFileUrl[URL_LEN];	//课件文件URL
	CHAR				szCWLocalPath[MAX_PATH];	//课件本地路径
}COURSEWAREDATA;

//课件背景图数据
typedef struct tagCWBGImage
{
	int					nImgCount;				//图片数
	DWORD				dwCWID;					//课件ID
	int					nPageIndex;				//页索引
	CHAR				szResUrl[URL_LEN];		//资源URL
	CHAR				szLocalPath[MAX_PATH];	//本地URL
}CWBGIMAGE;

//修改课件数据
typedef struct tagEditCWData
{
	DWORD				dwCWID;					//课件ID
	int					nDataType;				//数据类型(1=PPT状态,2=课件URL,3=两者都时)
	int					nHavePpt;				//是否有PPT
	CHAR				szCWUrl[URL_LEN];		//课件URL
	CHAR				szCWLocalPath[MAX_PATH];//课件本地路径
}EDITCWDATA;

//上课页面背景图
typedef struct tagClassPageBGImg
{
	DWORD				dwUserID;				//用户(教师ID)
	DWORD				dwRoomID;				//教室ID
	int					nPeriodNum;				//课时
	int					nPageNum;				//页数
	CHAR				szImgUrl[URL_LEN];		//背景图URL
	CHAR				szLocalPath[URL_LEN];	//背景图URL
}CLASSPAGEBGIMG;

//上课资源类型
typedef enum emClasResType
{
	CRT_AUDIO,								//音频
	CRT_VIDEO,								//视频
	CRT_CWD,								//oit课件
	CRT_IMAGE,								//图片
}CLASSRESTYPE;

//教师上课资源数据
typedef struct tagClassResFile
{
	DWORD			dwResID;				//资源ID
	DWORD			dwTeacherID;			//教师ID
	int				nResType;				//资源类型
	DWORD			dwCreateTime;			//创建时间
	TCHAR			szResName[NAME_LEN];	//资源名称
	CHAR			szResUrl[URL_LEN];		//资源url
	TCHAR			szLocalPath[MAX_PATH];	//本地路径
}CLASSRESFILE;

typedef struct tagPageSync
{
	DWORD				dwUserID;					//用户ID(教师ID)
	DWORD				dwRoomID;					//教室ID
	int					nPeriodNum;					//当前课时
	int					nFaildCount;				//失败次数
	int					nPageNum;					//页索引数
	CHAR				szbbcUrl[URL_LEN];			//页面文件url
	CHAR				szLocalFile[MAX_PATH];		//本地文件
}PAGESYNC;
//////////////////////////////////////////////////////////////////////////////////////
typedef vector<POINT>					CPointVector;			//坐标列表类
typedef vector<POINT>::iterator			CPointVectorIt;			//坐标列表类迭代器
typedef vector<DRAWELEMENT>				CElementVector;			//绘制元素列表类
typedef vector<DRAWELEMENT>::iterator	CElementVectorIt;		//绘制元素列表类迭代器

/////////////////////////////////////////////////////////////////////////////////////
typedef vector<LINEINFO>				CLineInfoVec;			//线条信息Vector
typedef vector<LINEINFO>::iterator		CLineInfoVecIt;			//线条信息Vector迭代器

typedef list<RECT>					CRectList;
typedef list<RECT>::iterator		CRectListIt;
///////////////////////////////////////////////////////////////////////////////////////
//网络消息通知接口
interface INetMsgNotify
{
	//消息通知
	virtual bool __cdecl NetMsgNotify(LPCTSTR lpszMsg,ITEOPTIONTYPE nOptType=OT_NOOPTION)=0;
	//消息通知
	virtual bool __cdecl NetMsgAutoParam(ITEOPTIONTYPE nOptType,LPCTSTR lpszFormate,...)=0;
};

//聊天接口
interface ITxtMsgChat
{
	//获取文本消息内存
	virtual void * __cdecl GetTxtMsgItem()=0;
	//添加聊天消息
	virtual void __cdecl AddTxtMsgItem(void * pMsgItem)=0;
};


#endif //__DEIRLYM_PC_CLIENT_GLOBAL_DEFINE_HEAD_FILE__
