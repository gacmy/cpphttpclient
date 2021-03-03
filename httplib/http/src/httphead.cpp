#include "httphead.h"
#include "./utils/util.h"

void HttpHead::add(string key, string value){
  mHead.push_back(key);
  mHead.push_back(value);
}

string HttpHead::get(const string &key){
  for(int i = 0; i < mHead.size()/2;i++){
    if(mHead[2*i] == key){
      return mHead[2*i+1];
    }

  }
  return NULL;
}

void HttpHead::add(const string& content){
  vector<string> res;
  string delim = ":";
  HttpUtil::spilt(content, delim, 0, content.size(), res); 
  if(res.size() == 2){
    add(res[0], res[1]);
  }
}

HttpHead& HttpHead::operator=(const HttpHead& head){
  mHead = head.mHead;
  return *this;
}
void HttpHead::clear(){
  mHead.clear();
}
