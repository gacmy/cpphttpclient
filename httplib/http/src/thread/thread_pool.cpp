
#include "thread_pool.h"
#include <stdlib.h>

ThreadPool::ThreadPool():m_worker_size(0),m_worker_list(NULL){}

ThreadPool::~ThreadPool(){}

int ThreadPool::init(uint32_t worksize){
  m_worker_size = worksize;
  m_worker_list = new WorkThread[worksize];
  if(!m_worker_list){
    return 1;
  }
  for(int i = 0; i < worksize; i++){
    m_worker_list[i].setThreadIdx(i);
    m_worker_list[i].start();
  }
  return 0;
}

void ThreadPool::destroy(){
  if(m_worker_list)
    delete[] m_worker_list;
}

void ThreadPool::addTask(Task* task){
  uint32_t thread_idx = random()%m_worker_size; 
  m_worker_list[thread_idx].pushTask(task);
}
