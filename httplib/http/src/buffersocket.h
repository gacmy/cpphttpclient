#ifndef G_BUFFERSOCKET_H
#define G_BUFFERSOCKET_H
#include <string>
#include "httpurl.h"
#include <sys/socket.h>
#include <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>       /* inet(3) functions */
#include "./cppokio/buffer.h"


using namespace std;

class BufferSocket{
  public:
    BufferSocket(HttpUrl& url);

    ~BufferSocket();

    BufferSocket(const BufferSocket& buf) = delete;

    BufferSocket& operator=(const BufferSocket& buf) = delete;

    int prepare();

    int readLine(string& line);

    int readLine(Buffer& buffer);

    int readbyte(Buffer& buffer);

    int _read(Buffer& buffer,const int& byteCount);

    int writeBuffer(Buffer& buffer);

    int writeChunked(Buffer& buffer);


  private:

    struct sockaddr_in servaddr;

    int mFd;

    HttpUrl mUrl;

    int readbyte(char& ch);

    int _connect(HttpUrl& url);

    void _close();

};

#endif
