#include "buffer.h"
#include "segmentpool.h"
#include "valid_error.h"
#include <stdio.h>
#include <math.h>
#include <string.h>


Buffer::Buffer():size(0),head(NULL){}

//consume one byte from head
byte Buffer::readByte(){
  if(size == 0) throw valid_error("size == 0");  
  Segment* seg = head;
  int pos = seg->getPos();
  int limit = seg->getLimit();
  //cout<<"readByte pos:"<<pos<<" limit:"<<limit<<endl;
  byte* data = seg->getData();
  byte b = data[pos++];  
  //cout<<"byte result:"<<b<<endl;
  size-=1;
  if(pos == limit){
    head = seg->pop();
    SegmentPool::recycle(seg);
  }else{
    seg->setPos(pos);
  }
  return b;
}

long Buffer::getSize(){
  return size;
}

byte Buffer::getByte(long pos){
  checkOffsetAndCount(size, pos, 1);
  //from head to tail look for it
  if(size - pos > pos){
    for(Segment* s = head;true;s = s->next){
      int segmentByteCount = s->getLimit()-s->getPos();
      if(pos < segmentByteCount) return s->getData()[s->getPos()+pos];
      pos-=segmentByteCount; 
    } 
  }else{
    //from tail to head look for it
    pos-=size;
    for(Segment* s = head->prev;true;s = s->prev){
      pos+= s->getLimit()-s->getPos();
      if(pos >= 0) return s->getData()[s->getPos()+pos];
    }
  }
}

byte* Buffer::readByteArray() {
  try {
    return readByteArray(size);
  } catch (valid_error& var2) {
    throw;
  }
}

byte* Buffer::readByteArray(long byteCount){
  checkOffsetAndCount(size, 0L, byteCount);
  if (byteCount > 2147483647L) {
    throw valid_error("byteCount too much");
  } else {
    byte* result = new byte[(int)byteCount];
    readFully(result,byteCount);
    return result;
  }
}


int Buffer::readInt(){
  if(size < 4) throw valid_error("size < 4");

  Segment* segment = head;
  int pos = segment->getPos();
  int limit = segment->getLimit();

  // If the int is split across multiple segments, delegate to readByte().
  if (limit - pos < 4) {
    return (readByte() & 0xff) << 24
      |  (readByte() & 0xff) << 16
      |  (readByte() & 0xff) <<  8
      |  (readByte() & 0xff);
  }

  byte* data = segment->getData();
  int i = 0;
  i |= (data[pos++]&0xff)<<24;
  i |= (data[pos++]&0xff)<<16;
  i |= (data[pos++]&0xff)<<8;
  i |= (data[pos++]&0xff);

  size -= 4;

  if (pos == limit) {
    head = segment->pop();
    SegmentPool::recycle(segment);
  } else {
    segment->setPos(pos);
  }
  return i;
}

long Buffer::readLong(){
  if (size < 8) throw new valid_error("size < 8: ");

  Segment* segment = head;
  int pos = segment->getPos();
  int limit = segment->getLimit();

  // If the long is split across multiple segments, delegate to readInt().
  if (limit - pos < 8) {
    return (readInt() & 0xffffffffL) << 32
      |  (readInt() & 0xffffffffL);
  }

  byte* data = segment->getData();
  long v = 0;
  v|= (data[pos++] & 0xffL) << 56;
  v|=(data[pos++] & 0xffL) << 48;
  v|=(data[pos++] & 0xffL) << 40;
  v|=(data[pos++] & 0xffL) << 32;
  v|=(data[pos++] & 0xffL) << 24;
  v|=(data[pos++] & 0xffL) << 16;
  v|=(data[pos++] & 0xffL) <<  8;
  v|=(data[pos++] & 0xffL);
  size -= 8;

  if(pos == limit) {
    head = segment->pop();
    SegmentPool::recycle(segment);
  } else {
    segment->setPos(pos);
  }

  return v;
}

