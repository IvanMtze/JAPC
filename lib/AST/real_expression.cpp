//
// Created by wuser on 18/01/23.
//

#include "japc/AST/real_expression.h"
using namespace Pascal;
RealExpression::RealExpression(const Location &loc, double value, std::shared_ptr<TypeDeclaration> typeDec)
    : ExpressionAST(loc, ExpressionType::TYPE_REAL_EXPRE, typeDec), value(value)
{
}
llvm::Value *RealExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
bool RealExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_REAL_EXPRE;
};

bool RealExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}