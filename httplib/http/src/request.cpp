#include "request.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "buffersocket.h"
#include "./utils/util.h"


using namespace std;
const char* Request::method_str[] = {"POST","GET"};
/**
 *
 *GET /mix/76.html?name=kelvin&password=123456 HTTP/1.1
Host: www.fishbay.cn
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.  2924.87 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,;q=0.8
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8,en;q=0.6
 *
 */

std::string getRequestHead(){
  string head; 
  head.append("GET /breka/articles/9791664.html HTTP/1.1\r\n");
  head.append("Host: www.cnblogs.com\r\n");
  head.append("User-Agent: cpphttpclient\r\n");
  head.append("Accept: text/html\r\n");
  head.append("Accept-Encoding: gzip\r\n");
  head.append("Accept-Language: zh-CN\r\n");
  head.append("\r\n");
  return head;
}

Request::Request(string url,Method method){
  int ret = mUrl.parseUrl(url);
  if(ret == 0){
    mUrl.print();
  }
  mMethod = method;

}

Request::Request(const Request& request){
  int ret = mUrl.parseUrl(request.getUrl());
  if(ret == 0){
    mUrl.print();
  } 
  mMethod = request.mMethod;
  mHead.clear();
  mHead = request.mHead;

}

Request& Request::operator=(const Request& request){
  mUrl.parseUrl(request.getUrl());
  mMethod = request.mMethod;
  mHead.clear();
  mHead = request.mHead;
  return *this;
}


Request& Request::addHead(string key, string value){
  mHead.add(key, value);
  return *this;
}



string Request::getHead(string key){
  return mHead.get(key);
}


const string& Request::getUrl() const{
  return mUrl.getUrl();
}
void writeStateLine(Buffer& buffer){

}

void Request::createRequestBuffer(Buffer& buffer){
  buffer.writeUtf8(method_str[mMethod]); 
  buffer.writeUtf8(" ");
  string path = "";
  vector<string> paths = mUrl.getPath();
  for(int i = 0; i < paths.size();i++){
    if(paths[i] == "/"){
      path.append(paths[i]);
    }else{
      path.append(paths[i]);
      if(i != paths.size()-1)
        path.append("/");
    }

  }
  string querystr ="";
  mUrl.getQueryStr(querystr);

  //get param
  if(mMethod == Method::GET){
    if(querystr != ""){
      path.append("?").append(querystr);
    }
  }
  buffer.writeUtf8(path);
  buffer.writeUtf8(" ");
  buffer.writeUtf8("HTTP/1.1");
  buffer.writeUtf8("\r\n");

  vector<string> head = mHead.getHead();

  for(int i = 0; i < head.size()/2; i++){
    string h = "";
    h.append(head[2*i]);
    h.append(":");
    h.append(head[2*i+1]);
    h.append("\r\n");
    buffer.writeUtf8(h);
  }
  string host = "";
  host.append("Host:");
  host.append(mUrl.getHost());
  host.append("\r\n");
  buffer.writeUtf8(host);

  buffer.writeUtf8("\r\n");
  //post url-form param ,json param?
  if(mMethod == Method::POST){
    buffer.writeUtf8(querystr);
    string params = "";
    HttpUtil::genereateParamString(mParams, params);
    if(params != ""){
      if(querystr != "")
        buffer.writeUtf8("&");
      string endcodeparam = HttpUtil::urlencode(params);
      buffer.writeUtf8(endcodeparam);
    }
  }

}
int Request::parseCode(const string& str){
  vector<string> vec; 
  HttpUtil::spilt(str, " ", 0, str.size(), vec);
  mCode = -1;
  for(string& num:vec){
    if(HttpUtil::isNum(num)){
      mCode = atoi(num.c_str());
    }
  }
  return mCode;
}

void Request::request(){
  Buffer buffer;

  createRequestBuffer(buffer);

  BufferSocket socket(mUrl);

  socket.prepare();

  socket.writeBuffer(buffer);

  int n = 0;
  socket.readLine(buffer);
  parseCode(buffer.readUtf8());
  printf("response code:%d\n",mCode);
  while((n = socket.readLine(buffer)) != 0){
    string content = buffer.readUtf8();
    mResonseHead.add(content);
    //printf("%s\n",buffer.readUtf8().c_str());
  }
  for(string str:mResonseHead.getHead()){
    printf("head::%s\n",str.c_str());
  }
  string contentlen = mResonseHead.get(CONTENT_LENGTH);
  if(contentlen != ""){
    int len = atoi(contentlen.c_str());
    if(len == 0){
      printf("content len is zero finished\n");
      return;
    }
    printf("len:%d\n",len);
  }
  socket.readLine(buffer);
  contentlen = buffer.readUtf8();
  printf("contentLen:%s\n",contentlen.c_str());
  //chunked end flag
  int endFlag = 0;
  if(contentlen == "0"){
    endFlag = 2;
  }
  while(true){
    int temp =socket.readLine(buffer);
    temp = buffer.getSize();
    string str = buffer.readUtf8();
    printf("len=%d content=%s\n",temp,str.c_str());
    if(endFlag == 0 && temp == 1 && str == "0"){
      endFlag = 2;
    }else if(endFlag == 2 && temp == 0){
      printf("read finished\n");
      break;
    }else{
      endFlag = 0;
    }
  }
  /**
    int len = socket._read(buffer, 1024);
    for(int i = 0; i < len; i++){
    printf("%x ",buffer.readByte());
    if(i % 33 == 0)
    printf("\n");
    }  
   **/
  //socket.readLine()
  /** 
    printf("write finish buf size %ld\n",buffer.getSize());
    int len = socket.readLine(buffer); 

    printf("read size:%ld\n",buffer.getSize());
    printf("head end line %d\n",len);   

    printf("r %d n %d",'\r','\n');
    byte by1 = buffer.readByte();
    byte by2 = buffer.readByte();
    byte by3 = buffer.readByte(); 
    printf("by1 %x by2 %x by3 %x \n",by1,by2,by3); 

    socket.readLine(buffer);
    printf("%s\n",buffer.readUtf8().c_str());
   **/

}

Request& Request::addParam(const string& key,const string& value){
  if(key != "" && value != ""){
    mParams.push_back(key);
    mParams.push_back(value);
  } 
  return *this;
}


uint8_t* Request::get(){
  const char* method = method_str[mMethod];
  int port = mPort;      
  return NULL;
}
/**
  int main(){
  Request request;
  request.setMethod(Request::GET);
  request.setPort(80);
  request.setUrl("http://www.cnblogs.com/breka/articles/9791664.html");
  request.request();
  }
 **/
