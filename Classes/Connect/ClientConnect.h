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

class ClientConnect {
    bool                    isConnect;
    
public:
    CLASS_INSTANCE(ClientConnect)
    
    pthread_mutex_t         mutex;
    int                     m_socket;
    int                     m_gamesocket;
    SocketInputStream *     inputStream;
    SocketOutputStream *    outputStream;

    virtual ~ClientConnect();
    bool init();
    bool connected();
    void setConnected(bool conn);
    
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
     */
    void sendPacket(Packet * packet);
};

#endif /* defined(__NewProject__ClientConnect__) */
