#include "buffer.h"
#include <iostream>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <sys/sysinfo.h>
#include <unistd.h>
using namespace std;

void testInt(){
  Buffer buffer;
  buffer.writeInt(1);
  buffer.writeInt(2);
  cout<<buffer.readInt()<<endl; 
  cout<<buffer.readInt()<<endl;

}
void testbyte(){
  Buffer buffer;
  buffer.writeByte(10);
  printf("%d\n",buffer.readByte());
}
void testUtf(){
  Buffer buffer;
  string content = "我爱北京天安门";
  cout<<"content len:"<<content.length()<<endl;
  buffer.writeInt(100);
  buffer.writeUtf8(content);
  buffer.writeLong(1000);
  cout<<buffer.readInt()<<endl;
  cout<<buffer.readUtf8(content.length())<<endl;
  cout<<buffer.readLong()<<endl;

}

void memoryState(){
  struct sysinfo s_info;
  int error = sysinfo(&s_info);
  printf("memory state: %d, total: %lu free: %lu \n", error, s_info.totalram, s_info.freeram);
}

void testMemoryLeak(){
  while(true){
   // testUtf();
    //memoryState();
  //  sleep(1);
  }
}

int main(){
   testInt();
  testbyte();
  testUtf();
  //testMemoryLeak();
}
