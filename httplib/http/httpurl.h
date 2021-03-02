#ifndef G_HTTPURL_H
#define G_HTTPURL_H
#include <string>
#include <vector>

using namespace std;

class HttpUrl{
  public:
    HttpUrl(){};

    int parseUrl(const string& url);

    string& getScheme(){
      return scheme;
    }

    string& getHost(){
      return host;
    }

    const string& getUrl() const{
      return url;
    }
    vector<string>& getPath(){
      return path;
    }

    vector<string>& getQuery(){
      return query;
    }

    void getQueryStr(string& str){
      for(int i = 0; i < query.size();i++){
        str.append(query[i]);
        if(i != query.size()-1){
          str.append("&");
        } 
      }
    } 

    int getPort(){
      return port;
    }

    bool isIP(){
      return is_ip;
    }
    void print();
  private:
   string host;  
   vector<string> query;  
   string scheme;
   string url;
   vector<string> path; 
   int port;

   bool is_ip;

   int parseScheme(string& url,int pos,int end);

   int parseHost(string& url,int pos,int end);

   int parsePort(string& url,int pos);

   int parsePaths(string& url,int pos);

   int parseQuery(string& url,int pos);

};
#endif
