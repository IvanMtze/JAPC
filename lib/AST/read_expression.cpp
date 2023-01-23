//
// Created by wuser on 18/01/23.
//

#include "japc/AST/read_expression.h"
using namespace Pascal;
ReadExpression::ReadExpression(const Location &w, AddressableExpression *fi,
                               const std::vector<std::shared_ptr<ExpressionAST>> &a, bool isLn)
    : ExpressionAST(w, ExpressionType::TYPE_READ), file(fi), args(a), isReadln(isLn)
{
}
llvm::Value *ReadExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void ReadExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
bool ReadExpression::classof(const ExpressionAST *e)
{
    return e->getExpressionType() == ExpressionType::TYPE_READ;
}