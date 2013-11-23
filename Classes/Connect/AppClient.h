//
//  AppClient.h
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__AppClient__
#define __NewProject__AppClient__

#include <iostream>
#include "ConnectDefine.h"
#include "Packets/PacketAll.h"

#if COCOS2D_ENGINE
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
class AppClient : public cocos2d::CCObject
#else
using namespace std;
class AppClient
#endif
{
    
public:
    string ip;
    int port;
    float serverTipDelay;
    
    AppClient();
    virtual ~AppClient();
    virtual bool init();

    void startServer();
    void startServer(string dstIP, int dstPort);
    void reconnect();
    void reconnect(string dstIP, int dstPort);
    void connectFail(bool isLost = false);
    
    /**
     @brief     do some request in main thread
     @param     packetId : ushort
     @param     data : void *
     @return    state
     */
    bool onRequest(ushort packetId, void * data);
    
    /**
     @brief     process response in main thread
     @param     packet : Packet
     */
    void onResponse(Packet * packet);
    
    /**
     @brief     process error response in main thread
     @param     packet : Packet
     */
    void onResponseError(Packet * packet);
    
    /**
     @brief     process server tip response in main thread
     @param     packet : PacketServerTip
     */
    void onResponseServerTip(PacketServerTip * packet);
#if COCOS2D_ENGINE
    void resetServerTip(float t = 0);
#endif
    
#pragma mark -
    
    void onTestResponse(PacketTest * packet);
    
};

#endif /* defined(__NewProject__AppClient__) */
