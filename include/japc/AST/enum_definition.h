//
// Created by wuser on 20/01/23.
//

#ifndef JAPC_ENUM_DEFINITION_H
#define JAPC_ENUM_DEFINITION_H
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
namespace Pascal{
class EnumDefinition : public NamedObject
{
  public:
    EnumDefinition(const std::string name, const std::shared_ptr<TypeDeclaration> &typeDeclaration, int value);
    int getValue() const;
    static bool isClassOf(const NamedObject *namedObject);
    //used by llvm:isa and dyn_cast, faster and secure casting
    static bool classof(const NamedObject *namedObject);

  private:
    int value;
    const static NamedObjectType namedObjectType = NamedObjectType::ENUM;
};
}
#endif // JAPC_ENUM_DEFINITION_H
