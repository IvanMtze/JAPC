//
// Created by wuser on 18/01/23.
//

#include "japc/AST/variable_declaration_expression.h"
using namespace Pascal;
VariableDeclarationExpression::VariableDeclarationExpression(const Location &loc,
                                                             std::vector<std::shared_ptr<VariableDefinition>> vars)
    : ExpressionAST(loc, ExpressionType::TYPE_VAR_DECL_EXPRE), vars(vars), function(0)
{
}
llvm::Value *VariableDeclarationExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void VariableDeclarationExpression::setFunction(std::shared_ptr<Function> function)
{
    this->function = function;
}
bool VariableDeclarationExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_VAR_DECL_EXPRE;
}
const std::vector<std::shared_ptr<VariableDefinition>> VariableDeclarationExpression::getVars()
{
    return this->vars;
}