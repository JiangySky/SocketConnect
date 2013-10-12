//
//  Packet.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "Packet.h"
#include "SocketOutputStream.h"
#include "SocketInputStream.h"
#include "AppConnect.h"

Packet::~Packet()
{
#ifdef PACKET_WITH_JSON
    delete jsonData;
#endif
}

bool Packet::init()
{
    requestResult = kResultSuccess;
#ifdef PACKET_WITH_JSON
    compression = 0;
    jsonData = new Json::Value();
#endif
    return true;
}

void Packet::packetData()
{
    // NOTE: overwrite during json
}

bool Packet::setData(std::string data, int compress, bool byServer)
{
#ifdef PACKET_WITH_JSON
    jsonData->clear();
    switch (compress) {
        case 0:
            Json::Reader().parse(data, *jsonData);
            break;
        case 1: {
            uLong length;
            u_char * finalData;
            if (byServer) {
                GZipUtils::gzdecompress((Byte *)data.c_str(), sizeof(data), finalData, &length);
            } else {
                GZipUtils::gzcompress((Byte *)data.c_str(), sizeof(data), finalData, &length);
            }
            Json::Reader().parse((char *)finalData, *jsonData);
            break;
        }
        default:
            break;
    }
    if (byServer) {
        requestResult = (* jsonData)[kKeyRequestResult].asInt();
        requestErrMsg = (* jsonData)[kKeyRequestErrMsg].asString();
    }
#endif
    return true;
}

int Packet::write(SocketOutputStream * oStream)
{
    int writeSize = oStream->getTail();
    // NOTE: overwrite during stream
    
    return oStream->getTail() - writeSize;
}

int Packet::writeJSON(SocketOutputStream * oStream)
{
    int writeSize = oStream->getTail();
#ifdef PACKET_WITH_JSON
    oStream->writeInt(abs(compression));
    oStream->writeData(jsonData->toFastString());
#endif
    return oStream->getTail() - writeSize;
}

ushort Packet::getPacketSize()
{
    // NOTE: overwrite and return actual size to avoid write back to stream
#ifdef PACKET_WITH_JSON
    return jsonData->toFastString().length() + sizeof(int);
#else
    return 0;
#endif
}

int Packet::read(SocketInputStream * iStream)
{
    int readSize = iStream->getHead();
    // NOTE: overwrite during stream
    
    return iStream->getHead() - readSize;
}

int Packet::readJSON(SocketInputStream * iStream)
{
    // NOTE: overwrite during JSON
    return 0;
}

int Packet::execute()
{
    switch (requestResult) {
        case kResultSuccess: { // Success
            // NOTE: process data
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
            dispatch_async(dispatch_get_main_queue(), ^{
                AppConnect::client()->onResponse(this);
            });
#elif COCOS2D_ENGINE
            AppConnect::client()->onResponse(this);
#else
            // other platform or engine
#endif
            return 1;
        }
        default: { // Failure or otherwise
            std::cout << "Response: " << requestResult << " (" + requestErrMsg + ")\n";
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
            dispatch_async(dispatch_get_main_queue(), ^{
                AppConnect::client()->onResponseError(this);
            });
#elif COCOS2D_ENGINE
            AppConnect::client()->onResponseError(this);
#else
            // other platform or engine
#endif
            return 0;
        }
    }
    return -1;
}
