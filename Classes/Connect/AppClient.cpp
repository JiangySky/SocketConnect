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
    CCMessageBox(packet->requestErrMsg.c_str(), "请求错误");
}

void AppClient::onTestResponse(PacketTest *packet)
{
    
}
