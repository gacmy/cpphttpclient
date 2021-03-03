#ifndef G_TASK_H
#define G_TASK_H
class Task{
  public:
    Task(){};
    Task(const Task& t) = delete;
    Task& operator=(const Task& t) =delete;
    virtual ~Task(){};
    virtual void run()=0;
  private:

};


#endif
