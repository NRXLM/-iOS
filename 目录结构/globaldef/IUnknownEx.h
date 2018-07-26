// //////////////////////////////////////////////////////////////////////////////////////////
//  Module Interface  parent interface head file IUnknownEx.h
//  Other all interface parent interface
//  Author : deirlym
//  time:   2008-08-13
//  Version: 2.0
// ////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_IUNKNOWN_EX_HEAD_FILE__
#define __DEIRLYM_IUNKNOWN_EX_HEAD_FILE__

#ifndef WIN32
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <string>
    #include <assert.h>
    #include <dlfcn.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/wait.h>
    #include <arpa/inet.h>
    #include <sys/time.h>
    #include <semaphore.h>
    #include <sys/ipc.h>
    #include <signal.h>
    #include <errno.h>
    #include <stdarg.h>
    #include <time.h>
	#include <netdb.h>
	#include <sys/types.h>

//#define _DEBUG
// ///////////////////////////////////////////////////////////////////////////////////////////
    typedef unsigned short WORD;
    typedef unsigned int DWORD;
    typedef unsigned int UINT;
    typedef long	DDWORD;
    typedef unsigned long long UDWORD;
    typedef unsigned long long ULONGLONG;
    typedef long long INT64;
    typedef long long LONGLONG;
    typedef int   INT;
	typedef UDWORD UNDWORD;
	typedef long	LONG;

    #define MAX_PATH                    260

    typedef unsigned char BYTE;
    typedef  const char * LPCTSTR;
	typedef LPCTSTR			LPCSTR;
    typedef char *            LPTSTR;
    typedef char *            LPSTR;
    typedef char              TCHAR;
    typedef long              LONG;
	typedef int               MYBOOL;
    typedef int               INT;
    typedef int               MYBOOL;
    typedef unsigned char     UCHAR;
    typedef WORD            WTCHAR; 
	typedef TCHAR			CHAR;	
	typedef short			__int16;
	typedef short			SHORT;
	typedef int				__int32;
	
    typedef int INT_PTR, *PINT_PTR;
    typedef unsigned int UINT_PTR, *PUINT_PTR;
    typedef long LONG_PTR, *PLONG_PTR;
    typedef unsigned long ULONG_PTR,*PULONG_PTR;

    typedef UINT_PTR            WPARAM;
    typedef LONG_PTR            LPARAM;
    typedef LONG_PTR            LRESULT;

	typedef void *		HWND;
    typedef void *      HANDLE;                     //handle
    typedef void *      HINSTANCE;                  //instance handle
    typedef unsigned int         SOCKET;                     
    #define INVALID_SOCKET      (SOCKET)(~0)
    #define TRUE            1
    #define FALSE           0

    #define STRARRAY(str) (sizeof(str)/sizeof(str[0]));
    #define ZeroMemory(objPtr,nSize) (memset(objPtr,0,nSize));

    #ifndef CONTAINING_RECORD
    #define CONTAINING_RECORD(address,type,field) \
        ((type*)((char *)(address)-(ULONG_PTR)(&((type*)0)->field)))
    #endif

    #define TEXT(quote) quote

    typedef struct sockaddr SOCKADDR;
    typedef struct sockaddr_in SOCKADDR_IN;
    #define closesocket(s) close(s)
    #define SOCKET_ERROR              (-1)

    #define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD)(a) & 0xff)) | ((WORD)((BYTE)((DWORD)(b) & 0xff))) << 8))
    #define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))
    #define LOWORD(l)           ((WORD)((DWORD)(l) & 0xffff))
    #define HIWORD(l)           ((WORD)((DWORD)(l) >> 16))
    #define LOBYTE(w)           ((BYTE)((DWORD)(w) & 0xff))
    #define HIBYTE(w)           ((BYTE)((DWORD)(w) >> 8))

    // ///////////////////////////////////////////////////////////////////////////////////////////
    //time struct define
    typedef struct tagSystemTime
    {
        WORD                              wYear;
        WORD                              wMonth;
        WORD                              wDay;
        WORD                              wHour;
        WORD                              wMinute;
        WORD                              wSecond;           
    }SYSTEMTIME,*PSYSTEMTIME;

	#define BUILD_VERSION(a,b,c,d)	((a)<<24)|((b)<<16)|((c)<<8)|(d)
	#define GET_VERSION(dest,ver)	dest[0]=(ver>>24);dest[1]=(ver>>16)&0x000000FF;dest[2]=(ver>>8)&0x000000FF;dest[3]=ver&0x000000FF;

	//模块信息结构
	typedef struct tagModuleInfo
	{
		DWORD							dwVersion;						//版本号
		DWORD							dwMouleCode;					//模块编号
		DWORD							dwProperty;						//模块属性值
		TCHAR							szInterfaceName[32];			//实现接口名
		TCHAR							szName[64];						//模块名称
		SYSTEMTIME						stCreateTime;					//创建时间
		SYSTEMTIME						stUpdateTime;					//更新时间
        TCHAR							szDescribe[128];				//描绘		
	}MODULEINFO,*PMODULEINFO;

	//验证模块版本
	#define CHECK_MODULE_VER(pIUnknownExEx,dwVer)														\
			MODULEINFO *_pcheckModuleInfo=(MODULEINFO *)pIUnknownExEx->GetModuleInfo();					\
			if(_pcheckModuleInfo->dwVersion<dwVer)														\
			{																							\
				BYTE cbVer[4];																			\
				GET_VERSION(cbVer,dwVersion);															\
				printf("模块[%s]版本太低，程序不能继续运行，请使用[%d.%d.%d.%d]以上的版本！\n",		\
				_pcheckModuleInfo->szName,cbVer[0],cbVer[1],cbVer[2],cbVer[3]);								\
				exit(1);																				\
			}	

	//写版到日志
	#define WRITE_VERSION(pLogInterface,szInfo,szVer)				\
			TCHAR		_writeVerMsg[256]="";					\
			snprintf(_writeVerMsg,sizeof(_writeVerMsg),"*******************  %s %s ********************",szInfo,szVer);	\
			pLogInterface->WriteString(_writeVerMsg);

    // build time
    #define __max(a,b) ((a>b)?a:b)
    #define __min(a,b) ((a<b)?a:b)

    #ifdef _DEBUG
        #define ASSERT(a) assert(a)
        #define myprintf(str) printf(str)
    #else
        #define ASSERT(a) ((void)0)
        #define myprintf(str) ((void)0)
    #endif 

    #define lstrcpy(dest,src) strcpy(dest,src)
    #define lstrlen(str) (int)strlen(str)
    #define lstrcat(dest,str) strcat(dest,str)
    #define lstrcmp(str1,str2) strcmp(str1,str2)

    //事件结构体
    typedef struct tagEventHandle
    {
        sem_t							semt;
        struct tagEventHandle          *pPrev;
        struct tagEventHandle          *pNext;
    }*EVENTHANDLE;

   #define INFINITE            0xFFFFFFFF  // Infinite timeout

