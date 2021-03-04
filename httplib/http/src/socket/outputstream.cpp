#include "outputstream.h"

OutputStream::OutputStream(){}

OutputStream::~OutputStream(){
  _close();
};

int OutputStream::_write(char& ch){
  return -1;
}
int OutputStream::_write(char* chs,int len){
  return this->_write(chs,0,len);
}


int OutputStream::_write(char* chs, int off, int len){
  int count = 0;
  int ret = -1;
  for(int i = 0; i < len; ++i) {
    ret = this->_write(chs[off + i]);
    if(ret == -1 || ret == 0){
      break;
    }
    count+=ret;
  } 
  return count;
} 

void OutputStream::flush(){
  
}

int OutputStream::_close(){
  return -1;
}




