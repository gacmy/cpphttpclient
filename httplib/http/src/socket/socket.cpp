#include "socket.h"
#include <stdio.h>
#include <string.h>

Socket::Socket(const string& host,int port):m_port(port),m_host(NULL),m_fd(socket(AF_INET, SOCK_STREAM, 0)),
  m_input(NULL),m_output(NULL),m_addr(NULL){

    m_host = new char[strlen(host.c_str()) + 1];
    strcpy(m_host, host.c_str());
    //printf("m_host:%s\n",m_host);
    //only for ipv4? should encapsulate
    m_addr = new sockaddr_in; 
    memset(m_addr, 0, sizeof(*m_addr)); 
    m_addr->sin_family = AF_INET;
    m_addr->sin_port = htons(m_port);

  }

Socket::~Socket(){
  delete m_host;
  m_host = NULL;
  delete m_addr;
  m_addr = NULL;
  delete m_input;
  m_input = NULL;
  delete m_output;
  m_output = NULL;
}




