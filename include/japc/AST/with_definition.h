//
// Created by wuser on 20/01/23.
//

#ifndef JAPC_WITH_DEFINITION_H
#define JAPC_WITH_DEFINITION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
namespace Pascal
{
class WithDefinition : public NamedObject
{
  public:
    WithDefinition(const std::string &name, const std::shared_ptr<TypeDeclaration> &typeDeclaration,
                   const std::shared_ptr<ExpressionAST> &expression);
    const std::shared_ptr<ExpressionAST> &getExpression() const;
    static bool isClassOf(NamedObject *namedObject);
    static bool isClassOf(const NamedObject *namedObject);
    // used by llvm:isa and dyn_cast, faster and secure casting
    static bool classof(const NamedObject *namedObject);

  private:
    std::shared_ptr<ExpressionAST> expression;
    const static NamedObjectType namedObjectType = NamedObjectType::WITH;
};
} // namespace Pascal
#endif // JAPC_WITH_DEFINITION_H
