//
//  SocketInputStream.h
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__SocketInputStream__
#define __NewProject__SocketInputStream__

#include <iostream>
#include "ConnectDefine.h"

class Packet;

class SocketInputStream {
    int             im_pSocket;
    
public:
    CLASS_INSTANCE(SocketInputStream)
    
    char *          im_buffer;
    uint            im_BufferLen;
    uint            im_MaxBufferLen;
    uint            im_Head;
    uint            im_Tail;
    
// MARK: -
        
    virtual bool init();
    virtual void inputSocket(int socket);
    virtual uint read(char * buf, uint len);
    virtual int read(Packet * p);
    virtual bool peek(char * buf, uint len);
    virtual bool find(char * buf);
    virtual bool skip(uint len);
    virtual uint fill();
    virtual uint fill(const char * pmsg, const uint len);
    virtual void initSize();
    virtual bool resize(int size);
    virtual uint capacity();
    virtual uint size();
    virtual bool isEmpty();
    virtual void cleanUp();
    virtual uint getHead(int head = 0);
    virtual uint getTail(int tail = 0);
    virtual uint getBufferLength();
    virtual char * getBuffer();
    virtual uint receive();
    virtual bool decrypt();
    virtual bool decrypt(char * buf, uint len, uint keyIndex);
    virtual void logInfo();
    
// MARK: -
    
    virtual long readLong();
    virtual int64_t readLonglong();
    virtual int readInt();
    virtual uint readUInt();
    virtual short readShort();
    virtual ushort readUShort();
    virtual float readFloat();
    virtual double readDouble();
    virtual int readByte();
    virtual std::string readString();
    virtual std::string readData(uint length);

};

#endif /* defined(__NewProject__SocketInputStream__) */
