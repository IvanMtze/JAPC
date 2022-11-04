//
// Created by wuser on 15/10/22.
//

#ifndef JAPC_STACK_H
#define JAPC_STACK_H

#include <deque>
#include <map>
#include <vector>
namespace Pascal
{
template <typename T> class Stack
{
  public:
    void addLevel()
    {
        stack.push_back(std::map<std::string, T>());
    }
    std::vector<T> getLevelValues(){}
    void deleteLevel(){}
    T findAtTop(std::string &name){}
    T find(std::string &name){
        return nullptr;
    }
    T findFirstLevel(std::string &name){}
    T findAtLevel(std::string &name, size_t level){}
    size_t getSize(){
        return 0;
    }
    bool insert(std::string name,T t){
        return false;
    }

  private:
    std::deque<std::map<std::string, T>> stack;
};

} // namespace Pascal
#endif // JAPC_STACK_H
