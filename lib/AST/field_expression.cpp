//
// Created by wuser on 18/01/23.
//

#include "japc/AST/field_expression.h"

using namespace Pascal;
FieldExpression::FieldExpression(const Location &loc, std::shared_ptr<VariableExpression> variableExpression, int element,
                std::shared_ptr<TypeDeclaration> typeDeclaration)
    : element(element), expression(variableExpression),
      VariableExpression(loc, ExpressionType::TYPE_FIELD_EXPRE, variableExpression, typeDeclaration)
{
}
llvm::Value *FieldExpression::getAddress()
{
    return VariableExpression::getAddress();
}
void FieldExpression::accept(ExpressionVisitor &visitor)
{
    expression->accept(visitor);
    visitor.visit(this);
}
bool FieldExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_FIELD_EXPRE;
}