#include "ssl_socket.h"
#include <sys/socket.h>
#include <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h> 
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "real_inputstream.h"
#include "real_outputstream.h"
SSLSocket::SSLSocket(const string& host,int port):Socket(host,port),m_ssl(NULL),m_ctx(NULL){
  printf("m_host:%s port:%d fd:%d\n",m_host,m_port,m_fd);
}

SSLSocket::~SSLSocket(){
  if(!m_bclose)
    _close();
}

int SSLSocket::ssl_connect(){
  if(m_bconnect){
    return 0;
  }
  if(m_fd <= 0){
    m_bconnect = false;
    return -1;
  }

  //添加SSL的加密/HASH算法
  SSLeay_add_ssl_algorithms();
  //客户端，服务端选择SSLv23_server_method()
  const SSL_METHOD *meth = SSLv23_client_method();
  //建立新的SSL上下文
  m_ctx = SSL_CTX_new(meth);
  if (m_ctx == nullptr) {
    printf("SSL_CTX_new error !\n");
    return -1;
  }
  inet_pton(AF_INET,m_host,&m_addr->sin_addr);
  int ret = 0; 
  if((ret = connect(m_fd, (struct sockaddr *) m_addr, sizeof(*m_addr))) != 0) {
    m_bconnect = false;
    printf("connect error:%s\n",strerror(ret));
    return ret;
  }

  //建立SSL
  m_ssl = SSL_new(m_ctx);
  if (m_ssl == nullptr) {
    printf("SSL NEW error");
    return -1;
  }
  //将SSL与TCP SOCKET 连接
  SSL_set_fd(m_ssl, m_fd);
  // SSL连接
  ret = SSL_connect(m_ssl);
  if (ret == -1) {
    printf("SSL ACCEPT error ");
    return -1;
  }
  m_bconnect = true;
  m_input = new RealInputStream(m_fd);
  m_output = new RealOutputStream(m_fd);
  return 0;
}

int SSLSocket::_connect(){
  return 0;

}

int SSLSocket::_close(){
   m_bclose = true;
  //关闭SSL套接字
  SSL_shutdown(m_ssl);
 //释放SSL套接字
  SSL_free(m_ssl);
  //释放SSL会话环境
  SSL_CTX_free(m_ctx);
  return close(m_fd);
}
