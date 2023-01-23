//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_FUNCTION_EXPRESSION_H
#define JAPC_FUNCTION_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/prototype_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/variable_expression.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal
{
class FunctionExpression : public VariableExpression
{
  public:
    FunctionExpression(const Location &loc, const std::shared_ptr<PrototypeExpression> p);
    llvm::Value *codeGen() override;
    const std::shared_ptr<PrototypeExpression> getPrototype() const;
    static bool isClassOf(const ExpressionAST *expressionAst);

  private:
    const std::shared_ptr<PrototypeExpression> prototype;
};
} // namespace Pascal
#endif // JAPC_FUNCTION_EXPRESSION_H
