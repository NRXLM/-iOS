//
//  NetTool.m
//  REducation
//
//  Created by changxm on 14-3-22.
//  Copyright (c) 2014年 changxm. All rights reserved.
//

#import "NetTool.h"
#import <netinet/in.h>
#import <sys/time.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>

union NetByeOrder
{
	DWORD	dwValue;
	BYTE	cbByte[4];
};

struct tagInt
{
    int nVal1;
    int nVal2;
};

union tagLong
{
    UNDWORD ulVal;
    struct tagInt st;
};

@implementation NetTool

#pragma mark - 将本地字节序转换为网络字节序
+ (UNDWORD) htonl64:(UNDWORD)lluValue
{
	union NetByeOrder nbo;
	nbo.dwValue = 1;
	if(nbo.cbByte[0] == 0)
		return lluValue;
	
	union tagLong t1 = {0};
	union tagLong t2 = {0};
    t1.ulVal = lluValue;
    t2.st.nVal1 = htonl(t1.st.nVal2);
    t2.st.nVal2 = htonl(t1.st.nVal1);
    return t2.ulVal;
}

#pragma mark - 将网络字节序转换为本地字节序
+ (UNDWORD) ntohl64:(UNDWORD)lluSrc
{
	union NetByeOrder nbo;
	nbo.dwValue = 1;
	if(nbo.cbByte[0] == 0)
		return lluSrc;
    
	union tagLong t1 = {0};
	union tagLong t2 = {0};
    t1.ulVal = lluSrc;
    t2.st.nVal1 = ntohl(t1.st.nVal2);
    t2.st.nVal2 = ntohl(t1.st.nVal1);
    return t2.ulVal;
}

// 时间
+ (UNDWORD) getNowTime
{
    struct timeval t;
    gettimeofday(&t,NULL);
    long long dwTime = ((long long)1000000 * t.tv_sec + (long long)t.tv_usec)/1000;
    return dwTime;
}

// 将做任意时间适配为秒
+ (UNDWORD) adaptToSecondsOfTimeSince1970:(UNDWORD)time
{
    UNDWORD timeTemple = (UNDWORD)[[NSDate date] timeIntervalSince1970] * 2;
    static UNDWORD refTimeTemple = (UNDWORD)44 * 365 * 24 * 60 * 60 * 2;
    timeTemple = MAX(timeTemple, refTimeTemple);
    
    UNDWORD adaptedTime = time;
    
    while (adaptedTime > timeTemple)
    {
        adaptedTime /= 10;
    }
    
    return adaptedTime;
}

#pragma mark - 判断是否同一网段
+ (BOOL) isInSameLAN:(DWORD)dwPrivateIP_A privateIP_B:(DWORD)dwprivateIP_B publicIP_A:(DWORD)dwPublicIP_A publicIP_B:(DWORD)dwPublicIP_B mask_A:(DWORD)dwMask_A mask_B:(DWORD)dwMask_B
{
    //return true;
    if (dwPublicIP_A != dwPublicIP_B) return false;
    
    DWORD dwPrivateNetID_A = dwPrivateIP_A & dwMask_A;
    DWORD dwPrivateNetID_B = dwPublicIP_B & dwMask_B;
    
    return (dwPrivateNetID_A == dwPrivateNetID_B);
}

#pragma mark - 获取子网掩码
+ (DWORD) getSubnetMaskFromSocket:(int)sock
{
    struct sockaddr_in sin;
    struct ifreq ifr;
    if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0)
    {
        return 1;
    }
    
    memcpy(&sin, &ifr.ifr_name, sizeof(sin));
    printf("zwym,%s/n----%u",inet_ntoa(sin.sin_addr),sin.sin_addr.s_addr);
    
    return sin.sin_addr.s_addr;
}

#pragma mark - 将点分十进制ip转换为数值ip
+ (DWORD)convertIPAddress:(NSString*)strIPAddr
{
    if (!strIPAddr)
    {
        return 0;
    }
    DWORD dwIP = inet_addr([strIPAddr UTF8String]);
    if(dwIP == INADDR_NONE)
    {
        struct hostent * phost = gethostbyname([strIPAddr UTF8String]);
        if(!phost) return 0;
        
        struct in_addr ** pList = (struct in_addr **)phost->h_addr_list;
        NSString * addrIP = [[NSString alloc] initWithUTF8String:inet_ntoa(*pList[0])];
        dwIP = (DWORD) inet_addr([addrIP UTF8String]);
        [addrIP release];
    }
    return dwIP;
}

