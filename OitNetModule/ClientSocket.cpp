#include "ClientSocket.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#include <netdb.h>

#include "custom_log.h"

////////////////////////////////////////////////////////////////////////
#define SOCK_SUCCESS                0                   //Socket success
#define SOCK_CREATEFAIL             1                   //Create socket fail
#define SOCK_SETIPFAIL               2                   //Set server ip fail
#define SOCK_CONNFAIL               4                   //Socket connect fail
#define SOCK_SELECTFAIL              8                   //Select return fail
#define SOCK_CONN_TIMEOUT          16                  //Socket connect time out
#define SOCK_TIMEOUT                30                  //Socket connect timeout
#define SOCK_ADDR_INVAILID          35                  //ip invailid
///////////////////////////////////////////////////////////////////////////////////

int DomainToIp(const char * domain) {

	int result = 0;
	struct hostent *hptr;
	char **ptr;
	char str[32] = {0};

	LOGW("DOMAIN:%s", domain);
	if ((hptr = gethostbyname(domain)) == NULL) {
		return INADDR_NONE;
	}
	bool ret = false;
	if (hptr->h_addrtype == AF_INET) {
		ptr = hptr->h_addr_list;
		for (; *ptr != NULL; ptr++) {
			inet_ntop(hptr->h_addrtype, *ptr, str, sizeof(str));
			if (str[0] != '\0') {
				ret = true;
				break;
			}
		}
	}
	if(ret==false)
		return INADDR_NONE;
	LOGD("DomainToIp success,IP addr=%s",str);
	return inet_addr(str);
}


//construct function
CClientSocket::CClientSocket()
{
    m_bCloseByServer=false;
    m_SocketState=SocketState_NoConnect;
    m_hSocket=INVALID_SOCKET;
    m_wRecvSize=0;
    m_wServerPort=9100;
    m_nMaxfd = 0;
	m_dwServerIP=0;
    m_dwState = THREAD_WAIT;
    m_bigPacketBuff = NULL;
    m_bigPacketBuffSize = 0;

	m_pIClientSocketSink=NULL;
}
CClientSocket::~CClientSocket()
{
    CThreadLockHandle LockHandle(&m_ThreadLock);
    if(m_bigPacketBuff!=NULL){
    	free(m_bigPacketBuff);
    	m_bigPacketBuff==NULL;
    }
}

bool CClientSocket::SetSocketSink(void * pISocketSink)
{
	if(pISocketSink==NULL)
		return false;
	m_pIClientSocketSink=(IClientSocketSink *)pISocketSink;

	return true;
}

//开始网络线程
bool CClientSocket::StartSocketThread()
{
	//开启网络线程
	return StartThread();
}

//停止网络线程
bool CClientSocket::StopSocketThread()
{
	m_bRun=false;
	if (m_hSocket!=INVALID_SOCKET)
    {        
        CloseSocket(false);
        m_hSocket=INVALID_SOCKET;
    }

	bool bStoped=StopThread();	
    
	return bStoped;
}

//Connect to server
bool CClientSocket::ConnectToServer(DWORD dwServerIP,WORD wPort)
{
	if(wPort==0) return false;

	TCHAR szServerIP[32]=TEXT("");
	in_addr inaddr;
	//memcpy(&inaddr,&dwServerIP,sizeof(dwServerIP));
	inaddr.s_addr=dwServerIP;
	TCHAR *pIpAddr=inet_ntoa(inaddr);
	LOGD("---Connect to server,szServerIP=%s,wPort=%d\n", pIpAddr,wPort);
	m_dwServerIP=dwServerIP;
    m_wServerPort=wPort;
    m_dwState=THREAD_PREPARE;

    return true;
}

