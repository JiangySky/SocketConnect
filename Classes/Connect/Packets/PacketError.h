//
//  PacketError.h
//  NewProject
//
//  Created by Jiangy on 13-1-8.
//  Copyright (c) 2013 35VI. All rights reserved.
//

/**
 * #include "PacketError.h" in PacketAll.h
 * return INSTANCE(PacketError) at case in PacketManager::getPacket(uint packetId)
 */

#ifndef __NewProject__PacketError__
#define __NewProject__PacketError__

#include <iostream>
#include "../Packet.h"

class PacketError : public Packet {
    
public:
    CLASS_INSTANCE(PacketError)
    
    // NOTE: declare variable
    
    virtual int read(SocketInputStream * iStream);
    virtual int execute();
    
};

#endif /* defined(__NewProject__PacketError__) */