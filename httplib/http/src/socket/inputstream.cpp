#include "inputstream.h"
#include <stdio.h>
#include <limits.h>

long InputStream::MAX_SIZE = SSIZE_MAX; 

InputStream::InputStream(){

}

InputStream::~InputStream(){
  _close();
}

int InputStream::_close(){
  return -1;
}

int InputStream::readByte(char &ch){
  return -1;
}

int InputStream::_read(char* chs,int len){
  return this->_read(chs, 0, len);
}

//uncheck  outOfIndex
int InputStream::_read(char *chs, int off, int len){
  if(chs == NULL || len == 0){
    return 0;
  }else{
    char ch;
    int ret = readByte(ch);
    if(ret == 0 || ret == -1){
      return ret;
    }else{
      chs[off] = ch;
      int i = 1; 
      while(i < len){
        ret = readByte(ch);
        if (ret == -1 || ret == 0) {
          break;
        }
        chs[off + i] = ch;
        ++i;
      }
      return i;
    }

  }
  return -1;
}
