#include "thread_pool.h"
#include <unistd.h>
#include <stdio.h>
class TestTask:public Task{
  public:
    
  TestTask(int i){
    m_index = i;
  }

  void run() override{
    printf("processing task mIndex:%d\n",m_index);
    sleep(2);
  }

  private:
  int m_index;
};
int main(){
 ThreadPool pool; 
 pool.init(5);
 int i = 0;
 while(true){
   try{
      sleep(1); 
      pool.addTask(new TestTask(i++));
   }catch(exception& ex){
      printf("excep:%s\n",ex.what());
   }

 }
 
}
