//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_WHILE_EXPRESSION_H
#define JAPC_WHILE_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal
{
class WhileExpression : public ExpressionAST
{
  public:
    WhileExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond, std::shared_ptr<ExpressionAST> body);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &expressionVisitor) override;

  private:
    std::shared_ptr<ExpressionAST> condition;
    std::shared_ptr<ExpressionAST> body;
};

} // namespace Pascal
#endif // JAPC_WHILE_EXPRESSION_H
