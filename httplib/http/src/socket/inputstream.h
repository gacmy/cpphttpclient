#ifndef G_INPUTSTREAM_H
#define G_INPUTSTREAM_H
/**
 *EBADF  fd is not a valid file descriptor or is not open for reading.
  EFAULT buf is outside your accessible address space.
  EINTR  The call was interrupted by a signal before any data was read; see signal(7).
 *
 */
class InputStream{
  public:
    InputStream();

    InputStream(const InputStream& ins) = delete;

    InputStream& operator=(const InputStream& ins) = delete;

    virtual ~InputStream();

    virtual int _close();
    
    virtual void settimeout(int seconds){
    };

    virtual int readByte(char& ch);

    int _read(char* chs,int off,int len);

    int _read(char* chs,int len);
  private:
    static long MAX_SIZE;
     
};

#endif
