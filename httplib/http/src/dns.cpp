#include "dns.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Dns::Dns(const string& name):m_host(name){
}


void Dns::getIpByName(Result* res){
  if(res == NULL){
    return;
  }
  struct hostent* host = NULL;
  host = gethostbyname(m_host.c_str());
  if(host == NULL){
    printf("dns parse error\n");
    return;
  }
  res->isIPv6=!(host->h_addrtype==AF_INET);
  char** pptr = host->h_addr_list;
  /* 打印IP地址 */
  for(;*pptr != NULL;pptr++){
    char str[INET_ADDRSTRLEN];
    res->ips.push_back(inet_ntop(AF_INET,*pptr, str, sizeof(str)));
  }
  res->name = m_host;
}
/**
  int main(){
  struct hostent* ht1 = 0; 
  const char* name = "www.baidu.com";//"www.baidu.com";
  Dns dns(name);
  Dns::Result res;
  dns.getIpByName(&res);
  res.print();
  return 0;
  }
 **/

