//
// Created by wuser on 18/01/23.
//
#include "japc/AST/variant_field_expression.h"
using namespace Pascal;
VariantFieldExpression::VariantFieldExpression(const Location &loc,
                                               std::shared_ptr<VariableExpression> variableExpression, int element,
                                               std::shared_ptr<TypeDeclaration> typeDeclaration)
    : element(element), expre(variableExpression),
      VariableExpression(loc, ExpressionType::TYPE_VARIANT_EXPRE, variableExpression, typeDeclaration)
{
}
llvm::Value *VariantFieldExpression::getAddress()
{
    return VariableExpression::getAddress();
}
bool VariantFieldExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_VARIANT_EXPRE;
}