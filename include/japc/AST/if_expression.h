//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_IF_EXPRESSION_H
#define JAPC_IF_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class IfExpression : public ExpressionAST
{
  public:
    IfExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond, std::shared_ptr<ExpressionAST> then,
                 std::shared_ptr<ExpressionAST> other);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> cond;
    std::shared_ptr<ExpressionAST> then;
    std::shared_ptr<ExpressionAST> other;
};

} // namespace Pascal
#endif // JAPC_IF_EXPRESSION_H
