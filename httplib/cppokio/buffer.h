#ifndef G_BUFFER_H
#define G_BUFFER_H
#include <string>
#include "segment.h"

class Buffer{
  public:


    Buffer();

    Buffer(const Buffer&) = delete;

    Buffer& operator=(const Buffer&) = delete;

    long getSize();

    byte readByte();

    byte getByte(long pos);

    byte* readByteArray();

    byte* readByteArray(long byteCount);

    int readInt();

    long readLong();

    void skip(long count);
    
    Buffer& writeByte(byte b);

    Buffer& writeInt(int i);
    
    Buffer& writeLong(long l);

    Buffer& writeUtf8(std::string content);

    Buffer& writeUtf8(std::string& content,int begin,int end);
      
    std::string readUtf8();

    std::string readUtf8(long byteCount);

    std::string readUtf8Line(long newline);

    std::string readUtf8Line();

    const Buffer& write(byte* bys,int size); 

    const Buffer& write(byte* bys,int offset,int byteCount,int size);
    
    Segment* writeableSegment(int minimumCapacity);

    void write(Buffer& source,long byteCount);

    const long read(Buffer& sink,long byteCount);

    void readFully(byte* sink,int size);

    int read(byte* sink,int offset,int byteCount,int size);

    //find one byte position
    long indexOf(byte& b,long& fromIndex,long& toIndex);

    long indexOf(byte& b);
  /**
   * Returns the index of {@code b} in this at or beyond {@code fromIndex}, or
   * -1 if this buffer does not contain {@code b} in that range.
   */
   long indexOf(byte& b, long& fromIndex);

    Buffer* clone();

    static void checkOffsetAndCount(long size, long offset, long byteCount);
  private:

    Segment* head;
    long size;
};

#endif
