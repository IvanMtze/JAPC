//
// Created by wuser on 18/01/23.
//
// FUNCTION
#include "japc/AST/function.h"
using namespace Pascal;
Function::Function(const Location &loc, std::shared_ptr<PrototypeExpression> prototype,
                   const std::vector<std::shared_ptr<VariableDeclarationExpression>> variablesDecl,
                   std::shared_ptr<BlockExpression> block)
    : ExpressionAST(loc, ExpressionType::TYPE_FUNCTION), prototype(prototype), variablesDecl(variablesDecl),
      body(block), parent(nullptr), returnType(nullptr)
{
}
llvm::Function *Function::codeGen(const std::string &namePrefix)
{
}
std::shared_ptr<TypeDeclaration> Function::getReturnType()
{
    return std::shared_ptr<TypeDeclaration>();
}
void Function::accept(ExpressionVisitor &v)
{
    v.visit(this);
    for (auto varD : variablesDecl)
    {
        varD->accept(v);
    }
    if (body)
    {
        body->accept(v);
    }
    for (auto subfunc : subFunctions)
    {
        subfunc->accept(v);
    }
}
llvm::Function *Function::codeGen() const
{
}

const std::shared_ptr<PrototypeExpression> Function::getPrototype() const
{
    return this->prototype;
}
void Function::addSubFunctions(const std::vector<std::shared_ptr<Function>> subs)
{
    subFunctions = subs;
}
void Function::setParent(std::shared_ptr<Function> parent)
{
    this->parent = parent;
}
const std::vector<std::shared_ptr<Function>> Function::getSubFunctions()
{
    return subFunctions;
}
void Function::setUsedVars(const std::set<VariableDefinition> &usedVariables)
{
    this->usedVariables = usedVariables;
}
const std::set<VariableDefinition> &Function::getUsedVars()
{
    return this->usedVariables;
}
const std::string Function::closureName()
{
    return "$$CLOSURE";
}
bool Function::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_FUNCTION;
}
void Function::setEndLoc(Location loc)
{
    this->endLoc = loc;
}
const std::shared_ptr<Function> Function::getParent()
{
    return this->parent;
}
