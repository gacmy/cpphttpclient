#include "segment.h"
#include <string.h>
#include <stdlib.h>
#include "valid_error.h"
#include "segmentpool.h"

int Segment::SIZE = 8192;
int Segment::SHARE_MINIMUM = 1024;

Segment::Segment(){
  data = new byte[SIZE];
  owner = true;
  shared = false;
}


Segment::Segment(byte* data,int pos,int limit,bool shared,bool owner){
  this->data = data; 
  this->pos = pos;
  this->limit = limit;
  this->shared = shared;
  this->owner = owner;
}


Segment* Segment::sharedCopy(){
  //需要共享内存出去 shared 置为true  
  shared = true;
  //共享出去的数据 onwer = false 代表它不是数据的拥有者
  return new Segment(data,pos,limit,true,false);
}

Segment* Segment::unSharedCopy(){
  //创建一个全新的data 需要使用现在的segment数据 和标记内容
  Segment* seg = createSegment();
  seg->setPos(pos);
  seg->setLimit(limit);
  memcpy(seg->data,data,SIZE);
    
  return seg;
}
//创建全新的Segment
Segment* Segment::createSegment(){
  return new Segment(new byte[SIZE],0,0,false,true);
}

//这里弹出的元素 不负责销毁，交给外部销毁
Segment* Segment::pop(){
  //next != this 说明循环链表有超过一个元素 返回下一个元素
  //如果next == this 说明循环链表 只有一个元素 弹出自己后 返回后继元素为NULL
  Segment* result = (next != this ? next : NULL); 
  prev->next = next;
  next->prev = prev; 
  next = NULL;
  prev = NULL;
  return result;
}

//当前节点的后面插入seg 返回当前seg
Segment* Segment::push(Segment *seg){
  seg->prev = this; 
  seg->next = next;
  next->prev = seg;
  next = seg;
  return seg;
}

//将当前segment 划分为两个segment
//第一个数据范围是pos...pos+byteCount 
//第二个数据范围是pos+byteCount...limit
Segment* Segment::split(int byteCount){
  if(byteCount <= 0 || byteCount > (limit-pos))
    throw valid_error("byteCount param is illegal");
  Segment* prefix;
  //字节数必须大于一 定的值才能划分segment 避免一个复制一个segment只有很少量的数据
  //导致segment链表很长
  //数据量越大，共享数据的效率大于分配内存的开销。
  //数据量越小，共享数据 会导致链表很长
  if(byteCount >= SHARE_MINIMUM){
    //共享一个data数据实例
    prefix = sharedCopy(); 
  }else{
    //从缓冲区拿内存，没有就重新创建
    prefix = SegmentPool::take(); 
    //将data pos位置的数据 拷贝byteCount个数据 到prefix里面去
    memcpy(prefix, data+pos, byteCount);
  }
  //prefix pos..pos+byteCount
  prefix->limit = pos+byteCount; 
  //cur segment  pos+byteCount .. limit
  pos+=byteCount;
  prev->push(prefix);
  return prefix;
}

//合并segment
//tail segment and prev segment may both be less than half
//full copy data,and recycle segment
//compact cur segment to prev segment
void Segment::compact(){
  if(prev == this) throw valid_error("segment count is only one");
  if(!prev->owner) return;//segment data is get from other segment
  int byteCount = limit-pos;//cur segment byte count
  int avaliableCount = SIZE - prev->limit+(prev->shared?0:prev->pos);//prev tail avaliableCount+ prev head avaliableCount
  if(byteCount > avaliableCount) return;//not enough writable space
  writeTo(prev, byteCount);
  pop();
  SegmentPool::recycle(this);
}

//write data from this segment to sink
void Segment::writeTo(Segment *sink, int byteCount){
  if(!sink->owner) throw valid_error("sink is not data owner,not be writeable");  
  //sink tail space and head space both need
  if(sink->limit + byteCount > SIZE){
    if(sink->shared) throw valid_error("sink head space be shared,  not be writeable");
    if(sink->limit+byteCount-sink->pos > SIZE) throw valid_error("head space and tail space have not enough writable space");
    //move sink data pos..limit to 0..limit-pos
    memcpy(sink->data,sink->data+sink->pos, sink->limit-sink->pos);
    //reset sink
    sink->setLimit(sink->limit-sink->pos);
    sink->setPos(0);
  }
  //copy cur segment pos..pos+byteCount to sink after limit
  memcpy(sink->data+limit, data+pos, byteCount);
  //sink limit pos reset
  sink->setLimit(sink->limit+byteCount);
  //cur segment consume bytecount byte
  pos+=byteCount;
}

void Segment::setLimit(int limit){
  this->limit = limit;
}
void Segment::setPos(int pos){
  this->pos = pos;
}

Segment::~Segment(){
   delete data; 
}
