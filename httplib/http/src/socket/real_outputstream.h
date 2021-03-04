#ifndef G_REAL_OUTPUTSTREAM_H
#define G_REAL_OUTPUTSTREAM_H
#include "outputstream.h"
class RealOutputStream:public OutputStream{
  public:
    RealOutputStream(int fd);
    ~RealOutputStream();
    RealOutputStream(const RealOutputStream&) = delete;
    virtual void settimeout(int seconds);
    RealOutputStream& operator=(const RealOutputStream&)=delete;
    virtual int _write(char& ch);
    virtual void flush();

    virtual int _close();



  private:
    int m_fd;
};

#endif

