//
// Created by wuser on 20/01/23.
//

#ifndef JAPC_LABEL_DEFINITION_H
#define JAPC_LABEL_DEFINITION_H

#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
namespace Pascal{
class LabelDefinition : public NamedObject
{
  public:
    LabelDefinition(const int name);
    static bool isClassOf(NamedObject *namedObject);
    static bool isClassOf(const NamedObject *namedObject);
    //used by llvm:isa and dyn_cast, faster and secure casting
    static bool classof(const NamedObject *namedObject);
  private:
    const static NamedObjectType namedObjectType = NamedObjectType::LABEL;
};
}
#endif // JAPC_LABEL_DEFINITION_H
