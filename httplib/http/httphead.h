#ifndef G_HTTPHEAD_H
#define G_HTTPHEAD_H
#include <vector>
#include <string>
using namespace std;

class HttpHead{
  public:

    HttpHead(){};

    HttpHead(const HttpHead& head) = delete;

    HttpHead& operator=(const HttpHead&);
    
    void add(string key,string value);

    string get(string& key);

    void add(const string& content);

    void clear();

    vector<string>& getHead(){
      return mHead;
    }

  private:
   vector<string> mHead; 
    
};

#endif
