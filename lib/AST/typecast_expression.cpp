//
// Created by wuser on 18/01/23.
//
#include "japc/AST/typecast_expression.h"
using namespace Pascal;
TypeCastExpression::TypeCastExpression(const Location &loc, std::shared_ptr<ExpressionAST> expressionAst,
                   std::shared_ptr<TypeDeclaration> typeDeclaration)
    : expr(expressionAst), AddressableExpression(loc, ExpressionType::TYPE_TYPE_CAST_EXPRE, typeDeclaration){};
llvm::Value *TypeCastExpression::codeGen()
{
    return AddressableExpression::codeGen();
}
llvm::Value *TypeCastExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
std::shared_ptr<ExpressionAST> TypeCastExpression::getExpressionn()
{
    return expr;
}
bool TypeCastExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_TYPE_CAST_EXPRE;
}
void TypeCastExpression::accept(ExpressionVisitor &v)
{
    expr->accept(v);
    v.visit(this);
}