int CClientSocket::ipv4_ipv6(char*address)
{

    struct addrinfo *result;
    struct addrinfo *res;
    
    struct addrinfo addrCriteria;
    memset(&addrCriteria,0,sizeof(addrCriteria));
    addrCriteria.ai_family=AF_UNSPEC;
    addrCriteria.ai_socktype=SOCK_STREAM;
    addrCriteria.ai_protocol=IPPROTO_TCP;
    
    //判定网络类型    指定特定接口信息
    int error = getaddrinfo(address, NULL, &addrCriteria, &result);
    if (error == 0)
    {
        for (res = result; res != NULL; res = res->ai_next)
        {
            if (AF_INET6 == res->ai_addr->sa_family)
            {
                sockaddr_in6 *pAddr6=(sockaddr_in6 *)res->ai_addr;
                memcpy(m_dwServerIp6, &pAddr6->sin6_addr,sizeof(m_dwServerIp6));
            
                return AF_INET6;
               
            }
            else if (AF_INET == res->ai_addr->sa_family)
            {
                sockaddr_in *pAddr=(sockaddr_in *)res->ai_addr;
                m_dwServerIP=pAddr->sin_addr.s_addr;
                return AF_INET;
            
            }
        }
    }
    return 0;
}

bool CClientSocket::ConnectToServer(LPCTSTR lpszServerIP,WORD wPort)
{
	if(wPort==0) return false;
    
     m_IpType = ipv4_ipv6((char*)lpszServerIP);
    
    if(m_IpType==0) return false;
    //LOGD("---Connect to server,szServerIP=%s,wPort=%d", lpszServerIP,wPort);
    
    m_wServerPort=wPort;
    m_dwState=THREAD_PREPARE;
	

    return true;
}

int IpV4ToIpV6(DWORD dwIpVal,sockaddr_in6 *pIpV6)
{
    in_addr socketAddr;
    memset(&socketAddr,0,sizeof(socketAddr));
    socketAddr.s_addr=dwIpVal;
    BYTE *pIpAddr=(BYTE *)&dwIpVal;//inet_ntoa(socketAddr);
    char szIpTmp[32]="";
    sprintf(szIpTmp, "64:ff9b::%02x%02x:%02x%02x",pIpAddr[0],pIpAddr[1],pIpAddr[2],pIpAddr[3]);
    printf("%s",szIpTmp);
    //    mysprintf(szIpTmp,32,"::%02x%02x:%02x%02x",pIpAddr[0],pIpAddr[1],pIpAddr[2],pIpAddr[3]);
    //lstrcat(szIpTmp,pIpAddr);
    int nRet=inet_pton(AF_INET6,szIpTmp,&pIpV6->sin6_addr);
    return nRet;
}

 
//Send data function
bool CClientSocket::SendData(WORD mainCmd, WORD subCmd,DWORD dwSequenceID,void * pData,WORD wDataSize)
{    
    if (m_hSocket==INVALID_SOCKET){
    	 LOGE("m_hSocket==INVALID_SOCKET");
		 m_dwState=THREAD_PREPARE;
    	 return false;
    }
    if (m_SocketState!=SocketState_Connected) {
    	LOGE("m_SocketState!=SocketState_Connected");
		m_dwState=THREAD_PREPARE;
    	return false;
    }
    
    ASSERT(wDataSize<=SOCKET_PACKAGE);
    if (wDataSize>SOCKET_PACKAGE) return false;
    
    if(wDataSize<=SOCKET_PACKAGE){
    	return sendpacket(mainCmd,subCmd,dwSequenceID,pData,wDataSize,0);
    }
    WORD wPackCount=(wDataSize+SOCKET_PACKAGE-1)/SOCKET_PACKAGE;
	WORD wPackIndex=1;
	WORD wPackNumber=0;
	WORD wSendSize=0;
	const BYTE *pSrcData=(const BYTE *)pData;
	for(WORD i=0;i<wPackCount;i++)
	{
		wSendSize=wDataSize>=SOCKET_PACKAGE?SOCKET_PACKAGE:wDataSize;
		wPackNumber=(wPackIndex<<8)|wPackCount;
		sendpacket(mainCmd,subCmd,dwSequenceID,pSrcData,wSendSize,wPackNumber);
		pSrcData+=wSendSize;
		wDataSize-=wSendSize;
	}
	return true;
}

