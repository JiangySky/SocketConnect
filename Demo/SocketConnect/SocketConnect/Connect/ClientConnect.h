//
//  ClientConnect.h
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__ClientConnect__
#define __NewProject__ClientConnect__

#include <iostream>
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "PacketManager.h"

static const std::string ErrMsg[] = {
    "没有错误",
    "数据库错误",
    "内存分配错误",
    "创建角色失败",
    "进入游戏失败",
    "player Id 已存在",
    "player name 已存在",
    "客户端发多次登录请求",
    "判断玩家是否已经登录,把上次登录的题下线，已在其他地方登录",
    "玩家不存在",
    "登录数据不一致(ID name和数据库中的不一致)",
    "金钱不够",
    "铜币不够",
    "玩家等级错误",
};

#define DATA_LOCK       0
#define PROCESS_LOCK    0
#define READ_BUFF       1

class ClientConnect {
    bool                    isConnect;
    int                     reconnectCount;
    
public:
    CLASS_INSTANCE(ClientConnect)
    
    int                     m_socket;
    int                     m_gamesocket;
    SocketInputStream *     packetBuff;
    SocketInputStream *     inputStream;
    SocketOutputStream *    outputStream;
    int                     netDelay;
    
    ushort m_key1, m_key2, m_key3, m_key4, m_key5;
    
    ClientConnect() : m_key1(1), m_key2(1), m_key3(1), m_key4(1), m_key5(1) {};
    virtual ~ClientConnect();
    bool init();
    bool connected();
    void setConnected(bool conn, bool needTip = false);
    
    /**
     @brief     create socket
     @return    state number
     */
    int createSocket();
    int createGameSocket();
    void closeSocket();
    
    /**
     @brief     connect to host with port
     @param     ip : string & port : int
     @return    state number
     */
    bool connectServer(std::string ip, int port);
    void connectGame(std::string ip, int port);
    bool connectServer(std::string ip, int port, bool nonblock);
    void connectGame(std::string ip, int port, bool nonblock);
    
    bool writeDataFinish();
    void switchSocket(short flag);
    
    /**
     @brief     process with stream in single thread
     @return    state number
     */
    int processSend();
    int processRecv();
    bool processCommand();
    
    /**
     @brief     send packet to server
     @return    send result
     */
    bool sendPacket(Packet * packet);
};

#endif /* defined(__NewProject__ClientConnect__) */
