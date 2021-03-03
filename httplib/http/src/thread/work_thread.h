#ifndef G_WORK_THREAD_H
#define G_WORK_THREAD_H
#include <list>
#include "task.h"
#include "thread_notify.h"
#include <pthread.h>
#include <stdint.h>
using namespace std;
class WorkThread{
  public:
     WorkThread();
    ~WorkThread();
    WorkThread(const WorkThread& w) = delete;
    WorkThread& operator=(const WorkThread& w) =delete;
   static void* startRoutine(void* arg); 

   void start();
   void execute();
   void pushTask(Task* task);

   void setThreadIdx(uint32_t idx){
    m_thread_idx = idx;
   }
  private:
   uint32_t m_thread_idx;
   uint32_t m_task_cnt;
   pthread_t m_thread_id;
   ThreadNotify m_thread_notify;
   list<Task*> m_task_list; 


};

#endif
