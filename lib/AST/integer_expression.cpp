//
// Created by wuser on 18/01/23.
//

#include "japc/AST/integer_expression.h"

using namespace Pascal;
IntegerExpression::IntegerExpression(const Location &loc, ExpressionType expressionType, int64_t value,
                  std::shared_ptr<TypeDeclaration> typeDec)
    : value(value), ExpressionAST(loc, expressionType, typeDec)
{
}
IntegerExpression::IntegerExpression(const Location &loc, int64_t value, std::shared_ptr<TypeDeclaration> typeDec)
    : value(value), ExpressionAST(loc, ExpressionType::TYPE_INTEGER_EXPRE, typeDec)
{
}
llvm::Value *IntegerExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
bool IntegerExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_INTEGER_EXPRE ||
           expressionAst->getExpressionType() == ExpressionType::TYPE_CHAR_EXPRE;
}
bool IntegerExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}
int64_t IntegerExpression::getValue()
{
    return this->value;
}