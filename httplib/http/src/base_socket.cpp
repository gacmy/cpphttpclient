#include "base_socket.h"

BaseSocket::~BaseSocket(){
  _close();
}

int BaseSocket::prepare(){
 return _connect(m_url);
}

int BaseSocket::_connect(HttpUrl& url){
  return -1;
}

void BaseSocket::_close(){

}

int BaseSocket::readbyte(char& ch){
  return -1;
}

