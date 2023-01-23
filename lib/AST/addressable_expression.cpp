//
// Created by wuser on 18/01/23.
//

#include "japc/AST/addressable_expression.h"

using namespace Pascal;
AddressableExpression::AddressableExpression(const Location &location, ExpressionType expressionType,
                      std::shared_ptr<TypeDeclaration> typeDeclaration)
    : ExpressionAST(location, expressionType, typeDeclaration)
{
}
llvm::Value *AddressableExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

llvm::Value *AddressableExpression::getAddress()
{
    assert(0 && "What?!");
    return 0;
}
const std::string AddressableExpression::getName() const
{
    return "";
}

bool AddressableExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() >= ExpressionType::TYPE_ADDRESSABLE_EXPRE &&
           expressionAst->getExpressionType() <= ExpressionType::TYPE_LAST_ADDRESSABLE;
}
bool AddressableExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}