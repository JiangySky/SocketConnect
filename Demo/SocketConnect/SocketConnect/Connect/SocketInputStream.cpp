//
//  SocketInputStream.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "SocketInputStream.h"
#include "Packet.h"

bool SocketInputStream::init()
{
    im_BufferLen  = DEFAULTINPUTBUFFERSIZE;
    im_MaxBufferLen = DISCONNECTINPUTSIZE;
    im_Head = 0;
    im_Tail = 0;
    im_buffer = (char *)malloc(im_BufferLen * sizeof(char));
    memset(im_buffer, 0, im_BufferLen * sizeof(char));
    
    return true;
}

void SocketInputStream::inputSocket(int socket)
{
    im_pSocket = socket;
}

uint SocketInputStream::read(char * buf, uint len)
{
    if (len == 0) {
        return 0;
    }
    if (len > im_BufferLen) {
        return 0;
    }
    if (im_Head < im_Tail) {
        memcpy(buf, &im_buffer[im_Head],len);
    } else {
        uint rightLen = im_BufferLen - im_Head;
        if (rightLen >= len) {
            memcpy(buf, &im_buffer[im_Head], len);
        } else {
            memcpy(buf, &im_buffer[im_Head], rightLen);
            memcpy(&buf[rightLen], &im_buffer[0], len - rightLen);
        }
    }
    im_Head = (im_Head +len ) % im_BufferLen;
    return len;
}

int SocketInputStream::read(Packet * p)
{
    if (!this->skip(PACKET_HEADER_SIZE)) {
        return -1;
    }
#ifdef PACKET_WITH_JSON
    return p->readJSON(this);
#else
    return p->read(this);
#endif
}

bool SocketInputStream::peek(char * buf, uint len)
{
    if (len == 0) {
        return false;
    }
    if (len > this->size()) {
        return false;
    }
    if (im_Head < im_Tail) {
        memcpy(buf, &im_buffer[im_Head], len);
    } else {
        uint rightLen = im_BufferLen - im_Head;
        if (rightLen >= len) {
            memcpy(buf, &im_buffer[im_Head], len);
        } else {
            memcpy(buf, &im_buffer[im_Head], rightLen);
            memcpy(&im_buffer[rightLen], &im_buffer[0], len - rightLen);
        }
    }
    return true;
}

bool SocketInputStream::find(char * buf)
{
    return false;
}

bool SocketInputStream::skip(uint len)
{
    if (len == 0) {
        return false;
    }
    if (len > this->size()) {
        return false;
    }
    im_Head = (im_Head + len) % im_BufferLen;
    return true;
}

uint SocketInputStream::fill()
{
    return 0;
}

uint SocketInputStream::fill(const char * pmsg, const uint len)
{
    uint nFree = im_BufferLen - this->size() -1;
    if (len <= nFree) {
        if (im_Head <=  im_Tail) {
            if (im_Head == 0) { // head
                memcpy(&im_buffer[im_Tail], pmsg, len);
                im_Tail += len;
            } else { // need to split
                uint nRightFree = im_BufferLen - im_Tail - 1; // space to tail
                if (len <= nRightFree) { // stored directly
                    memcpy(&im_buffer[im_Tail], pmsg, len);
                    im_Tail +=len;
                } else { // split
                    memcpy(&im_buffer[im_Tail], pmsg, nRightFree); // tail
                    memcpy(&im_buffer[0], &pmsg, len); // head
                    im_Tail = (im_Tail + len) % im_BufferLen;
                }
            }
            
        } else {
            memcpy(&im_buffer[im_Tail], pmsg, len);
            im_Tail += len;
        }
        
    } else { // out of space
        if (!this->resize(len - nFree + 10240)) {
            return 0;
        }
        memcpy(&im_buffer[im_Tail], pmsg, len);
        im_Tail += len;
    }
    return len;
}

void SocketInputStream::initSize()
{
    im_Head = 0;
    im_Tail = 0;
    free(im_buffer);
    im_BufferLen = DEFAULTINPUTBUFFERSIZE;
    im_buffer = (char *)malloc(im_BufferLen * sizeof(char));
    memset(im_buffer, 0, im_BufferLen * sizeof(char));
}

bool SocketInputStream::resize(int size)
{
    size = fmax(size, (int)(im_BufferLen >> 1)); // At least increase half of im_BufferLen
    int newBufferLen = im_BufferLen + size;
    uint len = this->size();
    if (size < 0) {
        if (newBufferLen < 0 || newBufferLen < len) {
            return false;
        }
    }
    char * newBuffer = (char *)malloc(newBufferLen * sizeof(char));
    if (newBuffer == NULL) { // failed to malloc
        return false;
    }
    memset(newBuffer, 0, newBufferLen*sizeof(char));
    if (im_Head <=im_Tail) {
        memcpy(newBuffer, &im_buffer[im_Head], len);
    }else{
        memcpy(newBuffer, &im_buffer[im_Head], im_BufferLen - im_Head);
        memcpy(&newBuffer[im_BufferLen - im_Head], &im_buffer[0], im_Tail);
    }
    free(im_buffer);
    im_buffer = newBuffer;
    im_BufferLen = newBufferLen;
    im_Head = 0;
    im_Tail = len;
    return true;
}

