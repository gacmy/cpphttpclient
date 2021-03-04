#ifndef G_OUTPUTSTREAM_H
#define G_OUTPUTSTREAM_H
/**
 *  EAGAIN or EWOULDBLOCK
 The  file  descriptor  fd refers to a socket and has been marked nonblocking (O_NONBLOCK), and the write
 would block.  POSIX.1-2001 allows either error to be returned for this case, and does not require  these
 constants to have the same value, so a portable application should check for both possibilities.

 EBADF  fd is not a valid file descriptor or is not open for writing.

 EDESTADDRREQ
 fd refers to a datagram socket for which a peer address has not been set using connect(2).

 EDQUOT The  user's  quota  of  disk blocks on the filesystem containing the file referred to by fd has been ex‐
 hausted.

 EFAULT buf is outside your accessible address space.

 EFBIG  An attempt was made to write a file that exceeds the implementation-defined maximum  file  size  or  the
 process's file size limit, or to write at a position past the maximum allowed offset.

 EINTR  The call was interrupted by a signal before any data was written; see signal(7).

 EINVAL fd  is  attached  to an object which is unsuitable for writing; or the file was opened with the O_DIRECT
 flag, and either the address specified in buf, the value specified in count, or the file offset  is  not
 suitably aligned.

 **/
//not handle any eception
class OutputStream{
  public:
    OutputStream();

    OutputStream(const OutputStream& outs) = delete;

    OutputStream& operator=(const OutputStream& outs) = delete;

    virtual ~OutputStream();

    virtual int _write(char& ch);

    int _write(char* chs,int len);

    int _write(char* chs, int off, int len); 

    virtual void flush();

    virtual int _close();
};

#endif