void Buffer::skip(long byteCount){
  while (byteCount > 0) {
    if (head == NULL) throw new valid_error("EOF error");
    int limit = head->getLimit();
    int pos = head->getPos();
    int toSkip = 0;
    limit > pos?toSkip = pos:toSkip = limit;
    size -= toSkip;
    byteCount -= toSkip;
    head->setPos(pos+toSkip);

    if (head->getPos() == head->getLimit()) {
      Segment* toRecycle = head;
      head = toRecycle->pop();
      SegmentPool::recycle(toRecycle);
    }
  }
}


Buffer& Buffer::writeByte(byte b){
  Segment* tail = writeableSegment(1);
  int limit = tail->getLimit();
  byte* data = tail->getData();
  data[limit++] = (byte) b;
  tail->setLimit(limit);
  size += 1;
  return *this;
}

Buffer& Buffer::writeInt(int i){
  Segment* tail = writeableSegment(4);
  byte* data = tail->getData();
  int limit = tail->getLimit();
  data[limit++] = (byte) ((i >> 24) & 0xff);
  data[limit++] = (byte) ((i >> 16) & 0xff);
  data[limit++] = (byte) ((i >>  8) & 0xff);
  data[limit++] = (byte)  (i        & 0xff);
  tail->setLimit(limit);
  size += 4;
  return *this;
}

Buffer& Buffer::writeLong(long v){
  Segment* tail = writeableSegment(8);
  byte* data = tail->getData();
  int limit = tail->getLimit();
  data[limit++] = (byte) ((v >> 56L) & 0xff);
  data[limit++] = (byte) ((v >> 48L) & 0xff);
  data[limit++] = (byte) ((v >> 40L) & 0xff);
  data[limit++] = (byte) ((v >> 32L) & 0xff);
  data[limit++] = (byte) ((v >> 24L) & 0xff);
  data[limit++] = (byte) ((v >> 16L) & 0xff);
  data[limit++] = (byte) ((v >>  8L) & 0xff);
  data[limit++] = (byte)  (v          & 0xff);
  tail->setLimit(limit);
  size += 8;
  return *this;
}

const Buffer& Buffer::write(byte* source,int size){
  if (source == NULL) throw new valid_error("source == null");
  return write(source, 0, size,size);
}

const Buffer& Buffer::write(byte *source, int offset, int byteCount, int sourceSize){
  if (source == NULL) throw new valid_error("source == null");
  //check source offset byteCount sourceSize
  checkOffsetAndCount(sourceSize, offset, byteCount);
  //souce-> limit
  int limit = offset + byteCount;
  while (offset < limit) {
    //get tail segment
    Segment* tail = writeableSegment(1);
    int toCopy = (limit-offset) > (Segment::SIZE-tail->getLimit())?Segment::SIZE-tail->getLimit():limit-offset; 
    byte* data = tail->getData();
    memcpy(data+limit,source+offset,toCopy);
    offset += toCopy;
    tail->setLimit(tail->getLimit()+toCopy);
  }

  size += byteCount;
  return *this;
}

Segment* Buffer::writeableSegment(int minimumCapacity){
  if (minimumCapacity < 1 || minimumCapacity > Segment::SIZE) throw valid_error("minimumCapacity param is illegal");

  if (head == NULL) {
    head = SegmentPool::take(); // Acquire a first segment.
    head->next = head;
    head->prev = head;
    return head;
  }

  Segment* tail = head->prev;

  int limit = tail->getLimit();
  if ((limit + minimumCapacity > Segment::SIZE) || !tail->owner) {
    tail = tail->push(SegmentPool::take()); // Append a new empty segment to fill up.
  }
  return tail;
}


