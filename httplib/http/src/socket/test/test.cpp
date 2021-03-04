#include "../client_socket.h"

int main(){
  ClientSocket* clientsocket = new ClientSocket(5000,"127.0.0.1");
  int ret = clientsocket->_connect();
  printf("connect ret %d\n",ret);
  int len = 0;
  char chs[1024];
  if(clientsocket->isConnect()){
    InputStream* input = clientsocket->getInputStream();
    while(1){
     len = input->_read(chs, 1024);
     printf("read n = %d content=%s\n",len,chs);
    } 
  }else{

  }
}
