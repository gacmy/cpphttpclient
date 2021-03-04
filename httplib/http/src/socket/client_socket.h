#ifndef G_CLIENT_SOCKET_H
#define G_CLIENT_SOCKET_H
#include "socket.h"
class ClientSocket:public Socket{

  public:

    ClientSocket(int port,const string& host);

    ~ClientSocket();
    

    virtual int _connect();


    virtual int _close();


};

#endif
