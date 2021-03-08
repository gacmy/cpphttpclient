#ifndef G_SSL_SCOKET_H
#define G_SSL_SCOKET_H
#include "socket.h"
#include <openssl/err.h>
#include <openssl/ssl.h>


class SSLSocket:public Socket{
  public:
    SSLSocket(const string& host,int port);

    int ssl_connect();
    virtual ~SSLSocket();


    virtual int _connect();

    virtual int _close();

  private:
    SSL *m_ssl;
    SSL_CTX *m_ctx;
};
#endif
