#ifndef G_SOCKET_H
#define G_SOCKET_H
//#include <sys/socket.h>
#include <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include <string>
#include "inputstream.h"
#include "outputstream.h"

//#include <arpa/inet.h>       /* inet(3) functions */
using namespace std;
class Socket{
  public:
    Socket(const string& host,int port);

    virtual ~Socket();

    bool isConnect(){
      return m_bconnect;
    }

    virtual int _connect() = 0;

    bool isClose(){
      return m_bclose;
    }

    virtual int _close() = 0;

    InputStream* getInputStream(){
      return m_input;
    }

    OutputStream* getOutputStream(){
      return m_output;
    }

  protected:
    int m_fd;
    sockaddr_in* m_addr;
    bool m_bclose;
    bool m_bconnect;
    char* m_host;
    int m_port;
    InputStream* m_input;
    OutputStream* m_output;
};

#endif
