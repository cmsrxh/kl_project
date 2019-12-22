#ifndef NET_BUFFER_H
#define NET_BUFFER_H


class NetBuffer
{
public:
    NetBuffer();

private:
    unsigned char *mBuffer;
    unsigned long  mSize;
    unsigned long  mAllocSize;
};

#endif // NET_BUFFER_H
