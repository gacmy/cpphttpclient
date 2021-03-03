#ifndef G_BASE_SOCKET_H
#define G_BASE_SOCKET_H
#include "./cppokio/buffer.h"
#include <string>
#include "httpurl.h"
using namespace std;
class BaseSocket{

public:
    BaseSocket(HttpUrl& url):m_url(url){};

    virtual ~BaseSocket();

    BaseSocket(const BaseSocket& buf) = delete;

    BaseSocket& operator=(const BaseSocket& buf) = delete;

    virtual int prepare();

    virtual int readLine(string& line) = 0;

    virtual int readLine(Buffer& buffer) = 0;

    virtual int readbyte(Buffer& buffer) = 0;

    virtual int _read(Buffer& buffer,const int& byteCount) = 0;

    virtual int writeBuffer(Buffer& buffer) = 0;

private:
    HttpUrl m_url;
    virtual int readbyte(char& ch);

    virtual int _connect(HttpUrl& url);

    virtual void _close();



};
#endif
