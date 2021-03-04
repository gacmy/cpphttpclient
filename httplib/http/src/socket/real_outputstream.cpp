#include "real_outputstream.h"
#include <unistd.h>
#include <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
RealOutputStream::RealOutputStream(int fd):m_fd(fd){}

RealOutputStream::~RealOutputStream(){
  _close();
}

int RealOutputStream::_close(){
  return shutdown(m_fd,SHUT_WR);
}

int RealOutputStream::_write(char &ch){
  return write(m_fd, &ch, 1);
}

void RealOutputStream::flush(){

}

void RealOutputStream::settimeout(int seconds){
  if(m_fd > 0){
    struct timeval      tv;
    /* set read timeout time */
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    setsockopt(m_fd, SOL_SOCKET,SO_SNDTIMEO, &tv, sizeof(tv));
  }
}
