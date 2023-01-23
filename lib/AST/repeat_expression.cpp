//
// Created by wuser on 18/01/23.
//

#include "japc/AST/repeat_expression.h"
using namespace Pascal;
RepeatExpression::RepeatExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond,
                                   std::shared_ptr<ExpressionAST> body)
    : ExpressionAST(loc, ExpressionType::TYPE_REPEAT_EXPRE), cond(cond), body(body)
{
}
llvm::Value *RepeatExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void RepeatExpression::accept(ExpressionVisitor &visitor)
{
    cond->accept(visitor);
    body->accept(visitor);
}
bool RepeatExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_REPEAT_EXPRE;
}