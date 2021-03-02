#ifndef G_SEGMENT_H
#define G_SEGMENT_H
#include <sys/types.h>
typedef u_int8_t byte;

class Segment{
  public:
    Segment();

    Segment(byte* data,int pos,int limit,bool shared,bool owner);

    Segment(const Segment& seg) = delete;

    ~Segment();

    Segment& operator=(const Segment& seg) = delete;

    Segment* sharedCopy(); 

    Segment* unSharedCopy();

    //返回后继
    Segment* pop();

    //返回插入的seg
    Segment* push(Segment* seg);

    static Segment* createSegment();

    Segment* split(int byteCount);

    void compact();

    int getPos(){
      return pos;
    }

    int getLimit(){
      return limit;
    }

    //unsafe
    byte* getData(){
      return data;
    }

    byte* getWritePtr(){
      return data+pos;
    }
    void writeTo(Segment* sink,int byteCount);
    void setPos(int pos);
    void setLimit(int limit);
    
  private:

    byte* data;

    int pos;

    int limit;


  public:
    static int SIZE; 
    bool shared;

    bool owner;

    static int SHARE_MINIMUM;


    //unsafe
    Segment* next;

    Segment* prev;
};


#endif