bool CClientSocket::sendpacket(WORD mainCmd, WORD subCmd,DWORD dwSequenceID,const void * pData,WORD wDataSize,WORD wPackNumber){
	BYTE cbDataBuffer[SOCKET_BUFFER];

	WORD wTotalSize=sizeof(CMD_Head);
	CMD_Head * pHead=(CMD_Head *)cbDataBuffer;
	pHead->wMainCmd=mainCmd;
	pHead->wSubCmd=subCmd;
	pHead->dwCommandID=htonl(pHead->dwCommandID);
	pHead->dwSequenceID=htonl(dwSequenceID);

	if(wDataSize>0){
		memcpy(pHead+1,pData,wDataSize);
		wTotalSize += wDataSize;
	}

	pHead->wPackLength=wTotalSize;
	pHead->wPackNumber=wPackNumber;
	pHead->dwTotalLength=htonl(pHead->dwTotalLength);

	return SendBuffer(cbDataBuffer,wTotalSize);
}

//Close socket
bool CClientSocket::CloseSocket(bool bNotify)
{   
	CThreadLockHandle LockHandle(&m_ThreadLock);

    bool bClose=(m_hSocket!=INVALID_SOCKET);
    if(bClose==false)
       return true;

    if (m_hSocket!=INVALID_SOCKET)
    {        
        closesocket(m_hSocket);
        m_hSocket=INVALID_SOCKET;
    }
	
    m_SocketState=SocketState_NoConnect;   
    m_wRecvSize=0;   
    m_dwState = THREAD_WAIT;

	if(bNotify && m_pIClientSocketSink!=NULL)
	{
		m_pIClientSocketSink->OnSocketClose((IClientSocket *)this,m_bCloseByServer);
	}
   
    return true;
}

//网络连接
bool CClientSocket::OnSocketNotifyConnect(int iErrorCode)
{
	//判断状态	
	if (iErrorCode==0)
	{
		m_SocketState=SocketState_Connected;
		m_dwState=THREAD_OK;
	}
	else
	{
		CloseSocket(false);
	}
	//发送通知
	TCHAR szErrorDesc[128]=TEXT("");
	GetConnectError(iErrorCode,szErrorDesc,sizeof(szErrorDesc));
	if(m_pIClientSocketSink!=NULL)
	{
		m_pIClientSocketSink->OnSocketConnect(iErrorCode,szErrorDesc,(IClientSocket *)this);
	}
	return true;
}