#pragma mark - 装饰数值ip转换为点分十进制
+ (NSString*) convertIPToString:(DWORD)dwIP
{
    struct in_addr inaddr;
    inaddr.s_addr = dwIP;
    return [NSString stringWithFormat:@"%s", inet_ntoa(inaddr)];
}

#pragma mark - 将ip，端口号转为socket地址
+ (struct sockaddr_in) sockAddressFromIP:(DWORD)dwIP withPort:(WORD)wPort
{
    struct sockaddr_in sockAddr;
    sockAddr.sin_len = sizeof(struct sockaddr_in);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = dwIP;
    sockAddr.sin_port = htons(wPort);
    bzero(&(sockAddr.sin_zero), sizeof(sockAddr.sin_zero));
    return sockAddr;
}

// int型转为rgb
CColor TORGB(int color)
{
    CColor c;
    c.r = 0xFF & color;
    c.g = 0xFF00 & color;
    c.g >>= 8;
    c.b = 0xFF0000 & color;
    c.b >>= 16;
    return c;
}

int TOINT(CGFloat r, CGFloat g, CGFloat b)
{
    CColor c;
    c.r = r*255;
    c.g = g*255;
    c.b = b*255;
    c.a = 1;
    return ((uint)((c.r|((ushort)(c.g)<<8))|(((uint)c.b)<<16)));
}

// RGB转为int型
int ParseRGB(UIColor* color)
{
    CColor c = RGBAFromCGColor(color.CGColor);
    return ((uint)((c.r|((ushort)(c.g)<<8))|(((uint)c.b)<<16)));
}

CColor RGBAFromCGColor(CGColorRef color)
{
    CColor rgba;
    
    int numComponents = CGColorGetNumberOfComponents(color);
    
    if (numComponents >= 3)
    {
        const CGFloat *tmComponents = CGColorGetComponents(color);
        CGFloat red = tmComponents[0];
        CGFloat green = tmComponents[1];
        CGFloat blue = tmComponents[2];
        //CGFloat alpha = tmComponents[3];
        
        rgba.r = red*255;
        rgba.g = green*255;
        rgba.b = blue*255;
        rgba.a = 1;
    }
    return rgba;
}

//读双字
BYTE * ReadDword(BYTE * pData,DWORD & dwValue)
{
    if (pData == NULL){return NULL;}
    
	memcpy(&dwValue,pData,sizeof(DWORD));
	dwValue=ntohl(dwValue);
	return pData+sizeof(DWORD);
}

//读int32
BYTE * ReadInt32(BYTE * pData,__int32_t & nValue)
{
	if (pData == NULL){return NULL;}
	memcpy(&nValue,pData,sizeof(__int32_t));
	nValue=ntohl(nValue);
	return pData+sizeof(__int32_t);
}

//读字
BYTE * ReadWord(BYTE * pData,WORD & wValue)
{
	if (pData == NULL){return NULL;}
	memcpy(&wValue,pData,sizeof(WORD));
	wValue=ntohs(wValue);
	return pData+sizeof(WORD);
}

BYTE * ReadInt16(BYTE * pData,__int16_t & nValue)
{
    if (pData == NULL){return NULL;}
	memcpy(&nValue,pData,sizeof(__int16_t));
	nValue=ntohs(nValue);
	return pData+sizeof(__int16_t);
}

//读BYTE
BYTE * ReadByte(BYTE * pData,BYTE & cbValue)
{
	if (pData == NULL){return NULL;}
	cbValue=pData[0];
	return pData+1;
}

//读取指定长度
BYTE * ReadToBuffer(BYTE * pData,void * pDest,WORD wLength)
{
	if (pData == NULL){return NULL;}
	memcpy(pDest,pData,wLength);
	return pData+wLength;
}

//WORD高八位
WORD HIBYTE(WORD num)
{
    BYTE high;
    high = num>>8;
    return high;
}

//WORD低八位
WORD LOBYTE(WORD num)
{
    BYTE low;
    low = num&0x00ff;
    return low;
}

@end
