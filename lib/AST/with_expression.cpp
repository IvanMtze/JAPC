//
// Created by wuser on 18/01/23.
//

#include "japc/AST/with_expression.h"

using namespace Pascal;
WithExpression::WithExpression(const Location &loc, ExpressionAST *body)
    : ExpressionAST(loc, ExpressionType::TYPE_WITH_EXPRE), body(body)
{
}
llvm::Value *WithExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
bool WithExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_WITH_EXPRE;
}
void WithExpression::accept(ExpressionVisitor &visitor)
{
    body->accept(visitor);
    visitor.visit(this);
}