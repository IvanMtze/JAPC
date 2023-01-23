//
// Created by wuser on 18/01/23.
//

#include "japc/AST/for_expression.h"
using namespace Pascal;
ForExpression::ForExpression(const Location &loc, std::shared_ptr<VariableExpression> var,
                             std::shared_ptr<ExpressionAST> start, std::shared_ptr<ExpressionAST> end, bool down,
                             std::shared_ptr<ExpressionAST> body)
    : ExpressionAST(loc, ExpressionType::TYPE_FOR_EXPRE), variable(var), start(start), stepDown(down), end(end),
      body(body)
{
}
ForExpression::ForExpression(const Location &loc, std::shared_ptr<VariableExpression> var,
                             std::shared_ptr<ExpressionAST> start, std::shared_ptr<ExpressionAST> body)
    : ExpressionAST(loc, ExpressionType::TYPE_FOR_EXPRE), variable(var), start(start), stepDown(false), end(nullptr),
      body(body)
{
}
llvm::Value *ForExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void ForExpression::accept(ExpressionVisitor &visitor)
{

    ExpressionAST::accept(visitor);
}
llvm::Value *ForExpression::forInGen()
{
    return ExpressionAST::codeGen();
}
bool ForExpression::isStepDown() const
{
    return stepDown;
}
void ForExpression::setStepDown(bool stepDown)
{
    ForExpression::stepDown = stepDown;
}
const std::shared_ptr<VariableExpression> &ForExpression::getVariable() const
{
    return variable;
}
void ForExpression::setVariable(const std::shared_ptr<VariableExpression> &variable)
{
    ForExpression::variable = variable;
}
const std::shared_ptr<ExpressionAST> &ForExpression::getStart() const
{
    return start;
}
void ForExpression::setStart(const std::shared_ptr<ExpressionAST> &start)
{
    ForExpression::start = start;
}
const std::shared_ptr<ExpressionAST> &ForExpression::getEnd() const
{
    return end;
}
void ForExpression::setEnd(const std::shared_ptr<ExpressionAST> &end)
{
    this->end = end;
}
const std::shared_ptr<ExpressionAST> &ForExpression::getBody() const
{
    return body;
}
void ForExpression::setBody(const std::shared_ptr<ExpressionAST> &body)
{
    ForExpression::body = body;
}
bool ForExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_FOR_EXPRE;
}
bool ForExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}