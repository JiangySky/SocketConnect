//
//  PacketManager.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketManager.h"
#include "Packets/PacketAll.h"

PacketManager::~PacketManager()
{
    delete packet;
}

bool PacketManager::init()
{
    return true;
}

Packet * PacketManager::getPacket(uint packetId)
{
    switch (packetId) {
        case PACKET_TEST: // test
            packet = INSTANCE(PacketTest);
            break;
        case PACKET_HEARTBEAT: // heart beat
            packet = INSTANCE(PacketHBeat);
            break;
        case PACKET_ERROR: // error info
            packet = INSTANCE(PacketError);
            break;
            
        default:
            packet = INSTANCE(Packet);
            break;
    }
    
    return packet;
}
