//
// Created by wuser on 18/01/23.
//
#include "japc/AST/unary_expression.h"
using namespace Pascal;
UnaryExpression::UnaryExpression(const Location &loc, Token tk, std::shared_ptr<ExpressionAST> expressionAst)
    : ExpressionAST(loc, ExpressionType::TYPE_UNARY_EXPRE), op(tk), rhs(expressionAst){}
llvm::Value *UnaryExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<TypeDeclaration> UnaryExpression::getTypeDeclaration() const
{
    return rhs->getTypeDeclaration();
}
bool UnaryExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_UNARY_EXPRE;
}
void UnaryExpression::accept(ExpressionVisitor &visitor)
{
    rhs->accept(visitor);
    visitor.visit(this);
}