//
//  IMDelegate.h
//  REducation
//
//  Created by changxm on 14-3-22.
//  Copyright (c) 2014年 changxm. All rights reserved.
//

#ifndef INetDispatcher_IMDelegate_h
#define INetDispatcher_IMDelegate_h

#define BYTE Byte



// NetDispatch-AVManager 之间交互协议
@protocol AVDelegate <NSObject>

- (void) onReceiveAudioDataWithBytes:(unsigned char *)bytes length:(int)len sequence:(unsigned short)wSequence;

//发送音频response
- (BOOL) didReceiveAudioResp:(unsigned short)sequenceID;
@end


// 网络底层-AVManager 之间交互协议
@protocol AudioDataDelegate <NSObject>

-(BOOL)sendAudioData:(unsigned char *)audioData length:(int)len;

@end


#pragma mark - AVManager
/*
 房间与多媒体模块交互协议 1
 */
@protocol AVManagerDelegate <NSObject>

- (void)startAudioQueue:(int)type;
- (void)stopAudioQueue;
- (void)pauseAudioQueue;
- (void)reStartAudioQueue;
- (void)changeAudioSessionOverrideAudioRoute;

// 设置音频发送状态
- (void)setSendAudioDataState:(BOOL)state;
// 设置音频接受状态
- (void)setReceiveAudioDataState:(BOOL)state;
// 对方接听状态
- (void)setRemoteListenStatus:(int)status;

@end



@protocol NetworkProxySendDelegate <NSObject>

-(BOOL) sendAudioDataWithBytes:(BYTE *)bytes length:(unsigned int)length sequenceID:(unsigned short)sequenceID;
-(BOOL) sendOneToOneAudioDataWithBytes:(BYTE *)bytes length:(unsigned int)length sequenceID:(unsigned short)sequenceID;
-(BOOL) sendP2PAudioDataWityBytes:(BYTE *)bytes length:(unsigned int)length sequenceID:(unsigned short)sequenceID;

// 发送各种类型音频数据
- (BOOL)sendMediaAudioData:(BYTE* )pBuffer dSize:(unsigned short)wDataSize sequenceID:(unsigned short)sID callType:(int)type;

@end