#ifndef __IPHONE_4_0
	#define __cdecl  
	#define interface struct
#endif

	typedef struct hostent *LPHOSTENT;

#include "DataType.h"
#else
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
typedef int               MYBOOL;

#endif //WIN32

#define CREATE_FUNC_NAME            "CreateInterfaceObject"

#define BUILDTIME(year,month,day,hour,minute,second) (((year-2000)<<28)|(month<<24)|(day<<18)|(hour<<12)|(minute<<6)|(second))
#define YEAR(value) ((value>>28)+2000)  //获取年
#define MONTH(value) ((value & 0x0F000000)>>24) //月
#define DAY(value) ((value & 0x00FC0000)>>18)	//日
#define HOUR(value) ((value & 0x0003F000)>>12)	//时
#define MINUTE(value) ((value & 0x00000FC0)>>6)	//分
#define SECOND(value) (value & 0x0000003F)  //秒
#define BUILDSEQUENCE(day,hour,minute,second,index) (((day%16)<<28)|(hour<<23)|(minute<<17)|(second<<11)|(index))
#define INVALID_VALUE ((WORD)-1)
// ///////////////////////////////////////////////////////////////////////////////////////////
//product version
#define PRODUCT_VER                 1

//Version define
#define INTERFACE_VERSION(cbMainVer,cbSubVer)                   \
        (DWORD)(                                            \
        (((BYTE)(PRODUCT_VER))<<24)+                           \
        ((BYTE)(cbMainVer)<<16)+                                \
        ((BYTE)(cbSubVer)<<8))
       
