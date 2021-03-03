#ifndef G_EVENT_THREAD_H
#define G_EVENT_THREAD_H
#include "thread.h"
class EventThread:Thread{
  public:
    EventThread();
    ~EventThread();

    EventThread(const EventThread& thread) = delete;
    EventThread& operator=(const EventThread& t) = delete;

    virtual void onThreadTick() = 0;
    virtual void onThreadRun();
    virtual void startThread();
    virtual void stopThread();
    bool isRuning(){return m_bRuning;}
  private:
    bool m_bRuning;

};

#endif
