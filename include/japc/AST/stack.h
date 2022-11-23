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
    Stack(){
        addLevel();
    }
    void addLevel()
    {
        stack.push_back(std::map<std::string, T>());
    }
    std::vector<T> getLevelValues(){}
    void deleteLevel(){
        stack.pop_back();
    }
    T findAtTop(std::string &name){}
    T find(std::string &name){
        int maxL = getSize();
        for(auto s = stack.rbegin(); s!=stack.rend(); s++,maxL++){
            auto iterator = s->find(name);
            if(iterator!= s->end()){
                return iterator->second;
            }
        }
        return nullptr;
    }
    T findFirstLevel(std::string &name){}
    T findAtLevel(std::string &name, size_t level){}
    size_t getSize(){
        return stack.size()-1;
    }
    bool insert(std::string name,T t){
        typename std::map<std::string, T>::const_iterator iterator = stack.back().find(name);
        if(iterator == stack.back().end()){
            stack.back()[name] = t;
            return true;
        }
        return false;
    }

  private:
    std::deque<std::map<std::string, T>> stack;
};

template<typename T>
class StackWrapper
{
  public:
    StackWrapper(std::shared_ptr<Stack<T>>& v) : stack(v) { stack->addLevel(); }
    ~StackWrapper() { stack->deleteLevel(); }

  private:
    std::shared_ptr<Stack<T>>& stack;
};
} // namespace Pascal
#endif // JAPC_STACK_H
