#ifndef G_DNS_H
#define G_DNS_H
#include <string>
#include <netdb.h>
#include <vector>
#include <iostream>

using namespace std;
class Dns{
  public:
    Dns(const string& name);

    void operator=(const Dns& name) = delete;

    bool isIPv6();

    class Result{
      public:
        string name;
        vector<string> ips; 
        bool isIPv6;

        void print(){
          cout<<"host:"<<name<<endl; 
          if(isIPv6){
            cout<<"ipv6"<<endl;
          }else{
            cout<<"ipv4"<<endl;
          }
          
          for(int i = 0; i < ips.size();i++){
            cout<<"ip:["<<i<<"] "<<ips[i]<<endl;
          }
        }
    };

    void getIpByName(Result* res);
  private:
    string m_host;


};



#endif


