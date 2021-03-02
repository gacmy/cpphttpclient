#include "segmentpool.h"
#include "valid_error.h"


long SegmentPool::byteCount = 0;

std::mutex SegmentPool::lock;

Segment* SegmentPool::next = NULL;

//64kb
long SegmentPool::MAXSIZE = 64*1024;
Segment* SegmentPool::take(){
  lock.lock();
  if(next != NULL){
    //取出当前头结点
    Segment* result = next;
    //重置头结点
    next = result->next;
    //取出的头结点 next指针应该断开
    result->next = NULL;
    byteCount -= Segment::SIZE;
    return result;     
  }
  lock.unlock();

  return new Segment;
}

void SegmentPool::recycle(Segment* seg){
  if(seg == NULL){
    return;
  }
  if(seg->next != NULL || seg->prev != NULL) throw valid_error("param next || prev is not null"); 

  if(seg->shared) return; //This segment can not be recycled
  //不是递归锁
  lock.lock(); 
  //pool is full need delete seg?
  if(byteCount+Segment::SIZE > MAXSIZE){
    delete seg;
    return;
  }
  //add pool
  byteCount+=Segment::SIZE;
  seg->next = next;
  seg->setPos(0);
  seg->setLimit(0);
  next = seg;
  lock.unlock();
}

