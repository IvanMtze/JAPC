//
// Created by wuser on 18/01/23.
//

#include "japc/AST/range_reduce_expression.h"

using namespace Pascal;
RangeReduceExpression::RangeReduceExpression(ExpressionAST *expressionAst, RangeDeclaration *r)
    : ExpressionAST(expressionAst->getLocation(), ExpressionType::TYPE_RANGE_REDUCE_EXPRE,
                    expressionAst->getTypeDeclaration()),
      expr(expressionAst), range(r)
{
}
RangeReduceExpression::RangeReduceExpression(ExpressionType expressionType, ExpressionAST *expressionAst,
                                             RangeDeclaration *r)
    : ExpressionAST(expressionAst->getLocation(), expressionType, expressionAst->getTypeDeclaration()),
      expr(expressionAst), range(r)
{
}
llvm::Value *RangeReduceExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void RangeReduceExpression::accept(ExpressionVisitor &v)
{
    expr->accept(v);
    v.visit(this);
}
bool RangeReduceExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return (expressionAst->getExpressionType() == ExpressionType::TYPE_RANGE_REDUCE_EXPRE) ||
           (expressionAst->getExpressionType() == ExpressionType::TYPE_RANGE_CHECK_EXPRE);
}