//
// Created by wuser on 18/01/23.
//
#include "japc/AST/char_expression.h"
using namespace Pascal;
CharExpression::CharExpression(const Location &loc, char values, std::shared_ptr<TypeDeclaration> typeDeclaration)
    : IntegerExpression(loc, ExpressionType::TYPE_CHAR_EXPRE, values, typeDeclaration)
{
}
llvm::Value *CharExpression::codeGen()
{
    return llvm::ConstantInt::get(getCharType()->getLtype(), value);
}
bool CharExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_CHAR_EXPRE;
}
bool CharExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}