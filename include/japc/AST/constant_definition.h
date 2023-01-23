//
// Created by wuser on 20/01/23.
//

#ifndef JAPC_CONSTANT_DEFINITION_H
#define JAPC_CONSTANT_DEFINITION_H
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
namespace Pascal{
class ConstantDefinition : public NamedObject
{
  public:
    ConstantDefinition(const std::string& nm, const std::shared_ptr<ConstantDeclaration> cv);
    static bool isClassOf(NamedObject *namedObject);
    static bool isClassOf(const NamedObject *namedObject);
    //used by llvm:isa and dyn_cast, faster and secure casting
    static bool classof(const NamedObject *namedObject);
  public:
    const std::shared_ptr<ConstantDeclaration> getConstValue() const;

  private:
    const static NamedObjectType namedObjectType = NamedObjectType::CONST;
    std::shared_ptr<ConstantDeclaration> constValue;
};
}

#endif // JAPC_CONSTANT_DEFINITION_H