//write data from source to this buffer
void Buffer::write(Buffer &source, long byteCount){

  //if (source == null) throw new IllegalArgumentException("source == null");
  if (&source == this) throw new valid_error("source == this");
  checkOffsetAndCount(source.size, 0, byteCount);

  while (byteCount > 0) {
    // Is a prefix of the source's head segment all that we need to move?
    if (byteCount < (source.head->getLimit() - source.head->getPos())) {
      Segment* tail = head != NULL ? head->prev : NULL;
      if (tail != NULL && tail->owner
          && (byteCount + tail->getLimit()- (tail->shared ? 0 : tail->getPos()) <= Segment::SIZE)) {
        // Our existing segments are sufficient. Move bytes from source's head to our tail.
        source.head->writeTo(tail, (int) byteCount);
        source.size -= byteCount;
        size = byteCount;
        return;
      } else {
        // We're going to need another segment. Split the source's head
        // segment in two, then move the first of those two to this buffer.
        source.head = source.head->split((int) byteCount);
      }
    }

    // Remove the source's head segment and append it to our tail.
    Segment* segmentToMove = source.head;
    long movedByteCount = segmentToMove->getLimit() - segmentToMove->getPos();
    source.head = segmentToMove->pop();
    if (head == NULL) {
      head = segmentToMove;
      head->next = head->prev = head;
    } else {
      Segment* tail = head->prev;
      tail = tail->push(segmentToMove);
      tail->compact();
    }
    source.size =source.size - movedByteCount;
    size += movedByteCount;
    byteCount -= movedByteCount;
  }
}

const long Buffer::read(Buffer &sink, long byteCount){
  //jif (sink == null) throw new IllegalArgumentException("sink == null");
  if (byteCount < 0) throw valid_error("byteCount < 0: ");
  if (size == 0) return -1L;
  if (byteCount > size) byteCount = size;
  sink.write(*this, byteCount);
  return byteCount;
}

void Buffer::readFully(byte *sink, int size){
  int readCount;
  for(int offset = 0; offset < size; offset += readCount) {
    readCount = read(sink, offset, size - offset,size);
    if (readCount== -1) {
      throw new valid_error("EOF");
    }
  }
}

int Buffer::read(byte *sink, int offset, int byteCount, int size){
  checkOffsetAndCount((long)size, (long)offset, (long)byteCount);
  Segment* s = this->head;
  if (s == NULL) {
    return -1;
  } else {
    int toCopy = byteCount < s->getLimit()-s->getPos()?byteCount:s->getLimit()-s->getPos();
    memcpy(sink+offset,s->getData()+s->getPos(), toCopy);             
    s->setPos(s->getPos()+toCopy);
    this->size -= (long)toCopy;
    if (s->getPos() == s->getLimit()) {
      this->head = s->pop();
      SegmentPool::recycle(s);
    }

    return toCopy;
  }
}

Buffer* Buffer::clone(){
  Buffer* result = new Buffer();
  if (size == 0) return result;

  result->head = head->sharedCopy();
  result->head->next = result->head->prev = result->head;
  for (Segment* s = head->next; s != head; s = s->next) {
    result->head->prev->push(s->sharedCopy());
  }
  result->size = size;
  return result;
}

Buffer& Buffer::writeUtf8(string content) {
  return writeUtf8(content, 0, content.length());
}

Buffer& Buffer::writeUtf8(std::string& content, int begin, int end){
  if(begin < 0) throw valid_error("begin < 0"); 
  else if(begin > end) throw valid_error("begin > end");
  else if(end > content.length()) throw valid_error("end > content length");
  else{
    const char* str = content.c_str();
    //wstring str =  ANSIToUnicode(content);      
    //cout<<"wstring len:"<<str.length()<<endl;
    int i = begin;
    while(true){
      while(i < end){
        int c = str[i]; 
        cout<<"c:"<<c<<endl;
        if(c < 128){//<0x7f 一个字节编码
          Segment* tail = writeableSegment(1); 
          byte* data = tail->getData();
          int segmentOffset = tail->getLimit()-i;
          int runLimit = end > (8192-segmentOffset)?(8192-segmentOffset):end;

          for(data[segmentOffset + i++]=(byte)c; i < runLimit;data[segmentOffset + i++] = (byte)c){
            c = str[i]; 
            if(c > 128)
              break;
          }
          int runSize = i+segmentOffset-tail->getLimit();
          tail->setLimit(tail->getLimit()+runSize);
          size+=runSize;
        }else if(c < 2048){//< 0x7ff 2个字节表示
          writeByte(c >> 6 | 192); 
          writeByte(c & 63 | 128);
          ++i;
          //55296 57343
        }else if(c >= 55296&& c <= 57343){
          int low = i + 1 < end ? str[i + 1] : 0;
          if (c <= 56319 && low >= 56320 && low <= 57343) {
            int codePoint = 65536 + ((c & -55297) << 10 | low & -56321);
            writeByte(codePoint >> 18 | 240);
            writeByte(codePoint >> 12 & 63 | 128);
            writeByte(codePoint >> 6 & 63 | 128);
            writeByte(codePoint & 63 | 128);
            i += 2;
          } else {
            writeByte(63);
            ++i;
          }
        }else {
          writeByte(c >> 12 | 224);
          writeByte(c >> 6 & 63 | 128);
          writeByte(c & 63 | 128);
          ++i;
        }
      } 
      return *this;
    }

  }
}

