//
//  PacketHBeat.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketHBeat.h"
#include "../AppConnect.h"
#include "../ClientConnect.h"

bool PacketHBeat::init()
{
    savedTime = 0;
    return Packet::init();
}

#ifdef PACKET_WITH_JSON
int PacketHBeat::writeJSON(SocketOutputStream *oStream)
{
    if (savedTime == 0) {
        savedTime = time(0);
    } else {
        INSTANCE(ClientConnect)->netDelay = time(0) - savedTime;
    }
    return Packet::writeJSON(oStream);
}
#else
int PacketHBeat::write(SocketOutputStream * oStream)
{
    if (savedTime == 0) {
        savedTime = time(0);
    } else {
        INSTANCE(ClientConnect)->netDelay = time(0) - savedTime;
    }
    int writeSize = oStream->getTail();
    // NOTE: write data to stream
    oStream->writeInt(PACKET_HEARTBEAT);
    
    return oStream->getTail(writeSize) - writeSize;
}

uint PacketHBeat::getPacketSize()
{
    return sizeof(int);
}
#endif
int PacketHBeat::execute()
{
    INSTANCE(ClientConnect)->netDelay = 0;
    savedTime = 0;
    switch (requestResult) {
        case kResultSuccess: { // Success
            // NOTE: process data
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