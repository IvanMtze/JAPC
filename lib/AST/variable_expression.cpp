//
// Created by wuser on 18/01/23.
//

#include "japc/AST/variable_expression.h"
using namespace Pascal;
VariableExpression::VariableExpression(const Location &location, const std::string &name,
                                       std::shared_ptr<TypeDeclaration> typeDeclaration)
    : AddressableExpression(location, ExpressionType::TYPE_VARIABLE_EXPRE, typeDeclaration), name(name)
{
}
VariableExpression::VariableExpression(const Location &location, ExpressionType expressionType, const std::string &name,
                                       std::shared_ptr<TypeDeclaration> typeDeclaration)
    : AddressableExpression(location, expressionType, typeDeclaration), name(name)
{
}
VariableExpression::VariableExpression(const Location &location, ExpressionType expressionType,
                                       std::shared_ptr<VariableExpression> variableExpre,
                                       std::shared_ptr<TypeDeclaration> typeDeclaration)
    : name(variableExpre->name), AddressableExpression(location, expressionType, typeDeclaration)
{
}
llvm::Value *VariableExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
bool VariableExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() >= ExpressionType::TYPE_VARIABLE_EXPRE &&
           expressionAst->getExpressionType() <= ExpressionType::TYPE_LAST_ADDRESSABLE;
}
const std::string VariableExpression::getName() const
{
    return name;
}