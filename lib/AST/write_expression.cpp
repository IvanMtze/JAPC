//
// Created by wuser on 18/01/23.
//

#include "japc/AST/write_expression.h"

using namespace Pascal;
WriteExpression::WriteExpression(const Location &loc, std::shared_ptr<VariableExpression> out,
                                 const std::vector<wargs> &args, bool isWriteToLin)
    : ExpressionAST(loc, ExpressionType::TYPE_WRITE), output(out), args(args), isWriteToLn(isWriteToLin)
{
}
llvm::Value *WriteExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void WriteExpression::accept(ExpressionVisitor &expressionVisitor)
{
    ExpressionAST::accept(expressionVisitor);
}
bool WriteExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_WRITE;
}
bool WriteExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}