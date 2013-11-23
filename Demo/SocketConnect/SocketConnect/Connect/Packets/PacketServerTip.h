//
//  PacketServerTip.h
//  Hero
//
//  Created by Jiangy on 13-11-23.
//  Copyright (c) 2013 35VI. All rights reserved.
//

/**
 * #include "PacketServerTip.h" in PacketAll.h
 * define PACKET_* in _RequestType of ConnectDefine.h
 * return INSTANCE(PacketServerTip) at case in PacketManager::getPacket(uint packetId)
 */

#ifndef __Hero__PacketServerTip__
#define __Hero__PacketServerTip__

#include <iostream>
#include <list>
#include "../Packet.h"

enum E_ServerTip {
    kServerTipNone,
    kServerTipNormal,
    kServerTipWarnning,
    kServerTipError,
};
struct S_ServerTip {
    int         tipType;
    std::string message;
};
typedef S_ServerTip ServerTip;

class PacketServerTip : public Packet {
    
public:
    CLASS_INSTANCE(PacketServerTip)
    
    // NOTE: declare variable
    std::list<ServerTip> tips;
    
    virtual int write(SocketOutputStream * oStream);
    virtual int read(SocketInputStream * iStream);
    virtual int execute();
    
};

#endif /* defined(__Hero__PacketServerTip__) */