//网络读取
bool CClientSocket::OnSocketNotifyRead()
{
	try
	{
		struct timeval tv;
		tv.tv_sec=1;
		tv.tv_usec=0;
		FD_ZERO(&m_rfds);
		FD_SET(m_hSocket,&m_rfds);
		int nRetVal=select(m_nMaxfd+1,&m_rfds,NULL,NULL,&tv);

		CThreadLockHandle LockHandle(&m_ThreadLock);
		if(m_hSocket==INVALID_SOCKET) 
			return true;

		if(nRetVal==-1)
		{
    		LOGD("select error\n");
			throw TEXT("Exit,select error!"); 
		}
		else if(nRetVal==0)
		{
    		//LOGD("select time out");
		}
		else if(FD_ISSET(m_hSocket,&m_rfds))
		{   
			//读取数据
			//LOGD("socket begin recv data...");
			int iRetCode=recv(m_hSocket,(char *)m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);
			//LOGD("recv function was returned.");
			if (iRetCode==0 || iRetCode==SOCKET_ERROR){
				LOGE("recv error\n");
				throw TEXT("socket was close，read data faild");
			}
		
			m_wRecvSize+=iRetCode;
		
			//变量定义
			WORD wPacketSize=0;
            CMD_Head head;
			BYTE cbDataBuffer[SOCKET_PACKAGE+sizeof(CMD_Head)];	
			CMD_Head * pHead=(CMD_Head *)m_cbRecvBuf;
			while (m_wRecvSize>=sizeof(CMD_Head))
			{
					
                CMD_Head &tmpHead=*pHead;
                head.dwCommandID=tmpHead.dwCommandID;
                head.dwSequenceID=tmpHead.dwSequenceID;
                head.dwTotalLength=tmpHead.dwTotalLength;
                                
				head.dwTotalLength=ntohl(head.dwTotalLength);
 
				//效验参数
				wPacketSize=head.wPackLength;;
				//LOGD("wPacketSize=%d",wPacketSize);
				ASSERT(wPacketSize<=(SOCKET_PACKAGE+sizeof(CMD_Head)));			
				if (wPacketSize>(SOCKET_PACKAGE+sizeof(CMD_Head))) 
				{
					LOGE("data len is wrong\n");
					throw TEXT("data len is wrong");
				}
				ASSERT(wPacketSize>=sizeof(CMD_Head));
				if(wPacketSize<sizeof(CMD_Head))
				{
					LOGE("datalen < sizeof(CMD_Head)\n");
					throw TEXT("data len is small");
				}

				if (m_wRecvSize<wPacketSize) {
					LOGE("m_wRecvSize:%d---<wPacketSize:%d\n",m_wRecvSize,wPacketSize);
					return true;
				}

				//拷贝数据					
				memcpy(cbDataBuffer,m_cbRecvBuf,wPacketSize);
				m_wRecvSize-=wPacketSize;
				memmove(m_cbRecvBuf,m_cbRecvBuf+wPacketSize,m_wRecvSize);

				CMD_Command cmd={0};

				//解释数据
				WORD wDataSize=wPacketSize-sizeof(CMD_Head);
				void * pDataBuffer=cbDataBuffer+sizeof(CMD_Head);
				cmd.dwCommandID=ntohl(head.dwCommandID);
				cmd.dwSequenceID=ntohl(head.dwSequenceID);

				if(head.wPackNumber>0)
				{
					LOGE("----------------------nnnn\n");
					recvSubPacket(cmd,head.wPackNumber,pDataBuffer,wDataSize);
					continue;
				}

				if(m_pIClientSocketSink!=NULL)
				{
                    bool bSuccess=m_pIClientSocketSink->OnSocketRead(cmd,pDataBuffer,wDataSize,this);
                    if(!bSuccess)
                    {
                        LOGD("--->>>----- user return false---");
                        throw TEXT("user return false---");
                    }					
				}
			}
		}					
	}
	catch (...) 
	{
  		LOGD("<<<--------- catch close socket");
		CloseSocket(false);
	}
	return true;
}

void CClientSocket::recvSubPacket(CMD_Command &command,WORD packetInfo,void *pBuffer,WORD wDataSize){
	WORD wIndex=HIBYTE(packetInfo);
	WORD wCount=LOBYTE(packetInfo);
	if(wIndex<=1)	//第一个包
	{
		m_bigPacketSize = 0;
	}
	//LOGD("recv sub packet index=%d,wCount=%d",wIndex,wCount);

	if(m_bigPacketSize+wDataSize>m_bigPacketBuffSize){
		m_bigPacketBuff = (char*)realloc(m_bigPacketBuff,m_bigPacketSize+wDataSize);
		m_bigPacketBuffSize = m_bigPacketSize+wDataSize;
	}
	memcpy(m_bigPacketBuff+m_bigPacketSize,pBuffer,wDataSize);
	m_bigPacketSize+=wDataSize;
	if(wIndex>=wCount && m_pIClientSocketSink!=NULL){
		bool bSuccess=m_pIClientSocketSink->OnSocketRead(command,m_bigPacketBuff,m_bigPacketSize,this);
		if(!bSuccess) CloseSocket(false);
	}
}