//
//// 界面-SwitchViewController 进入教室  界面交互协议
//@protocol TransmitInRoomDelegate <NSObject>
//
//// 进入教室
//- (void)transmitToRoomViewWithData;
//
//// 离开教室
//- (void)exitClassRoomView;
//
//// 离开教室列表
//- (void)exitRoomListView;
//
//@end
//
//@protocol TransmitInShareScreenDelegate <NSObject>
//
//// 进入屏幕共享界面
//- (void) transmitToShareScreen;
//
//// 退出屏幕共享界面
//- (void) exitShareScreen;
//
//@end
//
//// 发送登录/登出请求代理
//@protocol SendLoginoutDelegate <NSObject>
//
//- (void) sendLoginRequest:(UNDWORD)lluUserId withPassword:(NSString*)strPWD;
//
//@end
//
//
//@protocol ChatRoomReceiveDelegate <NSObject>
//
////手动请求用户列表视频开关状态
//- (void)didReceiveGetVideoStatusResponse:(DWORD)roomID;
////收到某人视频开关状态改变
//- (void)didReceiveSomeVideoStatusResponse:(UNDWORD)userid status:(int)status;
//// 请求聊天室用户列表
//- (void)didReceiveGetChatRoomUsers:(NSMutableArray*)users;
//// 聊天室用户变更type 0注销 1进入
//- (void)didReceiveChatRoomUserChange:(UNDWORD)userid changeType:(int)type;
//
//@end
//
////@protocol IRoomChatDelegate <NSObject>
////
////- (BOOL) didReceiveGetAudioRoomListResponse:(CMDCOMMAND *)pCommand vpData:(void *)pDataBuf dataSize:(DWORD)dwDataSize;
////
////- (BOOL) didReceiveGetVideoStatusResponse:(CMDCOMMAND *)pCommand vpData:(void *)pDataBuf dataSize:(DWORD)dwDataSize;
////
////@end
//
//
//@protocol ShareScreenSendDelegate <NSObject>
//
//- (BOOL) sendHandleShareScreenRequest:(DWORD)dwFlag;
//
//@end
//
//@protocol UserManageReceiveDelegate <NSObject>
//
//@optional
//
//// 登录结果
//- (void)receiveLoginResult:(NSInteger)status studentid:(int)stID username:(NSString* )tName checkcode:(NSString* )cstr;
//
//// 注册结果
//- (void)receiveRegisterResult:(NSInteger)status studentid:(DWORD)stID;
//
//@end
//
//@protocol IUserMangeDelegage <NSObject>
//
//@optional
//// 网络问题 控制服务器断开
//- (BOOL)ctrlServerCloseByNet;
//
//// 登录response
//- (BOOL)didReceivedLoginResponse:(void *)pDataBuf dataSize:(DWORD)wDataSize;
//
//// 登出response
//- (BOOL)didReceivedLogoutResponse:(void *)pDataBuf dataSize:(DWORD)wDataSize;
//
//// 异地登录通知
//- (BOOL)didReceiveNonLocalLoginRequest:(void *)pDataBuf dataSize:(DWORD)wDataSize;
//
//// 连接登录服务器成功通知
//- (BOOL)didConnectLoginSvr:(NSInteger)errCode;
//
//@end
//
//#pragma mark - 教室管理send
///*
// 注:教室管理只管理教室相关，如学生进出,老师点名学生回答问题等
// */
//@protocol IRoomCtrlSendDelegate <NSObject>
//@optional
//// 注册控制服务器
//- (BOOL)sendRegisterRoomCtrlServer:(DWORD)ipstr port:(WORD)port;
//// 注册教室服务器
//- (BOOL)sendRegisterChatRoomServer:(DWORD)ipStr prot:(WORD)port;
//
//// 断开服务器
//- (BOOL)sendCloseServer;
//// 断开聊天服务器
//- (BOOL)sendCloseChatServer;
//// 断开控制服务器
//- (BOOL)sendCloseCtrlServer;
//
//@end
//
////#pragma mark - 管理receive
////@protocol IRoomCtrlReceiveDelegate <NSObject>
////@optional
////
/////////////////////////////////////////////response///////////////////////////////////////////////////////////////
////// 网络问题 控制服务器断开
////- (BOOL)ctrlServerCloseByNet;
////// 网络问题 教室服务器断开
////- (BOOL)chatServerCloseByNet;
////
////// 接收到控制服务器
////- (BOOL)didReceiveCtrlIP:(DWORD)ip andCtrlPort:(WORD)port;
////
//////// 注册控制服务器response
//////- (BOOL)didReceiveRegisterCtrlServerResponse:(CMDCOMMAND *)pCommand vpData:(void *)pDataBuf dataSize:(DWORD)wDataSize;
//////
//////// 注册教室服务器response
//////- (BOOL)didReceiveRegisterChatServerResponse:(CMDCOMMAND *)pCommand vpData:(void *)pDataBuf dataSize:(DWORD)wDataSize;
////
////@end
//
//
//#pragma mark - 界面跟RoomCtrl交互协议
//@protocol RoomControlProtocol <NSObject>
//@optional
//// 注册控制服务器结果返回给界面
//- (BOOL)didReceivedRegCtrlSvrResponse:(int)status;
//
//// 注册教室服务器结果返回给界面
//- (BOOL)didReceivedRegChatSvrResponse:(int)status;
//
//@end
//
//@protocol ClassRoomSendDelegate <NSObject>
//
//
//@end
//
//@protocol ICRoomReceiveDelegate <NSObject>
//
//// 房间列表推送
//- (BOOL)didReceivedClassRoomRequest:(DWORD)sequenceID data:(void *)pDataBuf dataSize:(DWORD)wDataSize;
//
//@end
//
//@protocol UpdateSendRequestListDelegate <NSObject>
//
//@optional
//- (BOOL)updateSendRequestAction;
//
//@end
//
//
//@protocol NetPacketWrapperDelegate <NSObject>
//
//- (BOOL) packetTag:(int) nTag didFinishWrap:(BYTE*)pWrapBuf withLength:(WORD)wLen;
//
//@end
//
//
//@protocol MediaDataTransmitDelegate <NSObject>
//
//-(BOOL)sendVideoDataWithBytes:(BYTE *)bytes length:(DWORD)len sequenceID:(WORD)sID;
//-(BOOL)sendOneToOneVideoDataWithBytes:(BYTE *)bytes length:(DWORD)len sequenceID:(WORD)sID;
//-(BOOL)sendP2PVideoDataWithBytes:(BYTE *)bytes length:(DWORD)len sequenceID:(WORD)sID;
//-(BOOL)sendVideoHeadData:(BYTE *)spsPpsData length:(WORD)len;
//-(BOOL)sendOneToOneVideoHeadData:(BYTE *)spsPpsData length:(WORD)len;
//-(BOOL)sendP2PVideoHeadData:(BYTE *)spsPpsData length:(WORD)len;
//
//// 发送视频数据
//-(BOOL)sendMediaVideoData:(BYTE* )bytes length:(DWORD)len sequenceID:(WORD)sID callType:(int)type;
//@end
//
///*
// 呼叫控制管理:
// 只负责决定要使用的呼叫控制流程以及转发数据
// 不负责实现具体呼叫逻辑
// */
//
//@protocol ChatRoomSendDelegate <NSObject>
//- (BOOL) sendChatRoomReq:(WORD)dwCommandID sequenceID:(WORD)dwSequenceID sendData:(BYTE *)pDataBuff dataSize:(DWORD)dwDataSize;
//- (BOOL) sendGetChatRoomUserList:(WORD)dwCommandID sequenceID:(WORD)dwSequenceID;
//- (BOOL) sendGetChatRoomUserVideoStatus:(WORD)dwCommandID sequenceID:(WORD)dwSequenceID users:(NSArray* )uArray;
//@end
//
//
//
//@protocol GetServerTimeOutDelegate <NSObject>
//
//// 开始获取接入服务器网关
//- (void) onGetGatewaySvrBegin;
//
//// 开始获取获取控制服务器
//- (void) onGetCtrlSvrBegin;
//
//// 成功获取接入服务器网关
//- (void) didReceiveGetwaySvr;
//
//// 成功获取控制服务器
//- (void) didReceiveCtrlSvr;
//
//@end
//
//// 接入服务器消息发送
//@protocol AccessServerSendDelegate <NSObject>
//
//// 注册接入服务器
//- (BOOL) registerAccessServerWithIP:(DWORD)dwIP port:(WORD)wPort checkCode:(const BYTE*)pszCheckCode;
//
//// 注销接入服务器
//- (BOOL) cancelAccessServer;
//
//// 接入服务器建立连接
//- (BOOL) connectAccessServerWithIP:(DWORD)dwIP port:(WORD)wPort;
//
//@end
//
//// 网关消息发送
//@protocol GatewaySendDelegate <NSObject>
//
//// 关闭
//- (BOOL) sendCloseGateWay;
//// 链接
//- (BOOL) sendConnectGateWay:(DWORD)svrIP withPort:(WORD)port;
//// 获取网络ip port
//- (BOOL) sendObtainCtrIPAndPortFromGateWayRequest:(WORD)dwCommandID serverType:(DWORD)type;
//
//@end
//
//// 接收屏幕共享
//@protocol ReceiveScreenShareDelegate <NSObject>
//
//- (void) didReceiveScreenShareReq:(void*)pDataBuf dataSize:(DWORD)wDataSize;
//- (void) didReceiveSPSPPSReq:(void*)pDataBuf dataSize:(DWORD)wDataSize;
//- (void) didReceiveVideoReq:(void*)pDataBuf dataSize:(DWORD)wDataSize;
//
//@end
//
//// 聊天消息
//@protocol ReceiveChatingMsgDelegate <NSObject>
//
//@optional
//- (BOOL) didReceiveTextMsgReq:(DWORD) dwSrcID a:(DWORD) dwDstID b:(WORD) wSrcType c:(WORD) wDstType d:(WORD) wRelationType e:(WORD) wMsgType f:(const char*) lpszMsgBody g:(DWORD) dwFileSize h:(DWORD) dwDuration i:(DWORD) dwMsgID j:(DWORD) dwTimpStap;
//- (BOOL) didReceiveTextMsgResp:(int) nResult a:(DWORD) dwMsgID b:(DWORD) dwSendTime c:(DWORD) dwSequenceID;
//
//@end
//
//#pragma mark - 收到系统消息协议
//@protocol SystemMsgReceiveDelegate <NSObject>
//@optional
//- (BOOL) didReceiveSysMsgReq:(void*)pDataBuf dataSize:(DWORD)wDataSize;
//
//@end
//
//// 群相关系统消息协议
//@protocol GroupSysDelegate <NSObject>
//
//@optional
//// 群解散
//- (void)receiveGroupBroke:(int)gId;
//
//// 群加人
//- (void)receiveAddPeople:(int)gId andInfo:(NSDictionary* )dic;
//// 群删人
//- (void)receiveDelPeople:(int)gId andInfo:(NSDictionary* )dic;
//// 群名字修改
//- (void)receiveModifyGroupName:(int)gId andInfo:(NSDictionary* )dic;
//
//@end
//
//// 好友相关系统消息
//@protocol FriendSysDelegate <NSObject>
//
//@optional
//// 删除好友
//- (void)receiveDelFriend:(int)uId;
//// 添加好友
//- (void)receiveAddFriend:(NSDictionary* )dic;
//
//@end
//
//// 视频显示
//@protocol ShowVideoDelegaate <NSObject>
//
//- (void) showVideoWithYBuf:(BYTE *)yBuf andUBuf:(BYTE *)uBuf andVBuf:(BYTE *)vBuf imgWidth:(int)w imgHeight:(int)h;
//
//@end
//
//
//
//
//
//@protocol MessageHistoryDeleage <NSObject>
//
//@optional
//- (BOOL)didReceiveSendResult:(int)result messageTiem:(int)time sequence:(unsigned short)seq;
//- (BOOL)didSendExchangeCardFailed;
//
//@end


#endif
