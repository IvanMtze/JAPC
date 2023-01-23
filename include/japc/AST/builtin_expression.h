//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_BUILTIN_EXPRESSION_H
#define JAPC_BUILTIN_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class BuiltInExpression : public ExpressionAST
{
  public:
    BuiltInExpression(const Location &loc, std::shared_ptr<FunctionBaseBuiltin> builtin);
    llvm::Value* codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<FunctionBaseBuiltin> builtin;
};
}
#endif // JAPC_BUILTIN_EXPRESSION_H
