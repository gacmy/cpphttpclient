#include "httpurl.h"
#include "./utils/util.h"
#include <stdlib.h>

void HttpUrl::print(){
  printf("scheme:%s\n",scheme.c_str());
  printf("host:%s\n",host.c_str());
  printf("port:%d\n",port);
  for(const string& p:path){
    printf(" path:%s ",p.c_str());
  }
  printf("\n");
  for(const string& q:query){
    printf(" query:%s ",q.c_str());
  }
  printf("\n");
}

int HttpUrl::parseUrl(const string& murl){
  this->url = murl;
  int start = HttpUtil::skipLeadingAsciiWhiteSpace(this->url);
  int end = HttpUtil::skipTrailingAsciiWhiteSpace(this->url);
  int ret;
  if(start > end){
    return -1;
  }

  if(parseScheme(url,start, end)){
    printf("parse scheme failed\n");
    return -1;
  }
  printf("scheme:%s\n",scheme.c_str());
  if(scheme == "https"){
    start+=5;
  }else{
    start+=4;
  }
  //printf("parse host:%d\n",start);

  ret = parseHost(url, start, end);  
  if(ret == -1){
    printf("parse host failed\n");
    return -1;
  }
  is_ip = HttpUtil::isIpv4(host);
  printf("parse endhost:%d\n",ret);

  ret = parsePort(url, ret);
  if(ret == -1){
    printf("parse port failed\n");
    return -1;
  }

  ret = parsePaths(url, ret);
  if(ret == -1){
    printf("parse paths failed\n");
    return -1;
  }
  if(ret == url.size()){
    return 0;
  }
  
  ret = parseQuery(url, ret);
  if(ret == -1){
    printf("parse query failed\n");
    return -1;
  }

  return 0;
}



int HttpUrl::parseScheme(string& url,int pos,int end){
  if(url.find("https",pos) == pos){
    scheme = "https";
    return 0;
  }
  if(url.find("http") == pos){
    scheme = "http";
    return 0;
  }
  return -1;
}



int HttpUrl::parsePaths(string& url,int pos){
  int endpath = url.find('?', pos);
  string delim = "/";
  //only rootpath
  if(pos == url.size()){
    path.push_back("/"); 
    return pos;
  }

  //no query
  //www.google.com/search&q=search
  //this link is parsed correct.
  //if do not have ? mark.<search&q=search> is viewd as path
  if(endpath == -1){
    endpath = url.size();
  }

  string segmentPath = url.substr(pos, endpath-pos); 
  
  path.push_back("/");
  HttpUtil::spilt(segmentPath,delim,0,segmentPath.size(), path);
  //printf("path=%s\n",segmentPath.c_str());
  //spilt segmentPath
  //printf("endpath:%d\n",endpath);
  return endpath;
}

int HttpUrl::parseHost(string& url,int pos,int end){
   //colon
   if(url.find_first_of(':') != pos){
      printf("parse host colon failed\n");
      return -1;
   }
   //slash
   if(url.find_first_of("//") != pos+1){
      printf("parse host double slash failed\n");
      return -1;
   }
   int start = pos+3;
     
   //have port or default port
   int endHost = url.find('/', start);
   printf("endHost%d\n",endHost);
   int endColon = url.find(':',start);
   if(endColon != -1){
      endHost = endHost < endColon ? endHost:endColon; 
   }
   //endHost = endHost < endColon ?endHost:endColon; 
   //printf("endHost%d\n",endHost);
   if(start >= endHost){
      return -1;
   }
   host = url.substr(start, endHost-start); 
   return endHost;
}

int HttpUrl::parseQuery(string& url,int pos){
    if(url[pos] != '?'){
      return -1;
    }
    string delim = "&";
    HttpUtil::spilt(url, delim, pos+1,url.size(), query);  
    return 0; 
}

int HttpUrl::parsePort(string& url,int pos){
  if(url.find(':',pos) == pos){
    int endPos = -1;
    endPos = url.find('/',pos); 
    //jprintf("endPos = %d\n",endPos);
    if(endPos <= pos || endPos == -1){
      return -1;
    }
    string port = url.substr(pos+1, endPos-pos);
    printf("port:%s\n",port.c_str());
    this->port = atoi(port.c_str()); 
    printf("port num:%d\n",this->port);
    if(!HttpUtil::isPortIllegal(this->port)){
      return -1;
    }
    return endPos;
  }else{
    if(scheme == "http"){
      port = 80;
    }else{
      port = 443;
    }
  }
  return pos;
}



