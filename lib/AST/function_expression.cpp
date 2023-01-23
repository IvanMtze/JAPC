//
// Created by wuser on 18/01/23.
//
#include "japc/AST/function_expression.h"
using namespace Pascal;

FunctionExpression::FunctionExpression(const Location &loc, const std::shared_ptr<PrototypeExpression> p)
    : prototype(p),
      VariableExpression(loc, ExpressionType::TYPE_FUNCTION_EXPRE, p->getName(), p->getTypeDeclaration())
{
}
llvm::Value *FunctionExpression::codeGen()
{
    return AddressableExpression::codeGen();
}

const std::shared_ptr<PrototypeExpression> FunctionExpression::getPrototype() const
{
    return this->prototype;
}
bool FunctionExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_FUNCTION_EXPRE;
}