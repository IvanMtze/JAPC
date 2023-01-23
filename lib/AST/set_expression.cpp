//
// Created by wuser on 18/01/23.
//

#include "japc/AST/set_expression.h"
using namespace Pascal;
SetExpression::SetExpression(const Location &location, ExpressionType expressionType,
              std::shared_ptr<TypeDeclaration> typeDeclaration,
              const std::vector<std::shared_ptr<ExpressionAST>> &values)
    : values(values), AddressableExpression(location, expressionType, typeDeclaration)
{
}
llvm::Value *SetExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
llvm::Value *SetExpression::makeConstantSet(TypeDeclaration *type)
{
}
const std::vector<std::shared_ptr<ExpressionAST>> &SetExpression::getValues() const
{
    return values;
}

bool SetExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_SET_EXPRE;
}