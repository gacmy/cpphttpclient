#ifndef G_SEGMENT_POOL_H
#define G_SEGMENT_POOL_H
#include "segment.h"
#include <mutex>
class SegmentPool{
  public:
    static Segment* take();
    static void recycle(Segment* seg);
  private:
    SegmentPool(){};

    static long byteCount;
    static Segment* next; 
    static long MAXSIZE;
    //not recursive lock!! be caution!!
    static std::mutex lock;
};

#endif
