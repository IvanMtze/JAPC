//
// Created by wuser on 7/09/22.
//

#ifndef JAPC_STRING_UTILS_H
#define JAPC_STRING_UTILS_H

#include <algorithm>
#include <iterator>
#include <list>
#include <memory>
#include <string>

class string_utils
{
  public:
    static char getCharAt(const int skip, const std::string string);
    static bool compareAhead(const int skip, const std::string mainString, const std::string compareString);
    static bool compareAhead(const int skip, const std::string mainString, const char &compareChar);
    static std::string fromCharCode(int charCode);
    static std::string fromCharCode(int charCode[]);
    static std::string fromCharCode(std::list<int> charCode);
    static std::string toUpperCase(std::string str);
    static char codePointAt(std::string source, int pos);

  private:
    string_utils()
    {
    }
};

#endif // JAPC_STRING_UTILS_H
