//
//  AppClient.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "AppClient.h"
#include "AppConnect.h"

AppClient::AppClient()
{
    this->init();
}

AppClient::~AppClient()
{

}

bool AppClient::init()
{
    ip = SERVERIP;
    port = SERVERPROT;
    serverTipDelay = 0.5;
    return true;
}

void AppClient::startServer()
{
    AppConnect::server();
}

void AppClient::startServer(string dstIP, int dstPort)
{
    ip = dstIP;
    port = dstPort;
    AppConnect::server();
}

void AppClient::reconnect()
{
    AppConnect::server()->restartConnect();
}

void AppClient::reconnect(string dstIP, int dstPort)
{
    ip = dstIP;
    port = dstPort;
    AppConnect::server()->restartConnect();
}

void AppClient::connectFail(bool isLost)
{
    if (isLost) {
        // TODO: reconnect & UI change
    } else {
        std::cout << "无法建立连接，请检查网络" << std::endl;
        // TODO: reconnect & UI change
    }
}

bool AppClient::onRequest(ushort packetId, void * data)
{
    return AppConnect::server()->sendRequest(packetId, data);
}

void AppClient::onResponse(Packet * packet)
{
    switch (packet->receivedPacketId) {
        case PACKET_ERROR:
            this->onResponseError(packet);
            break;
        case PACKET_SERVER_TIP:
            this->onResponseServerTip((PacketServerTip *)packet);
            break;
        case PACKET_TEST:
            this->onTestResponse((PacketTest *)packet);
            break;
            
        default:
            break;
    }
}

#pragma mark - == Process response single == -

void AppClient::onResponseError(Packet *packet)
{
    cout << "Error: " + packet->requestErrMsg << endl;
    // TODO: other
}

void AppClient::onResponseServerTip(PacketServerTip *packet)
{
#if COCOS2D_ENGINE
    CCDIRECTOR->getScheduler()->unscheduleSelector(schedule_selector(AppClient::resetServerTip), this);
    ccColor3B color = ccWHITE;
    std::list<ServerTip>::iterator it = packet->tips.begin();
    while (it != packet->tips.end()) {
        switch (it->tipType) {
            case kServerTipNormal:
                color = ccGREEN;
                break;
            case kServerTipWarnning:
                color = ccYELLOW;
                break;
            case kServerTipError:
                color = ccRED;
                break;
                
            default:
                color = ccWHITE;
                break;
        }
        CCLabelTTF::flyTip(CCMAINLAYER, it->message.c_str(), color, CCMAINLAYER->getCenter(), 20, serverTipDelay);
        serverTipDelay += 0.5;
        packet->tips.pop_front();
        it = packet->tips.begin();
    }
    CCDIRECTOR->getScheduler()->scheduleSelector(schedule_selector(AppClient::resetServerTip), this, serverTipDelay, false);
#else
    std::list<ServerTip>::iterator it = packet->tips.begin();
    while (it != packet->tips.end()) {
        cout << "ServerTip: [" << it->tipType << "] " << it->message << endl;
        packet->tips.pop_front();
        it = packet->tips.begin();
    }
#endif
}
#if COCOS2D_ENGINE
void AppClient::resetServerTip(float time)
{
    serverTipDelay = 0.5;
    CCDIRECTOR->getScheduler()->unscheduleSelector(schedule_selector(AppClient::resetServerTip), this);
}
#endif

void AppClient::onTestResponse(PacketTest *packet)
{
    
}
