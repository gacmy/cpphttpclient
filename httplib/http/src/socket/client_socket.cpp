#include "client_socket.h"
#include <sys/socket.h>
#include <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h> 
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "real_inputstream.h"
#include "real_outputstream.h"
ClientSocket::ClientSocket(int port,const string& host):Socket(host,port){
     
    printf("m_host:%s port:%d fd:%d\n",m_host,m_port,m_fd);
}

ClientSocket::~ClientSocket(){
  if(!m_bclose)
      _close();
}

int ClientSocket::_connect(){
   if(m_bconnect){
      return 0;
   }
   if(m_fd <= 0){
    m_bconnect = false;
    return -1;
  }
  inet_pton(AF_INET,m_host,&m_addr->sin_addr);
  int ret = 0; 
  if((ret = connect(m_fd, (struct sockaddr *) m_addr, sizeof(*m_addr))) != 0) {
    m_bconnect = false;
    printf("connect error:%s\n",strerror(ret));
    return ret;
  }
  m_bconnect = true;
  m_input = new RealInputStream(m_fd);
  m_output = new RealOutputStream(m_fd);
  return 0;

}

int ClientSocket::_close(){
  m_bclose = true;
  return close(m_fd);
}
