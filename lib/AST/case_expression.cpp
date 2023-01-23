//
// Created by wuser on 18/01/23.
//

#include "japc/AST/case_expression.h"

using namespace Pascal;
CaseExpression::CaseExpression(const Location &loc, std::shared_ptr<ExpressionAST> expr,
               const std::vector<std::shared_ptr<LabelExpression>> &labels, std::shared_ptr<ExpressionAST> other)
    : ExpressionAST(loc, ExpressionType::TYPE_CASE_EXPRE), expre(expr), labels(labels), otherwise(other)
{
}
llvm::Value *CaseExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void CaseExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
const std::shared_ptr<ExpressionAST> &CaseExpression::getExpre() const
{
    return expre;
}
const std::vector<std::shared_ptr<LabelExpression>> &CaseExpression::getLabels() const
{
    return labels;
}
const std::shared_ptr<ExpressionAST> &CaseExpression::getOtherwise() const
{
    return otherwise;
}
bool CaseExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_CASE_EXPRE;
}
bool CaseExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}