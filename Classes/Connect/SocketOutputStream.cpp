//
//  SocketOutputStream.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "SocketOutputStream.h"
#include "Packet.h"

bool SocketOutputStream::init()
{
    om_BufferLen  = DEFAULTINPUTBUFFERSIZE;
    om_MaxBufferLen = DISCONNECTINPUTSIZE;
    om_Head = 0;
    om_Tail = 0;
    isCloseKey = false;
    om_buffer = (char *)malloc(om_BufferLen * sizeof(char));
    memset(om_buffer, 0, om_BufferLen * sizeof(char));
    
    return true;
}

void SocketOutputStream::outputSocket(int socket)
{
    om_pSocket = socket;
}

uint SocketOutputStream::write(const char * buf, uint len)
{
    if (om_Head <= om_Tail) {
        if (om_Head == 0) {
            memcpy(&om_buffer[om_Tail], buf, len);
        } else {
            uint rightLen = om_BufferLen - om_Tail;
            if (rightLen >= len) {
                memcpy(&om_buffer[om_Tail], buf, len);
            } else {
                memcpy(&om_buffer[om_Tail], buf, rightLen);
                memcpy(&om_buffer[0], &buf[rightLen], len - rightLen);
            }
        }
        
    } else {
        memcpy(&om_buffer[om_Tail],buf,len);
    }
    
    om_Tail = (om_Tail+len)%om_BufferLen;
    return len;
}

uint SocketOutputStream::write(Packet * packet)
{
    ushort packetSize = 0;
    ushort packetId = 0;
    ushort errorId = 0;
    ushort encrypt = 0;
    ushort version = 0;
    ushort reserved = 0;
    
    packetSize = packet->getPacketSize() + PACKET_HEADER_SIZE;
    packetId = packet->packetId;
    version = PACKET_VERSION;
    
    this->writeUShort(packetSize);
    this->writeUShort(packetId);
    this->writeUShort(errorId);
    this->writeUShort(encrypt);
    this->writeUShort(version);
    this->writeUShort(reserved);
#ifdef PACKET_WITH_JSON
    int writeSize = packet->writeJSON(this);
#else
    int writeSize = packet->write(this);
#endif
    if (packetSize != writeSize + PACKET_HEADER_SIZE) {
        packetSize = writeSize + PACKET_HEADER_SIZE;
        om_Tail -= packetSize;
        this->writeShort(packetSize);
        om_Tail += packetSize - sizeof(ushort);
    }
    
    if (writeSize >= 0) {
        isCloseKey = true;
        std::cout << "Send message " << packetId << std::endl;
    } else {
        std::cout << "Failed to send message " << packetId << std::endl;
    }
    return packetSize;
}

uint SocketOutputStream::flush()
{
    uint nLeft = 0;
    ssize_t nsendlen = 0;
    
    if (om_Head < om_Tail) {
        nLeft = om_Tail - om_Head;
        while (nLeft > 0) {
            if (om_pSocket < 0) {
                return -1;
            }
            nsendlen = send(om_pSocket, (const char *)om_buffer, nLeft, 0);
            if (nsendlen <= 0) {
                this->cleanUp();
                return 0;
            }
            nLeft -= nsendlen;
            om_Head = (om_Head+nsendlen)%om_BufferLen;
        }
        
    } else {
        nLeft = om_BufferLen - om_Head;
        while (nLeft > 0) {
            if (om_pSocket < 0) {
                return -1;
            }
            nsendlen = send(om_pSocket, (const char *)om_buffer[om_Head], nLeft, 0);
            if (nsendlen <= 0) {
                this->cleanUp();
                return 0;
            }
            nLeft -= nsendlen;
            om_Head = (om_Head+nsendlen) % om_BufferLen;
        }
        nLeft = om_Tail;
        while (nLeft > 0) {
            nsendlen = send(om_pSocket, (const char *)om_buffer[om_Head], nLeft, 0);
            if (nsendlen <= 0) {
                this->cleanUp();
                return 0;
            }
            nLeft -= nsendlen;
            om_Head = (om_Head+nsendlen) % om_BufferLen;
        }
    }
    this->cleanUp();
    isCloseKey = false;
    return (uint)nsendlen;
}

char * SocketOutputStream::getOutBuffer()
{
    return om_buffer;
}

bool SocketOutputStream::isEmpty()
{
    if (om_Head == om_Tail) {
        return true;
    }
    return false;
}

void SocketOutputStream::cleanUp()
{
    om_BufferLen = DEFAULTINPUTBUFFERSIZE;
    om_MaxBufferLen = DISCONNECTINPUTSIZE;
    om_Head = 0;
    om_Tail = 0;
    isCloseKey = false;
    if (om_buffer) {
        free(om_buffer);
    }
    om_buffer = (char *)malloc(om_BufferLen * sizeof(char));
    memset(om_buffer, 0, om_BufferLen * sizeof(char));
}

uint SocketOutputStream::size()
{
    if (om_Head <= om_Tail) {
        return om_Tail - om_Head;
    } else {
        return om_BufferLen - om_Head + om_Tail;
    }
}

uint SocketOutputStream::getHead()
{
    return om_Head;
}

uint SocketOutputStream::getTail()
{
    return om_Tail;
}

uint SocketOutputStream::getBufferLength()
{
    return om_BufferLen;
}

char * SocketOutputStream::getBuffer()
{
    return om_buffer;
}

bool SocketOutputStream::encrypt()
{
    // NOTE encrypt
    return true;
}

bool SocketOutputStream::encrypt(char * buf, uint len, uint keyIndex)
{
    // NOTE encrypt
    return true;
}

#pragma mark -

void SocketOutputStream::writeLong(long data)
{
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    data = _OSSwapInt32(data);
#endif
    this->write((const char *)(&data), sizeof(long));
}

void SocketOutputStream::writeLonglong(int64_t data)
{
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    data = _OSSwapInt64(data);
#endif
    this->write((const char *)(&data), sizeof(int64_t));
}

void SocketOutputStream::writeInt(int data)
{
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    data = _OSSwapInt32(data);
#endif
    this->write((const char *)(&data), sizeof(int));
}

void SocketOutputStream::writeUInt(uint data)
{
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    data = _OSSwapInt32(data);
#endif
    this->write((const char *)(&data), sizeof(uint));
}

void SocketOutputStream::writeShort(short data)
{
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    data = _OSSwapInt16(data);
#endif
    this->write((const char *)(&data), sizeof(short));
}

void SocketOutputStream::writeUShort(ushort data)
{
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    data = _OSSwapInt16(data);
#endif
    this->write((const char *)(&data), sizeof(ushort));
}

void SocketOutputStream::writeFloat(float data)
{
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    data = _OSSwapInt32(data);
#endif
    this->write((const char *)(&data), sizeof(float));
}

void SocketOutputStream::writeDouble(double data)
{
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    data = _OSSwapInt64(data);
#endif
    this->write((const char *)(&data), sizeof(double));
}

void SocketOutputStream::writeByte(int data)
{
    this->write((const char *)(&data), sizeof(char));
}

void SocketOutputStream::writeString(std::string data)
{
    ushort length = (ushort)data.length();
    this->writeUShort(length + 1);
    if (length > 0) {
        char ucData[length];
        strcpy(ucData, data.c_str());
        this->write(ucData, length);
    }
    char temChar = '\0';
	this->writeByte(temChar);
}

void SocketOutputStream::writeData(std::string data)
{
    ushort length = (ushort)data.length();
    char ucData[length];
    strcpy(ucData, data.c_str());
    this->write(ucData, length);
}

