//
// Created by wuser on 18/01/23.
//
#include "japc/AST/range_check_expression.h"
using namespace Pascal;
RangeCheckExpression::RangeCheckExpression(ExpressionAST *expressionAst, RangeDeclaration *rangeDecl)
    : RangeReduceExpression(ExpressionType::TYPE_RANGE_CHECK_EXPRE, expressionAst, rangeDecl)
{
}
llvm::Value *RangeCheckExpression::codeGen()
{
    return RangeReduceExpression::codeGen();
}
bool RangeCheckExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_RANGE_CHECK_EXPRE;
}