//
//  PacketServerTip.cpp
//  Hero
//
//  Created by Jiangy on 13-11-23.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketServerTip.h"
#include "../AppConnect.h"

int PacketServerTip::write(SocketOutputStream * oStream)
{
    int writeSize = oStream->getTail();
    // NOTE: write data to stream
    
    return oStream->getTail(writeSize) - writeSize;
}

int PacketServerTip::read(SocketInputStream * iStream)
{
    int readSize = iStream->getHead();
    // NOTE: read data from stream
    ServerTip tip;
    tip.tipType = iStream->readInt();
    tip.message = iStream->readString();
    tips.push_back(tip);
    
    return iStream->getHead(readSize) - readSize;
}

int PacketServerTip::execute()
{
    switch (requestResult) {
        case kResultSuccess: { // Success
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
        }
    }
    return -1;
}

