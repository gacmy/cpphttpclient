#include "event_thread.h"

EventThread::EventThread():m_bRuning(false){}

EventThread::~EventThread(){stopThread();}

void EventThread::startThread(){
  m_bRuning = true; 
  Thread::startThread();
}

void EventThread::stopThread(){
  m_bRuning = false;
}

void EventThread::onThreadRun(){
  while(m_bRuning){
    onThreadTick();
  }
}
