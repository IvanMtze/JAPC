//
// Created by wuser on 18/01/23.
//

#include "japc/AST/builtin_expression.h"
using namespace Pascal;
BuiltInExpression::BuiltInExpression(const Location &loc, std::shared_ptr<FunctionBaseBuiltin> builtin)
    : ExpressionAST(loc, ExpressionType::TYPE_BUILTIN_EXPRE, builtin->getType()), builtin(builtin)
{
}
llvm::Value *BuiltInExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

void BuiltInExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
bool BuiltInExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_BUILTIN_EXPRE;
}
bool BuiltInExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}