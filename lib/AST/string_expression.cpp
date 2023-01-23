//
// Created by wuser on 18/01/23.
//

#include "japc/AST/string_expression.h"
using namespace Pascal;
StringExpression::StringExpression(const Location &location, const std::string &value,
                 std::shared_ptr<TypeDeclaration> typeDeclaration)
    : value(value), AddressableExpression(location, ExpressionType::TYPE_STRING_EXPRE, typeDeclaration)
{
}
llvm::Value *StringExpression::codeGen()
{
    return AddressableExpression::codeGen();
}
llvm::Value *StringExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
const std::string &StringExpression::getValue() const
{
    return value;
}
bool StringExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_STRING_EXPRE;
}