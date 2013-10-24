//
//  PacketTest.h
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__PacketTest__
#define __NewProject__PacketTest__

#include <iostream>
#include "../Packet.h"

class PacketTest : public Packet {
    
public:
    CLASS_INSTANCE(PacketTest)
    
    // NOTE: declare variable
    int testId;
    std::string msg;
    
    virtual int write(SocketOutputStream * oStream);
    virtual int read(SocketInputStream * iStream);
    virtual int execute();
    
};

#endif /* defined(__NewProject__PacketTest__) */
