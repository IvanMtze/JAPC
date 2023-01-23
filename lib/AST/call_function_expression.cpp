//
// Created by wuser on 18/01/23.
//
#include "japc/AST/call_function_expression.h"
using namespace Pascal;
CallFunctExpression::CallFunctExpression(const Location &w, std::shared_ptr<ExpressionAST> calle,
                    std::vector<std::shared_ptr<ExpressionAST>> &args,
                    std::shared_ptr<PrototypeExpression> prototype)
    : ExpressionAST(w, ExpressionType::TYPE_CALL_EXPRE, prototype->getTypeDeclaration()),
      prototypeExpression(prototype), callee(calle), args(args)
{
}
llvm::Value *CallFunctExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void CallFunctExpression::accept(ExpressionVisitor &visitor)
{
    callee->accept(visitor);
    for (auto i : args)
    {
        i->accept(visitor);
    }
    visitor.visit(this);
}

bool CallFunctExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_CALL_EXPRE;
}
bool CallFunctExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}
const std::shared_ptr<PrototypeExpression> CallFunctExpression::getPrototype()
{
    return this->prototypeExpression;
}
std::shared_ptr<ExpressionAST> CallFunctExpression::getCallee() const
{
    return callee;
}
std::vector<std::shared_ptr<ExpressionAST>> CallFunctExpression::getArgs()
{
    return this->args;
}