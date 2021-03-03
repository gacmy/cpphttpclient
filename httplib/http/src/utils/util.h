#ifndef G_UTIL_H
#define G_UTIL_H
#include <string>
#include <vector>
using namespace std;

#define CR                  '\r'
#define LF                  '\n'
#define LOWER(c)            (unsigned char)(c | 0x20)
#define IS_ALPHA(c)         (LOWER(c) >= 'a' && LOWER(c) <= 'z')
#define IS_NUM(c)           ((c) >= '0' && (c) <= '9')
#define IS_ALPHANUM(c)      (IS_ALPHA(c) || IS_NUM(c))
#define IS_HEX(c)           (IS_NUM(c) || (LOWER(c) >= 'a' && LOWER(c) <= 'f'))
#define IS_MARK(c)          ((c) == '-' || (c) == '_' || (c) == '.' || \
  (c) == '!' || (c) == '~' || (c) == '*' || (c) == '\'' || (c) == '(' || \
  (c) == ')')

#define CONTENT_LENGTH "Content-Length"
#define TRANSFER_ENDING "Transfer-Encoding"
typedef void (*callback_t)(string callback_data, uint32_t code);

namespace HttpUtil{
    
  int skipLeadingAsciiWhiteSpace(string& input,int pos,int limit);
  int skipLeadingAsciiWhiteSpace(string& input);
  int skipTrailingAsciiWhiteSpace(string& input,int pos,int limit);
  bool isIpv4(const string& strs);

  bool isNum(const char& ch);
  
  bool isNum(const string& strs);

  bool isPortIllegal(const int&port);
  void genereateParamString(vector<string>& vec,string& str);
  int skipTrailingAsciiWhiteSpace(string& input);
  string trimSubString(string input,int pos,int limit);
  void hexchar(unsigned char c, unsigned char &hex1, unsigned char &hex2);
  
  string urlencode(string& s);
  void fromHex(char& x);
  string urldecode(string& str);
  void spilt(const string& source,const string& deli,int pos,int end,vector<string>& vec);
};

#endif

