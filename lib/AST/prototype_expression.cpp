//
// Created by wuser on 18/01/23.
//

#include "japc/AST/prototype_expression.h"
using namespace Pascal;
PrototypeExpression::PrototypeExpression(const Location &loc, const std::string &name,
                                         const std::vector<std::shared_ptr<VariableDefinition>> args,
                                         std::shared_ptr<TypeDeclaration> resultType)
    : ExpressionAST(loc, ExpressionType::TYPE_PROTOTYPE, resultType), args(args), name(name), function(0),
      isForward(false), llvmFunction(0)
{
}
llvm::Function *PrototypeExpression::create(const std::string &namePrefix)
{
}
void PrototypeExpression::createArgumentsAlloca()
{
}
void PrototypeExpression::addExtraArgsFirst(std::vector<std::shared_ptr<VariableDefinition>> vars)
{
}
bool PrototypeExpression::operator==(const PrototypeExpression &rhs) const
{
    return false;
}
bool PrototypeExpression::isMatchWithoutClosure(const PrototypeExpression *rhs) const
{
    return false;
}
bool PrototypeExpression::isClassOf(ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_PROTOTYPE;
}
std::shared_ptr<llvm::Function> PrototypeExpression::getLlvmFunction() const
{
    return llvmFunction;
}
std::string PrototypeExpression::getName() const
{
    return this->name;
}
const std::vector<std::shared_ptr<VariableDefinition>> PrototypeExpression::getArgs()
{
    return this->args;
}
bool PrototypeExpression::isForwarded() const
{
    return this->isForward;
}
void PrototypeExpression::setIsForward(bool forward)
{
    this->isForward = forward;
}
void PrototypeExpression::setFunction(std::shared_ptr<Function> function)
{
    this->function = function;
}
std::shared_ptr<Function> PrototypeExpression::getFunction() const
{
    return this->function;
}