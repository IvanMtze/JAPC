//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_WITH_EXPRESSION_H
#define JAPC_WITH_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class WithExpression : public ExpressionAST
{
  public:
    WithExpression(const Location &loc, ExpressionAST *body);
    llvm::Value *codeGen();
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> body;
};
} // namespace Pascal
#endif // JAPC_WITH_EXPRESSION_H
