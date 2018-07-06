#ifndef CLIENT_SOCKET_H_H_
#define CLIENT_SOCKET_H_H_

#include "ClientModule.h"
#include "localdef.h"

//////////////////////////////////////////////////////////////////
class CClientSocket:public CServiceThread,public IClientSocket
{
protected:
    DWORD                           m_dwState;                      //Thread state
    bool                            m_bCloseByServer;               //Close mode
    enSocketState                   m_SocketState;					//Connect state
protected:
	int								m_nMaxfd;                      //Max socket value
    fd_set                          m_rfds;                         //SOCKET list
	DWORD							m_dwServerIP;					//Server ip address
    BYTE                            m_dwServerIp6[16];              //server ip 6
    DWORD                           m_IpType;
    
    WORD                            m_wServerPort;                  //Server listent port
    SOCKET                          m_hSocket;                      //Socket connect handle
    WORD                            m_wRecvSize;                    //Recv length
    BYTE                            m_cbRecvBuf[SOCKET_BUFFER];     //Recv buffer
    char*                           m_bigPacketBuff;
    int                             m_bigPacketBuffSize;
    int                             m_bigPacketSize;

public:
	IClientSocketSink				*m_pIClientSocketSink;			//ªÿµ˜Ω”ø⁄
	CThreadLock						m_ThreadLock;					//

public:
    //construct function
    CClientSocket();
    virtual ~CClientSocket();

public:
	//set sink interface
	virtual bool SetSocketSink(void * pISocketSink);
	//ªÒ»°Õ¯¬Á◊¥Ã¨
	virtual enSocketState GetConnectState(){return m_SocketState;}
	//conect to server
	virtual bool ConnectToServer(DWORD dwServerIP,WORD wPort);
	virtual bool ConnectToServer(LPCTSTR lpszServerIP,WORD wPort);
	//Send data function
	virtual bool SendData(WORD mainCmd,WORD subCmd,DWORD dwSequenceID,void * pData,WORD wDataSize);
	//Close socket
	virtual bool CloseSocket(bool bNotify);
	//ø™ ºÕ¯¬Áœﬂ≥Ã
	virtual bool StartSocketThread();
	//Õ£÷πÕ¯¬Áœﬂ≥Ã
	virtual bool StopSocketThread();
	

protected:
     //œﬂ≥ÃÃÂ∫Ø ˝
     virtual bool RepetitionRun();

private:
    //Socet connect
    bool OnSocketNotifyConnect(int iErrorCode);
    //Socket read
    bool OnSocketNotifyRead();
    
    //Helper function
    void recvSubPacket(CMD_Command &command,WORD packetInfo,void *pBuffer,WORD wDataSize);

    bool sendpacket(WORD mainCmd, WORD subCmd,DWORD dwSequenceID,const void * pData,WORD wDataSize,WORD wPackNumber);
private:
    //Send data
    bool SendBuffer(void * pBuffer, WORD wSendSize);
    //Connect to server
    bool ConnectToServer();
    //Get error describle
    void GetConnectError(int iErrorCode, LPTSTR pszBuffer, WORD wBufferSize);
    bool isIPv4();
    bool isIPv6();
    int ipv4_ipv6(char*address);
};

#endif