//Get product version
inline BYTE GetProductVer(DWORD dwVersion)
{
    BYTE * cbVersion=(BYTE*)&dwVersion;
    return cbVersion[0];
}  
//Get Main version
inline BYTE GetMainVer(DWORD dwVersion)
{
    BYTE * cbVersion=(BYTE*)&dwVersion;
    return cbVersion[1];
}
//Get sub version
inline BYTE GetSubVer(DWORD dwVersion)
{
    BYTE * cbVersion=(BYTE*)&dwVersion;
    return cbVersion[2];
}
//Get sub build version
inline BYTE GetBuildVer(DWORD dwVersion)
{
    BYTE * cbVersion=(BYTE *)&dwVersion;
    return cbVersion[3];
}
//Version cmp
inline bool InterfaceVersionCheck(DWORD dwQueryVer,DWORD dwInterfaceVer)
{
    if(GetProductVer(dwQueryVer)!=GetProductVer(dwInterfaceVer)) return false;
    if(GetMainVer(dwQueryVer)!=GetMainVer(dwInterfaceVer)) return false;
    if(GetSubVer(dwQueryVer)!=GetSubVer(dwInterfaceVer)) return false;
    return true;
}


// ///////////////////////////////////////////////////////////////////////////////////////////
#ifndef COUNTARRAY
#define COUNTARRAY
//Count array size
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))   
//Computer string length
#define CountString(string) ((UINT)((lstrlen(string)+1)*sizeof(TCHAR)))
#endif

//Safe delete point
#define SafeDelete(pData) {try{delete pData;}catch(...){} pData=NULL;}
#define SafeDeleteArray(pData){try{delete [] pData;}catch(...){} pData=NULL;}    


// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Error info length
#define LEN_ERROR           256

