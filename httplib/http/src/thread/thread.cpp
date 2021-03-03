#include "thread.h"

Thread::Thread():m_thread_id(0){
}

Thread::~Thread(){
}


void Thread::startThread(){
  pthread_create(&m_thread_id, NULL, startThreadRoutine, this);  
}

void* Thread::startThreadRoutine(void *arg){
  Thread* thread =  reinterpret_cast<Thread*>(arg);
  thread->onThreadRun();
  return NULL;
}


