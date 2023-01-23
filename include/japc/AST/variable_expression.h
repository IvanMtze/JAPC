//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_VARIABLE_EXPRESSION_H
#define JAPC_VARIABLE_EXPRESSION_H
#include "japc/AST/addressable_expression.h"
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class VariableExpression : public AddressableExpression
{
  public:
    VariableExpression(const Location &location, const std::string &name,
                       std::shared_ptr<TypeDeclaration> typeDeclaration);
    VariableExpression(const Location &location, ExpressionType expressionType, const std::string &name,
                       std::shared_ptr<TypeDeclaration> typeDeclaration);
    VariableExpression(const Location &location, ExpressionType expressionType,
                       std::shared_ptr<VariableExpression> variableExpre,
                       std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value *getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    const std::string getName() const override;

  protected:
    std::string name;
};

} // namespace Pascal
#endif // JAPC_VARIABLE_EXPRESSION_H
