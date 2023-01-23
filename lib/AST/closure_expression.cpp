//
// Created by wuser on 18/01/23.
//

#include "japc/AST/closure_expression.h"

using namespace Pascal;

// Closure Expression
ClosureExpression::ClosureExpression(const Location &loc, std::shared_ptr<TypeDeclaration> ty,
                  std::vector<std::shared_ptr<VariableExpression>> &vf)
    : ExpressionAST(loc, ExpressionType::TYPE_CLOSURE, ty), content(vf)
{
}
llvm::Value *ClosureExpression::codeGen()
{
}
bool ClosureExpression::classof(const ExpressionAST *e)
{
    return e->getExpressionType() == ExpressionType::TYPE_CLOSURE;
}