//
// Created by wuser on 18/01/23.
//

#include "japc/AST/pointer_expression.h"
using namespace Pascal;
PointerExpression::PointerExpression(const Location &location, std::shared_ptr<ExpressionAST> variableExpre,
                  std::shared_ptr<TypeDeclaration> typeDeclaration)
    : pointer(variableExpre), AddressableExpression(location, ExpressionType::TYPE_POINTER_EXPRE, typeDeclaration)
{
}
llvm::Value *PointerExpression::getAddress()
{
}
void PointerExpression::accept(ExpressionVisitor &visitor)
{
    pointer->accept(visitor);
    visitor.visit(this);
}

bool PointerExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_POINTER_EXPRE;
}