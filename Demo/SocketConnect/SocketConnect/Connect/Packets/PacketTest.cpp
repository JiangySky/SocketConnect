//
//  PacketTest.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketTest.h"
#include "../AppConnect.h"

int PacketTest::write(SocketOutputStream * oStream)
{
    int writeSize = oStream->getTail();
    // NOTE: write data to stream
    oStream->writeInt(testId);
    oStream->writeString(msg);
    
    return oStream->getTail() - writeSize;
}

int PacketTest::read(SocketInputStream * iStream)
{
    int readSize = iStream->getHead();
    // NOTE: read data from stream

    return iStream->getHead() - readSize;
}

int PacketTest::execute()
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
