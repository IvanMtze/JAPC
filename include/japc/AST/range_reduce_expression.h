//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_RANGE_REDUCE_EXPRESSION_H
#define JAPC_RANGE_REDUCE_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal
{
class RangeReduceExpression : public ExpressionAST
{
  public:
    RangeReduceExpression(ExpressionAST *expressionAst, RangeDeclaration *r);
    RangeReduceExpression(ExpressionType expressionType, ExpressionAST *expressionAst, RangeDeclaration *r);
    llvm::Value *codeGen() override;
    void accept(ExpressionVisitor &v) override;
    static bool isClassOf(const ExpressionAST *expressionAst);

  protected:
    std::shared_ptr<ExpressionAST> expr;
    std::shared_ptr<RangeDeclaration> range;
};

} // namespace Pascal
#endif // JAPC_RANGE_REDUCE_EXPRESSION_H