//Send data
bool CClientSocket::SendBuffer(void * pBuffer, WORD wSendSize)
{    
    ASSERT(wSendSize!=0);
    ASSERT(pBuffer!=NULL);
    //LOGD("<<<< --------sendbuffer------%d--------",wSendSize);
    WORD wSended=0;
    while (wSended<wSendSize)
    {
        int iErrorCode=send(m_hSocket,(char *)pBuffer+wSended,wSendSize-wSended,0);
        if (iErrorCode==SOCKET_ERROR)
        {
            if(errno!=EINPROGRESS)
            {
                return true;
            }
            return false;
        }
        wSended+=iErrorCode;
    }
    return true;
}
//Thread body function
bool CClientSocket::RepetitionRun()
{
    switch(m_dwState)
    {
    case THREAD_WAIT:
        {
            sleep(1);
            break;
        }
    case THREAD_PREPARE:
        {
            ConnectToServer();
            break;
        }
    case THREAD_OK:
        {
            OnSocketNotifyRead();
            break;
        }
    case THREAD_UNPREPARE:
        {
           CloseSocket(false);
        }
        break;
    }
    return true;
}

bool CClientSocket::isIPv4()
{
    in_addr inaddr;
    //memcpy(&inaddr,&dwServerIP,sizeof(dwServerIP));
    inaddr.s_addr=m_dwServerIP;
    TCHAR *pIpAddr=inet_ntoa(inaddr);
    int count = 0;
    for (int i=0; i<strlen(pIpAddr); i++)
    {
        if (pIpAddr[i] == ':')
        {
            count ++;
        }
    }
    if (count > 1) {
        return false;
    }
    return  true;
}

bool CClientSocket::isIPv6()
{
    
    return true;
}

