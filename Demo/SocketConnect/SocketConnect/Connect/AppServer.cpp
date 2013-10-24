//
//  AppServer.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "AppServer.h"
#include "ClientConnect.h"
#include "AppConnect.h"
#include "Packets/PacketAll.h"

AppServer::AppServer()
{
    this->init();
}

AppServer::~AppServer()
{
    this->stopConnect();
}

bool AppServer::init()
{
    sendthread = 0;
    recvthread = 0;
    heartBeatThread = 0;
    processthread = 0;
    
    reconnectCount = 0;
    this->initConnect();
    
    return true;
}

bool AppServer::initOffline()
{
    return true;
}

int AppServer::checkNet()
{
    return NETNOT;
}

static void * handleSendThread(void *)
{
    while (true) {
        sleep(0.1);
        if (INSTANCE(ClientConnect)->processSend() < 0) {
            std::cout << "Lose connection\n";
        }
    }
    return NULL;
}

static void * handleRecvThread(void *)
{
    while (true) {
        sleep(0.1);
        INSTANCE(ClientConnect)->processRecv();
    }
    return NULL;
}

static void * startHeartbeat(void *)
{
    while (true) {
        sleep(HEART_BEAT);
        AppConnect::server()->sendHeartBeat(NULL);
    }
    return NULL;
}

static void * handleProcessThread(void *)
{
    while (true) {
        sleep(0.1);
        INSTANCE(ClientConnect)->processCommand();
    }
    return NULL;
}

void AppServer::sendInMainThread(float)
{
    if (INSTANCE(ClientConnect)->processSend() < 0) {
        std::cout << "Lose connection\n";
    }
}

void AppServer::recvInMainThread(float)
{
    INSTANCE(ClientConnect)->processRecv();
}

void AppServer::heartbeatInMainThread(float)
{
    AppConnect::server()->sendHeartBeat(NULL);
}

void AppServer::processInMainThread(float)
{
    INSTANCE(ClientConnect)->processCommand();
}

void AppServer::initConnect()
{
#ifdef CHECK_NETWORK
    // TODO: check net & show tip
#endif
    if (INSTANCE(ClientConnect)->connectServer(AppConnect::client()->ip, AppConnect::client()->port)) {
        if (sendthread == 0) {
            pthread_create(&sendthread, NULL, handleSendThread, NULL);
        }
        if (recvthread == 0) {
            pthread_create(&recvthread, NULL, handleRecvThread, NULL);
        }
#if defined(HEARTBEAT_ENABLE) && HEARTBEAT_ENABLE == 1
        if (heartBeatThread == 0) {
            pthread_create(&heartBeatThread, NULL, startHeartbeat, NULL);
        }
#endif
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
        if (processthread == 0) {
            pthread_create(&processthread, NULL, handleProcessThread, NULL);
        }
#elif COCOS2D_ENGINE
        this->m_bIsRunning = true;
        this->unschedule(schedule_selector(AppServer::processInMainThread));
        this->schedule(schedule_selector(AppServer::processInMainThread), 0.1);
#else
        // other platform or engine
#endif
        INSTANCE(ClientConnect)->setConnected(true);
        std::cout << "*** Connect to " << AppConnect::client()->ip << ":" << AppConnect::client()->port << " ***\n";
        
    } else {
        AppConnect::client()->connectFail();
    }
}

void AppServer::restartConnect()
{
    this->stopConnect();
    this->initConnect();
}

bool AppServer::checkHeartbeat()
{
    return true;
}

int AppServer::connectOverTime()
{
    return -1;
}

void AppServer::stopConnect()
{
    pthread_cancel(sendthread);
    pthread_cancel(recvthread);
    pthread_cancel(processthread);
    pthread_cancel(heartBeatThread);
    
    sendthread = 0;
    recvthread = 0;
    processthread = 0;
    heartBeatThread = 0;
    
    INSTANCE(ClientConnect)->closeSocket();
}

bool AppServer::sendRequest(ushort packetId, void * data)
{
    switch (packetId) {
        case PACKET_HEARTBEAT:
            this->sendHeartBeat(data);
            break;
            
        default:
            break;
    }
    return true;
}

#pragma mark - == Send Packet == -

void AppServer::sendHeartBeat(void * data)
{
    PACKET(PacketHBeat, PACKET_HEARTBEAT);
    SEND_PACKET(packet)
}

void AppServer::sendTest(std::string msg)
{
    PACKET(PacketTest, PACKET_TEST);
#ifdef PACKET_WITH_JSON
    std::string data = "{\"msg\":" + msg + "}";
    packet->setData(data, 0, false);
#else
    packet->msg = msg;
#endif
    SEND_PACKET(packet)
}


