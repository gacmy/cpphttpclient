#include "real_inputstream.h"
#include <unistd.h>

#include <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
RealInputStream::RealInputStream(int fd):m_fd(fd){

}

RealInputStream::~RealInputStream(){
  _close();
}

int RealInputStream::_close(){
  return shutdown(m_fd,SHUT_RD);
}

int RealInputStream::readByte(char &ch){
  return read(m_fd, &ch, 1);
}

void RealInputStream::settimeout(int seconds){
  if(m_fd > 0){
    struct timeval      tv;
    /* set read timeout time */
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  }
}
