//
// Created by wuser on 18/01/23.
//
#include "japc/AST/assign_expression.h"
using namespace Pascal;
AssignExpression::AssignExpression(const Location &location, std::shared_ptr<ExpressionAST> &lhs, std::shared_ptr<ExpressionAST> &rhs)
    : ExpressionAST(location, ExpressionType::TYPE_ASSIGN_EXPRE), lhs(lhs), rhs(rhs)
{
}
llvm::Value *AssignExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
llvm::Value *AssignExpression::assignStr()
{
    return ExpressionAST::codeGen();
}
llvm::Value *AssignExpression::assignSet()
{
    return ExpressionAST::codeGen();
}
const std::shared_ptr<ExpressionAST> &AssignExpression::getLhs() const
{
    return lhs;
}
const std::shared_ptr<ExpressionAST> &AssignExpression::getRhs() const
{
    return rhs;
}
void AssignExpression::setLhs(const std::shared_ptr<ExpressionAST> &lhs)
{
    AssignExpression::lhs = lhs;
}
void AssignExpression::setRhs(const std::shared_ptr<ExpressionAST> &rhs)
{
    AssignExpression::rhs = rhs;
}

bool AssignExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_ASSIGN_EXPRE;
}

bool AssignExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}
void AssignExpression::accept(ExpressionVisitor &visitor)
{
    lhs->accept(visitor);
    visitor.visit(this);
    rhs->accept(visitor);
}