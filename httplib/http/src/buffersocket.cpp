#include "buffersocket.h"
#include "dns.h"
#include <string.h>
#include <unistd.h>

BufferSocket::BufferSocket(HttpUrl& url):mUrl(url){

  mFd = socket(AF_INET, SOCK_STREAM, 0);
  if(mFd <= 0){
    printf("create socket failed\n");    
    return;
  }

  //jbzero(&servaddr, sizeof(servaddr));
  memset(&servaddr, 0, sizeof(servaddr)); 
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(mUrl.getPort());
}

int BufferSocket::_connect(HttpUrl& url){
  if(mFd <= 0){
    return -1;
  }
  string ip;
  if(!url.isIP()){
    Dns dns(url.getHost());
    Dns::Result result;
    result.print();

    dns.getIpByName(&result);
    //result.print();
    if(0 == result.ips.size()){
      return -1;
    }
    ip = result.ips[0];
    printf("parsed ip:%s \n",result.ips[0].c_str());
  }else{
    ip = url.getHost().c_str();
  }
  inet_pton(AF_INET,ip.c_str(), &servaddr.sin_addr);
  if (connect(mFd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
    printf("connect error");
    return -1;
  }
  return 0;
}

void BufferSocket::_close(){
  if(mFd > 0)
    close(mFd);
}

BufferSocket::~BufferSocket(){
  _close();
}

int BufferSocket::prepare(){
  return _connect(mUrl);
}

int BufferSocket::readLine(string &line){
  Buffer buf;
  int count  = readLine(buf);
  line = buf.readUtf8();
  return count;
}


int BufferSocket::readLine(Buffer& line){
  char ch;
  int n;
  int count;
  while((n = readbyte(ch)) != -1){
    if(ch == '\r'){
      n = readbyte(ch);
      if(n == -1 || ch == '\n'){
        return count;
      }
    }
    count++;
    line.writeByte(ch);
  }
  return count;
}

int BufferSocket::_read(Buffer& buffer,const int& byteCount){
  char ch[byteCount]; 
  int len = read(mFd, ch, byteCount);
  if(len > 0)
    buffer.write((byte*)ch, len);
  return len;
}

int BufferSocket::readbyte(char& ch){
  return read(mFd, &ch, 1);
}

int BufferSocket::writeBuffer(Buffer &buffer){
  string reqStr = buffer.readUtf8();
  printf("request str:\n%s \n",reqStr.c_str());
  return write(mFd, reqStr.c_str(),reqStr.length());
}

int BufferSocket::writeChunked(Buffer &buffer){
  Buffer temp; 
  int len = readLine(temp); 
  //while(true){

  //buffer.write(temp, temp.getSize());   
  // }
  return -1;
}