#ifndef WIN32
    // ///////////////////////////////////////////////////////////////////////////////////////////
    // Interface define
    #define STD_METHOD (TYPE,METHOD) virtual TYPE METHOD
    // Interface module create function
    typedef void * (ModuleCreateFunc)(DWORD dwInterfaceVer);

    //base interface
    #define VER_IUnknownEx  INTERFACE_VERSION(1,1)
    class IUnknownEx
    {
    public:
        //Is validate
        virtual bool IsValid()=0;
        //Release object
        virtual bool Release()=0;
        //Interface query
        virtual void * QueryInterface(DWORD dwQueryVer)=0;
    };
	
	//扩展基础接口
	#define VER_IUnknownExEx  INTERFACE_VERSION(1,2)
	class IUnknownExEx :public IUnknownEx
    {
    public:
        //获取模块信息
		virtual void * GetModuleInfo()=0;
    };
            
    //Interface Query
    #define QUERYINTERFACE(Interface,dwQueryVer)                     \
            if(InterfaceVersionCheck(dwQueryVer,VER_##Interface))    \
                return static_cast<Interface *>(this);

    //Query base interface
    #define QUERYINTERFACE_IUNKNOWNEX(BaseInterface,dwQueryVer)     \
            if(InterfaceVersionCheck(dwQueryVer,VER_IUnknownEx))    \
                return static_cast<IUnknownEx *>(static_cast<BaseInterface*>(this))

	//Query base interface
    #define QUERYINTERFACE_IUNKNOWNEXEX(BaseInterface,dwQueryVer)     \
            if(InterfaceVersionCheck(dwQueryVer,VER_IUnknownExEx))    \
                return static_cast<IUnknownExEx *>(static_cast<BaseInterface*>(this))
               
    //Get owner interface
    #define GET_MYSELF_INTERFACE(Interface)                                             \
        (Interface *)QueryInterface(VER_##Interface)
         
    //Get object interface
    #define GET_OBJECT_INTERFACE(Object,Interface)                                         \
        (Interface *)Object.QueryInterface(VER_##Interface)
          
    //Get object point interface
    #define GET_OBJECTPTR_INTERFACE(pObject,Interface)                                  \
        ((pObject==NULL)?NULL:((Interface*)pObject->QueryInterface(VER_##Interface)))

///////////////////////////////////////////////////////////////////////////////////////////
    // Module helper template class
    template<typename IModuleInterface>
    class CTemplateHelper
    {
    //variable define
    protected:    
        LPCTSTR                     m_pszFuncName; 
        LPCTSTR                     m_pszModuleDll;
        HINSTANCE                   m_hDllInstance; 
        const DWORD                 m_dwVersion;                  
        IModuleInterface            *m_pIModuleInterface; 
        TCHAR                       m_szErrorMsg[LEN_ERROR]; 
        
    //function define
    public:
    //construct function
        CTemplateHelper(DWORD dwVer,LPCTSTR pszModuleDll,LPCTSTR pszFuncName)
    :m_dwVersion(dwVer)
        {
            m_szErrorMsg[0]=0;        
            m_hDllInstance=NULL;
            m_pIModuleInterface=NULL;
            m_pszFuncName=pszFuncName;        
            m_pszModuleDll=pszModuleDll;        
        }        
        virtual ~CTemplateHelper()
        {
            CloseInstance();
        }
    //function
    public:
        //set member value
        bool SetModuleName(LPCTSTR pszModuleDll)
        {        
            m_pszModuleDll=pszModuleDll;      
            return true;
        }        
    //close function
        bool CloseInstance()
        {
            if(m_pIModuleInterface!=NULL)
            {
                m_pIModuleInterface->Release();
                m_pIModuleInterface=NULL;
            }
            if(m_hDllInstance!=NULL)
            {
                dlclose(m_hDllInstance);
                m_hDllInstance=NULL;
            }
            return true;
        }
    //Create function
        bool CreateInstance()
        {
            char * pszError;
            CloseInstance();
            try
            {
            //Load so
            //printf("%s\n",m_pszModuleDll);
                m_hDllInstance=dlopen(m_pszModuleDll,RTLD_LAZY);
                pszError=dlerror();            
                if(m_hDllInstance==NULL || pszError!=NULL)                                    
                {                
                    sprintf(m_szErrorMsg,"%s Module Load Faild,%s!",m_pszModuleDll,pszError);                
                    return false;                
                }
                            
            //Get enter function            
                ModuleCreateFunc * pCreateFunc=(ModuleCreateFunc*)dlsym(m_hDllInstance,m_pszFuncName);
                pszError=dlerror();
                if(pCreateFunc==NULL || pszError!=NULL)            
                {                
                    sprintf(m_szErrorMsg,"%s Module Create function %s is not found,%s!",
                            m_pszModuleDll,m_pszFuncName,pszError);
                    return false;
                }
            //Create module
                m_pIModuleInterface=(IModuleInterface*)pCreateFunc(m_dwVersion);                     
                if(m_pIModuleInterface==NULL)            
                {
                    sprintf(m_szErrorMsg,"%sModule create function %s create faild!",m_pszModuleDll,m_pszFuncName);                
                    return false;
                }
            }
            catch(...)
            {
                sprintf(m_szErrorMsg,"%s Module Create exception,%s!",
                        m_pszModuleDll,pszError);
                return false;                        
            }
            return true;
        }
    //Is valid
        bool IsValid()
        {
            if(m_pIModuleInterface==NULL) return false;
            return m_pIModuleInterface->IsValid();
        }
    //Get Error message
        LPCTSTR GetErrorMessage(){return m_szErrorMsg;}
		LPCTSTR GetDllName(){return m_pszModuleDll;}

    //Helper function
    public:
    //Reload point
        IModuleInterface * operator->(){return m_pIModuleInterface;}
    //Get Interface
        IModuleInterface * GetInterface(){return m_pIModuleInterface;}
    };

	//创建IUnknownEx 接口辅助类
	class CIUnknownExHelper: public CTemplateHelper<IUnknownEx>
	{
	public:
		//构造函数
		CIUnknownExHelper(LPCTSTR lpszModuleName):CTemplateHelper<IUnknownEx>(VER_IUnknownEx,
			lpszModuleName,CREATE_FUNC_NAME){}
	};
#else    	
	//typedef unsigned long long UDWORD;
	typedef unsigned long int UDWORD;
	typedef unsigned __int64 UNDWORD;
    //Interface define
    #define STD_METHOD(TYPE,METHOD) virtual TYPE __cdecl METHOD
    //Interface module create function
    typedef void * (__cdecl ModuleCreateFunc)(const GUID & Guid,DWORD dwInterfaceVer);

    //基接口
    #define VER_IUnknownEx INTERFACE_VERSION(1,1)
    static const GUID IID_IUnknownEx = { 0x1e7ee16a, 0x4120, 0x4c27, { 0xa8, 0x7b, 0x3e, 0x39, 0x32, 0xee, 0xf2, 0x2e } };
    struct IUnknownEx
    {
	    //是否有效
	    virtual bool __cdecl IsValid()=0;
	    //释放对象
	    virtual bool __cdecl Release()=0;
	    //接口查询
	    virtual void * __cdecl QueryInterface(const IID &Guid,DWORD dwQueryVer)=0;
    };

    //接口查询宏
    #define QUERYINTERFACE(Interface,Guid,dwQueryVer)										\
	    if((Guid==IID_##Interface) &&(InterfaceVersionCheck(dwQueryVer,VER_##Interface)))	\
		    return static_cast<Interface *>(this);	

    //查询基接口
    #define QUERYINTERFACE_IUNKNOWNEX(BaseInterface,Guid,dwQueryVer)						\
	    if((Guid==IID_IUnknownEx) && (InterfaceVersionCheck(dwQueryVer,VER_IUnknownEx)))	\
		    return static_cast<IUnknownEx *>(static_cast<BaseInterface*>(this));

    //获取自身接口
    #define GET_MYSELF_INTERFACE(Interface)													\
	    ((Interface *)QueryInterface(IID_##Interface,VER_##Interface))

    //获取指定对象的接口
    #define GET_OBJECT_INTERFACE(Object,Interface)											\
	    ((Interface *)Object.QueryInterface(IID_##Interface,VER_##Interface))

    //获取对象指针的接口
    #define GET_OBJECTPTR_INTERFACE(pObject,Interface)										\
	    ((pObject==NULL)?NULL:((Interface *)pObject->QueryInterface(IID_##Interface,VER_##Interface)))

////////////////////////////////////////////////////////////////////////////////////////////////////////
    //组件辅助类定义
    template<typename IModuleInterface>
    class CTemplateHelper
    {
	#ifdef _UNICODE
	#define mysprintfs _swprintf_p
	#else
	#define mysprintfs _snprintf_s
	#endif
	    //变量定义
    protected:
	    LPCSTR							m_pszFuncName;					//函数名称
	    LPCTSTR							m_pszModuleDll;					//组件名称
	    HINSTANCE						m_hDllInstance;					//DLL句柄
	    REFGUID							m_GuidInterface;				//接口标识
	    const DWORD						m_dwVersion;					//接口版本
	    IModuleInterface				*m_pIModuleInterface;			//模块接口
	    TCHAR							m_szErrorMsg[LEN_ERROR];		//错误信息

	    //函数定义
    public:
	    //构造函数
        CTemplateHelper(REFGUID refGuid,DWORD dwVer,LPCTSTR pszModuleDll,LPCSTR pszFuncName)
		    :m_GuidInterface(refGuid),m_dwVersion(dwVer)
	    {
            m_szErrorMsg[0]=0;
		    m_hDllInstance=NULL;
		    m_pIModuleInterface=NULL;
		    m_pszFuncName=pszFuncName;
		    m_pszModuleDll=pszModuleDll;
	    }
	    //析构函数
	    virtual ~CTemplateHelper()
	    {
		    CloseInstance();
	    }

	    //功能函数
    public:
	    //创建函数
	    bool CreateInstance()
	    {
		    CloseInstance();
		    try
		    {	
			    //加载DLL
			    m_hDllInstance=LoadLibrary(m_pszModuleDll);
			    if(m_hDllInstance==NULL)
			    {
				    mysprintfs(m_szErrorMsg,sizeof(m_szErrorMsg),TEXT("%s 加载失败,错误代码: %ld !"),m_pszModuleDll,GetLastError());
				    return false;
			    }
			    //查找导出函数
			    ModuleCreateFunc * pCreateFunc=(ModuleCreateFunc *)GetProcAddress(m_hDllInstance,m_pszFuncName);
			    if(pCreateFunc==NULL)
			    {
				    mysprintfs(m_szErrorMsg,sizeof(m_szErrorMsg),TEXT("%s 组件创建函数 %s 不存在,错误代码:%ld !"),
					    m_pszModuleDll,m_pszFuncName,GetLastError());
				    return false;
			    }
			    //创建组件
			    m_pIModuleInterface=(IModuleInterface *)pCreateFunc(m_GuidInterface,m_dwVersion);
			    if(m_pIModuleInterface==NULL)
			    {
				    mysprintfs(m_szErrorMsg,sizeof(m_szErrorMsg),TEXT("%s 组件创建函数 %s 创建组件失败!"),
					    m_pszModuleDll,m_pszFuncName);
				    return false;
			    }
		    }
		    catch(...)
		    {
			    mysprintfs(m_szErrorMsg,sizeof(m_szErrorMsg),TEXT("%s 组件创建函数 %s 发生了异常错误"),
				    m_pszModuleDll,m_pszFuncName);
                return false;
		    }

		    return true;
	    }
	    //关闭函数
	    bool CloseInstance()
	    {
		    if(m_pIModuleInterface!=NULL)
		    {
			    m_pIModuleInterface->Release();
			    m_pIModuleInterface=NULL;
		    }
		    if(m_hDllInstance!=NULL)
		    {
			    FreeLibrary(m_hDllInstance);
			    m_hDllInstance=NULL;
		    }
		    return true;
	    }
	    //是否有效
	    bool IsValid()
	    {
		    if(m_pIModuleInterface==NULL) return false;
		    return m_pIModuleInterface->IsValid();
	    }
	    //获取错误
	    LPCTSTR GetErrorMessage(){return m_szErrorMsg;}
		LPCTSTR GetDllName(){return m_pszModuleDll;}

	    //辅助函数
    public:
	    //指针重载
	    IModuleInterface * operator->(){return m_pIModuleInterface;}
	    //获取接口
	    IModuleInterface * GetInterface(){return m_pIModuleInterface;}
    };

#endif  //WIN32

#endif //__DEIRLYM_IUNKNOWN_EX_HEAD_FILE__
