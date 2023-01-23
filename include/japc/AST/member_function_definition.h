//
// Created by wuser on 20/01/23.
//

#ifndef JAPC_MEMBER_FUNCTION_DEFINITION_H
#define JAPC_MEMBER_FUNCTION_DEFINITION_H

#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
namespace Pascal
{
class MemberFunctionDefinition : public NamedObject
{
  public:
    MemberFunctionDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                             int index);
    int getIndex() const;
    static bool isClassOf(NamedObject *namedObject);
    static bool isClassOf(const NamedObject *namedObject);
    // used by llvm:isa and dyn_cast, faster and secure casting
    static bool classof(const NamedObject *namedObject);

  private:
    int index;
    const static NamedObjectType namedObjectType = NamedObjectType::MEMBER_FUNCTION;
};
} // namespace Pascal
#endif // JAPC_MEMBER_FUNCTION_DEFINITION_H
