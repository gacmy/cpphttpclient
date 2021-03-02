#ifndef G_VALID_ERROR_H
#define G_VALID_ERROR_H
#include <exception>
#include <iostream>
#include <string>
using namespace std;
class valid_error:public std::exception{

  public:
  valid_error(const char* c)
  {
    error = c;	
  }

  //what()函数返回错误信息
  virtual const char* what()
  {
    cout << "异常类型为 valid_error: "<<error << endl;
    return error.c_str();
  }
  private:
  string error;

};

#endif
