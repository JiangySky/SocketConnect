//
//  SocketOutputStream.h
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__SocketOutputStream__
#define __NewProject__SocketOutputStream__

#include <iostream>
#include "ConnectDefine.h"

class Packet;

class SocketOutputStream {
    int             om_pSocket;
    
public:
    CLASS_INSTANCE(SocketOutputStream)
    
    char *          om_buffer;
    uint            om_BufferLen;
    uint            om_MaxBufferLen;
    uint            om_Head;
    uint            om_Tail;
    bool            isCloseKey;
    
// MARK: -
    
    virtual bool init();
    virtual void outputSocket(int socket);
    virtual uint write(const char * buf, uint len);
    virtual uint write(Packet * packet);
    virtual uint flush();
    virtual char * getOutBuffer();
    virtual bool isEmpty();
    virtual void cleanUp();
    virtual uint size();
    virtual uint getHead(int head = 0);
    virtual uint getTail(int tail = 0);
    virtual uint getBufferLength();
    virtual char * getBuffer();
    virtual bool encrypt();
    virtual bool encrypt(char * buf, uint len, uint keyIndex);
    
// MARK: -
    
    virtual void writeLong(long data);
    virtual void writeLonglong(int64_t data);
    virtual void writeInt(int data);
    virtual void writeUInt(uint data);
    virtual void writeShort(short data);
    virtual void writeUShort(ushort data);
    virtual void writeFloat(float data);
    virtual void writeDouble(double data);
    virtual void writeByte(int data);
    virtual void writeString(std::string data);
    virtual void writeData(std::string data);

};

#endif /* defined(__NewProject__SocketOutputStream__  */
