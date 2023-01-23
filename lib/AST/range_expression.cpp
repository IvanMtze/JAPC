//
// Created by wuser on 18/01/23.
//
#include "japc/AST/range_expression.h"

using namespace Pascal;
RangeExpression::RangeExpression(const Location &loc, std::shared_ptr<ExpressionAST> lhs,
                                 std::shared_ptr<ExpressionAST> rhs)
    : ExpressionAST(loc, ExpressionType::TYPE_RANGE_EXPRE), low(lhs), high(rhs)
{
}
llvm::Value *RangeExpression::getLow()
{
}
llvm::Value *RangeExpression::getHigh()
{
}
std::shared_ptr<ExpressionAST> RangeExpression::getLowExpression()
{
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> RangeExpression::getHighExpression()
{
    return std::shared_ptr<ExpressionAST>();
}

bool RangeExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_RANGE_EXPRE;
}
bool RangeExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}
void RangeExpression::accept(ExpressionVisitor &visitor)
{
    low->accept(visitor);
    high->accept(visitor);
    visitor.visit(this);
}
std::shared_ptr<TypeDeclaration> RangeExpression::getTypeDeclaration() const
{
    return low->getTypeDeclaration();
}