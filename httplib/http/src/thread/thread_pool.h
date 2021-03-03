#ifndef G_THREAD_POOL_H
#define G_THREAD_POOL_H
#include <stdint.h>
#include "work_thread.h"
#include "task.h"

class ThreadPool{
  public:
     ThreadPool();
    ~ThreadPool();
    ThreadPool(const ThreadPool& t) = delete;
    ThreadPool& operator=(const ThreadPool& t) = delete;
    int init(uint32_t worksize);
    void addTask(Task* task);
    void destroy();
  private:
    uint32_t m_worker_size;
    WorkThread* m_worker_list;
};

#endif
