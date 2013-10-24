//
//  PacketError.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-8.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketError.h"
#include "../AppConnect.h"

int PacketError::read(SocketInputStream * iStream)
{
    int readSize = iStream->getHead();
    // NOTE: read data from stream
    std::string err = iStream->readString();
    msgId = iStream->readInt();
    if (strcmp("", err.c_str()) != 0) {
        requestErrMsg = err;
    }
    return iStream->getHead(readSize) - readSize;
}

int PacketError::execute()
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

