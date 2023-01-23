//
// Created by wuser on 18/01/23.
//

#include "japc/AST/while_expression.h"
using namespace Pascal;
WhileExpression::WhileExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond,
                                 std::shared_ptr<ExpressionAST> body)
    : ExpressionAST(loc, ExpressionType::TYPE_WHILE_EXPRE), condition(cond), body(body)
{
}
llvm::Value *WhileExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

void WhileExpression::accept(ExpressionVisitor &expressionVisitor)
{
    condition->accept(expressionVisitor);
    body->accept(expressionVisitor);
}
bool WhileExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_WHILE_EXPRE;
}