//
//  PacketManager.h
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__PacketManager__
#define __NewProject__PacketManager__

#include <iostream>
#include "ConnectDefine.h"
#include "Packet.h"

class PacketManager {
    
public:
    CLASS_INSTANCE(PacketManager)
    
    Packet * packet;
    
    virtual ~PacketManager();
    bool init();
    
    /**
     @brief     get packet instance with id
     @param     packetId : uint
     @return    packet instance
     */
    Packet * getPacket(uint packetId);
};

#endif /* defined(__NewProject__PacketManager__) */
