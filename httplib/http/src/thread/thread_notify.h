#ifndef G_THREAD_NOTIFY_H
#define G_THREAD_NOTIFY_H
#include <pthread.h>

class ThreadNotify{
  public:
    ThreadNotify();
    ThreadNotify(const ThreadNotify& t) = delete;
    ~ThreadNotify();
    ThreadNotify& operator=(const ThreadNotify& t) = delete;
    void lock(){
      pthread_mutex_lock(&m_mutex); 
    }
    void unlock(){
      pthread_mutex_unlock(&m_mutex);
    }
    void wait(){
      pthread_cond_wait(&m_cond, &m_mutex);
    }
    void signal(){
      pthread_cond_signal(&m_cond);
    }
  private:
    pthread_mutex_t m_mutex; 
    pthread_mutexattr_t m_mutexattr; 
    pthread_cond_t m_cond;
};


#endif
