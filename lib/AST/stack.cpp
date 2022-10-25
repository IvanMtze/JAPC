//
// Created by wuser on 15/10/22.
//
#include "japc/AST/stack.h"
using namespace Pascal;

template <typename T> std::vector<T> Stack<T>::getLevelValues()
{
    return std::vector<T>();
}
template <typename T> void Stack<T>::deleteLevel()
{
}
template <typename T> T Stack<T>::findAtTop(std::string &name)
{
}
template <typename T> T Stack<T>::find(std::string &name)
{
}
template <typename T> T Stack<T>::findFirstLevel(std::string &name)
{
}
template <typename T> T Stack<T>::findAtLevel(std::string &name, size_t level)
{
}
template <typename T> size_t Stack<T>::getSize()
{
    return 0;
}
template <typename T> bool Stack<T>::insert(std::string name, T t)
{
    return false;
}
