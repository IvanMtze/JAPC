//
// Created by wuser on 7/09/22.
//

#include "string_utils.h"

bool string_utils::compareAhead(const int skip, const std::string mainString, const char &compareChar)
{
}

bool string_utils::compareAhead(const int skip, const std::string mainString, const std::string compareString)
{
}

char string_utils::getCharAt(const int skip, const std::string string)
{
    if (skip < 0 || skip > string.size())
        return '\0';
    return string[skip];
}

std::string string_utils::fromCharCode(int charCode)
{
}

std::string string_utils::fromCharCode(int *charCode)
{
}
std::string string_utils::fromCharCode(std::list<int> charCode)
{
    int arr[charCode.size()];
    std::copy(charCode.begin(), charCode.end(), arr);
    return fromCharCode(arr);
}
std::string string_utils::toUpperCase(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}
char string_utils::codePointAt(std::string source, int pos)
{
    if (pos < source.size())
        return int(getCharAt(pos, source));
    return -1;
}