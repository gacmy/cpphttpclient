#ifndef G_REAL_INPUTSTREAM_H
#define G_REAL_INPUTSTREAM_H
#include "inputstream.h"
class RealInputStream:public InputStream{
  public:
    RealInputStream(int fd);

    ~RealInputStream();

    virtual void settimeout(int seconds);
    
    RealInputStream(const RealInputStream& inputstream) = delete;

    RealInputStream& operator=(const RealInputStream& inputstream)=delete;
    

    virtual int _close();

    virtual int readByte(char& ch);


  
  private:
    int m_fd;

};

#endif
