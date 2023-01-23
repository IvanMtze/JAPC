//
// Created by wuser on 18/01/23.
//
#include "japc/AST/block_expression.h"

using namespace Pascal;
BlockExpression::BlockExpression(const Location &loc, std::vector<std::shared_ptr<ExpressionAST>> body)
    : ExpressionAST(loc, ExpressionType::TYPE_BLOCK_EXPRE), content(body)
{
}
llvm::Value *BlockExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void BlockExpression::accept(ExpressionVisitor &visitor)
{
    for (auto i : content)
    {
        i->accept(visitor);
    }
}
bool BlockExpression::isEmpty()
{
    return this->content.empty();
}

std::vector<std::shared_ptr<ExpressionAST>> &BlockExpression::getBody()
{
    return this->content;
}

bool BlockExpression::isClassOf(ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_BLOCK_EXPRE;
}