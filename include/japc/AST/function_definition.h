//
// Created by wuser on 20/01/23.
//

#ifndef JAPC_FUNCTION_DEFINITION_H
#define JAPC_FUNCTION_DEFINITION_H
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
namespace Pascal
{
class FunctionDefinition : public NamedObject
{
  public:
    FunctionDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                       const std::shared_ptr<PrototypeExpression> &prototype);
    const std::shared_ptr<PrototypeExpression> &getPrototype() const;
    static bool isClassOf(NamedObject *namedObject);
    static bool isClassOf(const NamedObject *namedObject);
    // used by llvm:isa and dyn_cast, faster and secure casting
    static bool classof(const NamedObject *namedObject);
    static bool classof(const std::shared_ptr<NamedObject> namedObject);

  private:
    std::shared_ptr<PrototypeExpression> prototype;
    const static NamedObjectType namedObjectType = NamedObjectType::FUNCTION;
};
} // namespace Pascal
#endif // JAPC_FUNCTION_DEFINITION_H