uint SocketInputStream::capacity()
{
    return im_BufferLen;
}

uint SocketInputStream::size()
{
    if (im_Head <= im_Tail) {
        return im_Tail - im_Head;
    } else {
        return im_BufferLen - im_Head + im_Tail;
    }
}

bool SocketInputStream::isEmpty()
{
    if(im_Head == im_Tail){
        return true;
    }
    return false;
}

void SocketInputStream::cleanUp()
{
    im_BufferLen = DEFAULTINPUTBUFFERSIZE;
    im_MaxBufferLen = DISCONNECTINPUTSIZE;
    im_Head = 0;
    im_Tail = 0;
    if (im_buffer) {
        free(im_buffer);
    }
    im_buffer = (char *)malloc(im_BufferLen * sizeof(char));
    memset(im_buffer, 0, im_BufferLen * sizeof(char));
}

uint SocketInputStream::getHead(int head)
{
    if (im_Head < head) {
        return im_BufferLen + im_Head;
    } else {
        return im_Head;
    }
}

uint SocketInputStream::getTail(int tail)
{
    if (im_Tail < tail) {
        return im_BufferLen + im_Tail;
    } else {
        return im_Tail;
    }
}

uint SocketInputStream::getBufferLength()
{
    return im_BufferLen;
}

char * SocketInputStream::getBuffer()
{
    return im_buffer;
}

uint SocketInputStream::receive()
{
    int nRecv = 0;
    uint nFree = 0;
    uint nFilled = 0;
    if (im_Head <= im_Tail) {
        if (im_Head == 0) {
            nFree = im_BufferLen - im_Tail - 1;
            if (im_pSocket < 0) {
                return -1;
            }
            nRecv = recv(im_pSocket, &im_buffer[im_Tail], nFree, 0);
            if (nRecv <= 0) {
                return 0;
            }
            im_Tail += nRecv;
            nFilled += nRecv;
            
        } else {
            nFree = im_BufferLen - im_Tail;
            nRecv = recv(im_pSocket, &im_buffer[im_Tail], nFree, 0);
            if (nRecv <= 0) {
                return 0;
            }
            im_Tail = (im_Tail + nRecv) % im_BufferLen;
            nFilled += nRecv;
        }
        
    } else {
        nFree = im_Head - im_Tail -1;
        if (im_pSocket < 0) {
            return -1;
        }
        nRecv = recv(im_pSocket, &im_buffer[im_Tail], nFree, 0);
        if (nRecv <= 0) {
            return 0;
        }
        im_Tail += nRecv;
        nFilled += nRecv;
    }
    return nFilled;
}

bool SocketInputStream::decrypt()
{
    // NOTE: decrypt
    return true;
}

bool SocketInputStream::decrypt(char * buf, uint len, uint keyIndex)
{
    // NOTE: decrypt
    return true;
}

void SocketInputStream::logInfo()
{
    std::cout << "im_Head = " << im_Head << ", im_Tail = " << im_Tail << std::endl;
}

// MARK: -

long SocketInputStream::readLong()
{
    long result = 0;
    this->read((char *)(&result), sizeof(long));
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    return _OSSwapInt32(result);
#else
    return result;
#endif
}

int64_t SocketInputStream::readLonglong()
{
    int64_t result = 0;
    this->read((char *)(&result), sizeof(int64_t));
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    return _OSSwapInt64(result);
#else
    return result;
#endif
}

int SocketInputStream::readInt()
{
    int result = 0;
    this->read((char *)(&result), sizeof(int));
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    return _OSSwapInt32(result);
#else
    return result;
#endif
}

uint SocketInputStream::readUInt()
{
    uint result = 0;
    this->read((char *)(&result), sizeof(uint));
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    return _OSSwapInt32(result);
#else
    return result;
#endif
}

short SocketInputStream::readShort()
{
    short result = 0;
    this->read((char *)(&result), sizeof(short));
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    return _OSSwapInt16(result);
#else
    return result;
#endif
}

ushort SocketInputStream::readUShort()
{
    ushort result = 0;
    this->read((char *)(&result), sizeof(ushort));
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    return _OSSwapInt16(result);
#else
    return result;
#endif
}

float SocketInputStream::readFloat()
{
    float result = 0;
    this->read((char *)(&result), sizeof(float));
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    return _OSSwapInt32(result);
#else
    return result;
#endif
}

double SocketInputStream::readDouble()
{
    double result = 0;
    this->read((char *)(&result), sizeof(double));
#if CLIENT_BYTE_ORDER != SERVER_BYTE_ORDER
    return _OSSwapInt64(result);
#else
    return result;
#endif
}

int SocketInputStream::readByte()
{
    char result;
    this->read((char *)(&result), sizeof(char));
    return result;
}

std::string SocketInputStream::readString()
{
    ushort length = this->readUShort();
    if (length == 0) {
        return "";
    }
    char result[length];
    memset(result, 0, length);
    this->read(result, length);
    return result;
}

std::string SocketInputStream::readData(uint length)
{
    char result[length];
    this->read(result, length);
    return result;
}
