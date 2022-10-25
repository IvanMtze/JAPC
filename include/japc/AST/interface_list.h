//
// Created by wuser on 19/10/22.
//

#ifndef JAPC_INTERFACE_LIST_H
#define JAPC_INTERFACE_LIST_H

#include "japc/AST/stack.h"
#include "japc/AST/named_object.h"

namespace Pascal{
class InterfaceList{
  public:
    bool add(std::string name, const NamedObject* obj);
    const std::map<std::string, const NamedObject*> getListNamedObjects();
  private:
    std::map<std::string, const NamedObject*> list;
};
}
#endif // JAPC_INTERFACE_LIST_H
