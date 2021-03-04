#include "./client_socket.h"
#include <unistd.h>
void testRead(){
  ClientSocket* clientsocket = new ClientSocket(5000,"127.0.0.1");
  int ret = clientsocket->_connect();
  printf("***connect ret %d\n",ret);
  int len = 0;
  char chs[1024];
  if(clientsocket->isConnect()){
    InputStream* input = clientsocket->getInputStream();
    input->settimeout(3);
    while(1){
      len = input->_read(chs, 1024);
      if(len > 0){
        chs[len] = '\0';
      }else{
        break;
      }
      printf("read n = %d content=%s\n",len,chs);
      //sleep(1);

    } 
  }else{

  }

}
int main(){
  testRead();
}