//Connect to server
bool CClientSocket::ConnectToServer()
{
	//增加关闭连接的功能
	if (m_hSocket!=INVALID_SOCKET)
	{        
		::closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
		m_SocketState=SocketState_NoConnect;
	}

    socklen_t sLen;
    bool connectYet = false;
    struct timeval tv;
    int nRetVal;
    
    if (m_IpType == AF_INET) {
        struct sockaddr_in dest_addr;
        
        //Create socket
        if((m_hSocket=socket(PF_INET,SOCK_STREAM,0))==-1)
        {
            LOGD("Create socket fail\n");
            OnSocketNotifyConnect(SOCK_CREATEFAIL);
            return false;
        }
        int nVal=fcntl(m_hSocket,F_GETFL,0);
        nRetVal=fcntl(m_hSocket,F_SETFL,nVal|O_NONBLOCK);
        
        memset(&dest_addr,0,sizeof(dest_addr));
        dest_addr.sin_family=AF_INET;
        dest_addr.sin_port=htons(m_wServerPort);
        dest_addr.sin_addr.s_addr=m_dwServerIP;

        connectYet = connect(m_hSocket,(struct sockaddr *)&dest_addr,sizeof(dest_addr))!=0;
        
    }else if(m_IpType == AF_INET6){
        struct sockaddr_in6 dest_addr;
        
        //Create socket
        if((m_hSocket=socket(PF_INET6,SOCK_STREAM,0))==-1)
        {
            LOGD("Create socket fail\n");
            OnSocketNotifyConnect(SOCK_CREATEFAIL);
            return false;
        }
        int nVal=fcntl(m_hSocket,F_GETFL,0);
        nRetVal=fcntl(m_hSocket,F_SETFL,nVal|O_NONBLOCK);
        
        int nosigpipe = 1;
        setsockopt(m_hSocket, SOL_SOCKET, SO_NOSIGPIPE, &nosigpipe, sizeof(nosigpipe));
        
        memset(&dest_addr,0,sizeof(dest_addr));
        dest_addr.sin6_len       = sizeof(sockaddr_in6);
        //dest_addr.sin6_addr      = in6addr_any;
        dest_addr.sin6_family=PF_INET6;
        dest_addr.sin6_port=htons(m_wServerPort);
        memcpy(&dest_addr.sin6_addr, m_dwServerIp6, sizeof(dest_addr.sin6_addr));
        
        //inet_pton(AF_INET6,m_dwServerIp6,&dest_addr.sin6_addr);
        
        connectYet = connect(m_hSocket,(struct sockaddr *)&dest_addr,sizeof(dest_addr))!=0;
    }

    //Connect to server
    if(connectYet)
    {
        if(errno!=EINPROGRESS)
        {
            LOGD("Connect to server faild\n");
            OnSocketNotifyConnect(SOCK_SETIPFAIL);
            return false;
        }
    }
		else
		{
        LOGD("Connect to server success\n");                 
        OnSocketNotifyConnect(SOCK_SUCCESS); 
        return true;
		}
		if((int)m_hSocket>m_nMaxfd)
        m_nMaxfd=(int)m_hSocket;
	    
    FD_ZERO(&m_rfds);
    FD_SET(m_hSocket,&m_rfds);
    for(int i=0;i<SOCK_TIMEOUT;i++)
    {        
			tv.tv_sec=1;
			tv.tv_usec=0;
    	nRetVal=select(m_nMaxfd+1,NULL,&m_rfds,NULL,&tv);
    	if(nRetVal==-1)
    	{
         	LOGE("Exit,select error!,%s\n",strerror(errno));
         	OnSocketNotifyConnect(SOCK_SELECTFAIL);
         	return false;
    	}
    	else if(nRetVal==0)
      {		
          continue;
      }
      else if(FD_ISSET(m_hSocket,&m_rfds))
      {
            int error;
            sLen=sizeof(error);
            getsockopt(m_hSocket,SOL_SOCKET,SO_ERROR,&error,&sLen);
            if(error==0) //--connect success
            {
                LOGD("Connect to server success(select)\n");                 
                OnSocketNotifyConnect(SOCK_SUCCESS); 
                return true;
            }
            else
            {
                LOGE("Connect fail,%s\n",strerror(error)); 
                OnSocketNotifyConnect(SOCK_CONNFAIL); 
                return false;           
            }      

       }        
    }

	LOGD("Connect to server 55555\n");
    OnSocketNotifyConnect(SOCK_CONN_TIMEOUT); 
    return false;
}
//Get error describle
void CClientSocket::GetConnectError(int iErrorCode, LPTSTR pszBuffer, WORD wBufferSize)
{
    //Invalidate parameter
    ASSERT(pszBuffer!=NULL);
    if (pszBuffer==NULL) return;
 
    //Error describle
    switch (iErrorCode)
    {
    case SOCK_SUCCESS:                    //no error
        {
            strncpy(pszBuffer,TEXT("oprator success"),wBufferSize);
            break;
        }
    case SOCK_CREATEFAIL:
        {
            strncpy(pszBuffer,TEXT("Create socket fail"),wBufferSize);
            break;
        }
    case SOCK_SETIPFAIL:    //IP Address format error
        {
            strncpy(pszBuffer,TEXT("Destination ip address format is error"),wBufferSize);
            break;
        }
    case SOCK_CONNFAIL:     //Connection refused
        {
            strncpy(pszBuffer,TEXT("连接服务器被拒绝，请稍后再试"),wBufferSize);
            break;
        }
    case SOCK_CONN_TIMEOUT:        //connection timeout
        {
            strncpy(pszBuffer,TEXT("Connection timeout"),wBufferSize);
            break;
        }
    case SOCK_ADDR_INVAILID:{
    	      strncpy(pszBuffer,TEXT("服务器地址无效"),wBufferSize);
    	      break;
    	}
    default:                //default error
        {
            sprintf(pszBuffer,TEXT("Connection errno%d please query help"),iErrorCode);
            break;
        }
    }    
}