std::string Buffer::readUtf8(){
  return readUtf8(size);
}

std::string Buffer::readUtf8(long byteCount){
  checkOffsetAndCount(this->size, 0L, byteCount);
  if (byteCount > 2147483647L) {
    throw new valid_error("byteCount too much" );
  } else if (byteCount == 0L) {
    return "";
  } else {
    Segment* s = this->head;
    if ((long)s->getPos() + byteCount > (long)s->getLimit()) {
      return string(reinterpret_cast<const char*>(readByteArray(byteCount)));
    } else {
      string result = string(reinterpret_cast<const char*>(s->getWritePtr()),  (int)byteCount);
      s->setPos(s->getPos()+byteCount);
      this->size -= byteCount;
      if (s->getPos() == s->getLimit()) {
        this->head = s->pop();
        SegmentPool::recycle(s);
      }

      return result;
    }
  }
}

void  Buffer::checkOffsetAndCount(long size, long offset, long byteCount) {
  if ((offset | byteCount) < 0 || offset > size || size - offset < byteCount) {
    char buf[256]; 
    snprintf(buf,sizeof(buf),"size=%ld offset=%ld byteCount=%ld", size, offset, byteCount);
    throw valid_error(buf);
  }
}

long Buffer::indexOf(byte &b, long &fromIndex, long &toIndex){
  if(fromIndex < 0 || toIndex < fromIndex){
    return -1;
  }
  if(toIndex > size) toIndex = size;  
  if(fromIndex == toIndex) return -1;

  Segment* s;
  long offset;

  s = head;
  if(s == NULL){
    return -1;
  }else if(size - fromIndex < fromIndex){
    //the back half of this buffer
    offset = size; 
    while(offset > fromIndex){
      s = s->prev;
      offset -=(s->getLimit()-s->getPos());
    }
  }else{
    //the front half of this buffer
    offset = 0L;
    for (long nextOffset; (nextOffset = offset + (s->getLimit() - s->getPos())) < fromIndex; ) {
      s = s->next;
      offset = nextOffset;
    }
  }

  //as above we find segment,now we scan through segment find the byte
  //
  while (offset < toIndex) {
    byte* data = s->getData();
    int limit = s->getLimit() > (s->getPos()+toIndex -offset)?(s->getPos()+toIndex-offset):s->getLimit();
    int pos = (int) (s->getPos()+ fromIndex - offset);
    for (; pos < limit; pos++) {
      if (data[pos] == b) {
        return pos - s->getPos() + offset;
      }
    }
    // Not in this segment. Try the next one.
    offset += (s->getLimit() - s->getPos());
    fromIndex = offset;
    s = s->next;
  }

  return -1L;
}


long Buffer::indexOf(byte& b) {
  long fromIndex = 0;
  return indexOf(b, fromIndex, size);
}

/**
 * Returns the index of {@code b} in this at or beyond {@code fromIndex}, or
 * -1 if this buffer does not contain {@code b} in that range.
 */
long Buffer::indexOf(byte& b, long& fromIndex) {
  return indexOf(b, fromIndex, size);
}

string Buffer::readUtf8Line(){
    byte line= '\n';    
    long newline = indexOf(line);
    if (newline == -1) {
      return size != 0 ? readUtf8(size) : NULL;
    }
    return readUtf8Line(newline);
}

string Buffer::readUtf8Line(long newline){
  if (newline > 0 && getByte(newline - 1) == '\r') {
      //Read everything until '\r\n', then skip the '\r\n'.
      string result = readUtf8((newline - 1));
      skip(2);
      return result;

    }else {
      // Read everything until '\n', then skip the '\n'.
      string result = readUtf8(newline);
      skip(1);
      return result;
    }
}




