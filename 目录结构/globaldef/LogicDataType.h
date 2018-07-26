////////////////////////////////////////////////////////////////////////////////////
//		逻辑层数据类型头文件 LogicDataType.h
//		2016-08-04 Bowen.hu
////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_LOGIC_DATA_TYPE__HEAD_FILE__
#define __BOWEN_HU_LOGIC_DATA_TYPE__HEAD_FILE__

////////////////////////////////////////////////////////////////////////////////////
typedef enum LogicDataType
{
	LDT_COURSE_LIST=100,				//课程列表
	LDT_STUDENT_LIST,					//学生列表
	LDT_TEARCH_CW_LIST,					//教师课件列表
	LDT_CW_BG_IMG_LIST,					//课件背景图列表
	LDT_T_T_RES_LIST,					//教师教学资源列表
	LDT_ADD_CW_BG_IMG,					//添加课件背图
	LDT_ADD_TEACHING_CW,				//添加教学/录制课件
	LDT_ADD_T_RES_FILE,					//添加教学资源文件
	LDT_DEL_T_RES_FILE,					//删除教学资源文件
	LDT_EDIT_CW_DATA,					//修改课件数据
	LDT_DEL_OLD_PPT_IMG,				//删除ppt图片
	LDT_DEL_CW_DATA,					//删除课件数据
	LDT_ADD_PAGE_BG_IMG,				//添加上课页背景图
	LDT_EDIT_PAGE_SYNC,					//修改上课页同步数据
	LDT_EDIT_CW_LOCAL_PATH,				//修改课件本地路径
	LDT_GET_USER_CW_LIST,				//获取学生课件列表
}OITLDT;


#endif //__BOWEN_HU_LOGIC_DATA_TYPE__HEAD_FILE__
