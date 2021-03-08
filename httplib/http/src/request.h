#ifndef G_REQUEST_H_
#define G_REQUEST_H_
#include <string>
#include "httpurl.h"
#include "httphead.h"
#include "./cppokio/buffer.h"


class Request{
  private:
      static const char* method_str[];

  public:
    enum Method{POST,GET} ;

    Request(string url,Method method);

    Request(const Request& req);

    Request& operator=(const Request& req);


    Request& addHead(string key,string value);

    string getHead(string key);

    Request& addParam(const string& key,const string& value);

    void request();


    int getPort();

    const std::string& getUrl() const;

    uint8_t* get(); 

 
  private:
    //create request buffer
    void createRequestBuffer(Buffer& buf);
    Method mMethod;
    HttpUrl mUrl;
    vector<string> mParams;
    HttpHead mHead;
    HttpHead mResonseHead;
    int parseCode(const string& str); 
    int mCode;

    int mPort;
};

#endif
