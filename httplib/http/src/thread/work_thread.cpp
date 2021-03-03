#include "work_thread.h"
#include <stdio.h>

WorkThread::WorkThread():m_task_cnt(0){}

WorkThread::~WorkThread(){}

void* WorkThread::startRoutine(void *arg){
   WorkThread* pthread = reinterpret_cast<WorkThread*>(arg);
   pthread->execute();
   return NULL;
}

void WorkThread::start(){
  pthread_create(&m_thread_id, NULL, startRoutine, this); 
}

void WorkThread::execute(){
  while(true){
    m_thread_notify.lock();
    while(m_task_list.empty()){
      m_thread_notify.wait();  
    }
    Task* pTask = m_task_list.front();
    m_task_list.pop_front();
    m_thread_notify.unlock();
    printf("thread %d is processing...\n",m_thread_idx); 
    pTask->run();
    delete pTask;
    //have finished task count
    m_task_cnt++;
  }

}

void WorkThread::pushTask(Task *task){
  if(task == NULL){
    return;
  }
  m_thread_notify.lock();
  m_task_list.push_back(task);
  m_thread_notify.signal();
  m_thread_notify.unlock();
}
