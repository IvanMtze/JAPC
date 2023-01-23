//
// Created by wuser on 18/01/23.
//

#include "japc/AST/array_expression.h"
#include "japc/AST/variable_expression.h"

using namespace Pascal;
ArrayExpression::ArrayExpression(const Location &loc, std::shared_ptr<ExpressionAST> v,
                                 const std::vector<std::shared_ptr<ExpressionAST>> inds,
                                 const std::vector<std::shared_ptr<RangeDeclaration>> rangeDeclaration,
                                 std::shared_ptr<TypeDeclaration> typeDeclaration)
    : AddressableExpression(loc, ExpressionType::TYPE_ARRAY_EXPRE, typeDeclaration)
{
}
llvm::Value *ArrayExpression::getAddress()
{
}
void ArrayExpression::accept(ExpressionVisitor &v)
{
    for (auto i : indices)
    {
        i->accept(v);
    }
    expression->accept(v);
    v.visit(this);
}
const std::shared_ptr<VariableExpression> &ArrayExpression::getExpression() const
{
    return expression;
}
const std::vector<std::shared_ptr<ExpressionAST>> &ArrayExpression::getIndices() const
{
    return indices;
}
const std::vector<std::shared_ptr<RangeDeclaration>> &ArrayExpression::getRanges() const
{
    return ranges;
}
const std::vector<size_t> &ArrayExpression::getIndexmul() const
{
    return indexmul;
}

bool ArrayExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_ARRAY_EXPRE;
}
