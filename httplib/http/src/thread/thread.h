#ifndef G_THREAD_H
#define G_THREAD_H
#include <pthread.h>
class Thread{
  public:
    Thread();
    virtual ~Thread();
    Thread(const Thread& t) =delete;
    Thread& operator=(const Thread* t) =delete;
    virtual void startThread(void);
    virtual void onThreadRun(void) = 0;
    static void* startThreadRoutine(void* arg);
  protected:
    pthread_t m_thread_id;

};
#endif
