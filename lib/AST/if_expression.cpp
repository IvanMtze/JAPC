//
// Created by wuser on 18/01/23.
//
#include "japc/AST/if_expression.h"
using namespace Pascal;
IfExpression::IfExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond, std::shared_ptr<ExpressionAST> then,
             std::shared_ptr<ExpressionAST> other)
    : ExpressionAST(loc, ExpressionType::TYPE_IF_EXPRE), cond(cond), then(then), other(other)
{
}
llvm::Value *IfExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void IfExpression::accept(ExpressionVisitor &visitor)
{
    cond->accept(visitor);
    if (then)
    {
        then->accept(visitor);
    }
    if (other)
    {
        other->accept(visitor);
    }
}
bool IfExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_IF_EXPRE;
}