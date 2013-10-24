//
//  AppServer.h
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__AppServer__
#define __NewProject__AppServer__

#include <iostream>
#include "ConnectDefine.h"

#if COCOS2D_ENGINE
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
class AppServer : public cocos2d::CCNode
#else
using namespace std;
class AppServer
#endif
{
    
public:
    pthread_t   sendthread;         // thread to send message
    pthread_t   recvthread;         // thread to receive message
    pthread_t   heartBeatThread;    // thread to continue heart beat
    pthread_t   processthread;      // thread to process message
    
    int reconnectCount;
    
    AppServer();
    virtual ~AppServer();
    virtual bool init();
    virtual bool initOffline();
    
    int checkNet();
    void initConnect();
    void restartConnect();
    bool checkHeartbeat();
    int connectOverTime();
    void stopConnect();
    void sendInMainThread(float t = 0);
    void recvInMainThread(float t = 0);
    void heartbeatInMainThread(float t = 0);
    void processInMainThread(float);
    
    /**
     @brief     do some request in main thread
     @param     packetId : ushort
     @param     data : void *
     @return    state
     */
    bool sendRequest(ushort packetId, void * data);
    
#pragma mark -
    
    void sendHeartBeat(void * data);
    void sendTest(string msg);
    
};

#endif /* defined(__NewProject__AppServer__) */
