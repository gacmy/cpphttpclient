
#include "util.h"
using namespace std;

int HttpUtil::skipLeadingAsciiWhiteSpace(string &input){
  return skipLeadingAsciiWhiteSpace(input,0,input.size());
}

int HttpUtil::skipLeadingAsciiWhiteSpace(string& input,int pos,int limit){
  for(int i = pos; i < limit; i++){
    switch(input[i]){
      case '\t':
      case '\n':
      case '\f':
      case '\r':
      case ' ':
        continue;
      default:
        return i;
    }
  }
  return limit;
}
int HttpUtil::skipTrailingAsciiWhiteSpace(string& input,int pos,int limit){
  for(int i = limit; i >= pos; i--){
    switch(input[i]){
      case '\t':
      case '\n':
      case '\f':
      case '\r':
      case ' ':
        continue;
      default:
        return i+1;
    }
  }
  return pos;
}
int HttpUtil::skipTrailingAsciiWhiteSpace(string& input){
  return skipTrailingAsciiWhiteSpace(input, 0, input.length());
}
string HttpUtil::trimSubString(string input,int pos,int limit){
  int start = skipLeadingAsciiWhiteSpace(input, pos, limit);
  int end   = skipTrailingAsciiWhiteSpace(input, pos, limit);
  return input.substr(start, end);
}


void HttpUtil::hexchar(unsigned char c, unsigned char &hex1, unsigned char &hex2)
{
  hex1 = c / 16;
  hex2 = c % 16;
  hex1 += hex1 <= 9 ? '0' : 'a' - 10;
  hex2 += hex2 <= 9 ? '0' : 'a' - 10;
}

string HttpUtil::urlencode(string& s)
{
  const char *str = s.c_str();
  vector<char> v(s.size());
  v.clear();
  for (size_t i = 0, l = s.size(); i < l; i++)
  {
    char c = str[i];
    if ((c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '-' || c == '_' || c == '.' || c == '!' || c == '~' ||
        c == '*' || c == '\'' || c == '(' || c == ')' ||c == '=' || c == '&')
    {
      v.push_back(c);
    }
    else if (c == ' ')
    {
      v.push_back('+');
    }
    else
    {
      v.push_back('%');
      unsigned char d1, d2;
      hexchar(c, d1, d2);
      v.push_back(d1);
      v.push_back(d2);
    }
  }

  return string(v.cbegin(), v.cend());
}
void HttpUtil::fromHex(char& x) 
{
  if (x >= 'A' && x <= 'Z') x = x - 'A' + 10;
  else if (x >= 'a' && x <= 'z') x = x - 'a' + 10;
  else if (x >= '0' && x <= '9') x = x - '0';
  else ;
}

string HttpUtil::urldecode(string& str){
  std::string strTemp = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++)
  {
    if (str[i] == '+') strTemp += ' ';
    else if (str[i] == '%')
    {
      //assert(i + 2 < length);
      fromHex(str[++i]);
      char high = str[i];
      fromHex(str[++i]);
      char low = str[i];
      strTemp += high*16 + low;
    }
    else strTemp += str[i];
  }
  return strTemp;
}


void HttpUtil::spilt(const string& source,const string& deli,int pos,int end,vector<string>& vec){
  while(pos < end){
    int temp = source.find(deli,pos);  
    //printf("temp %d pos=%d\n",temp,pos);
    if(temp == -1){
      if(pos < end){
        vec.push_back(source.substr(pos,end-pos));
      }
      break;
    }
    if(temp != pos){
      vec.push_back(source.substr(pos,temp-pos)); 
      pos = temp+1;
    }else{
      //start of delim
      if(pos == end){
        break;
      }
      pos++;
    }

  } 
}

bool HttpUtil::isIpv4(const string& ip){
  vector<string> vec; 
  spilt(ip, ".", 0, ip.size(), vec);
  if(vec.size() == 0){
    return false;
  }
  for(const string& res:vec){
    if(!isNum(res)){
      return false;
    }
  }
  return true;
}
void HttpUtil::genereateParamString(vector<string> &vec, string &str){
    for(int i = 0; i < vec.size()/2; i++){
    str.append(vec[2*i]);
    str.append("=");
    str.append(vec[2*i+1]);
    if(i != vec.size()/2-1){
      str.append("&");
    }
  }


}
bool HttpUtil::isNum(const char& ch){
  return ((ch) >= '0' && (ch) <= '9');
}

bool HttpUtil::isPortIllegal(const int &port){
  return port > 0 && port < 65535;
}
bool HttpUtil::isNum(const string& strs){
  for(const char&ch : strs){
    if(!isNum(ch)) return false;
  }
  return true;
}

