//
// Created by wuser on 18/01/23.
//

#include "japc/AST/trampoline_expression.h"
using namespace Pascal;
TrampolineExpression::TrampolineExpression(const Location &w, std::shared_ptr<FunctionExpression> fn,
                                           std::shared_ptr<ClosureExpression> c,
                                           std::shared_ptr<FunctionPointerDeclaration> fnPtrTy)
    : FunctionExpression(w, fn->getPrototype()), func(fn), closure(c), funcPtrTy(fnPtrTy)
{
}
llvm::Value *TrampolineExpression::codeGen()
{
}
void TrampolineExpression::accept(ExpressionVisitor &v)
{
    func->accept(v);
}
bool TrampolineExpression::classof(const ExpressionAST *e)
{
    return e->getExpressionType() == ExpressionType::TYPE_TRAMPOLINE;
}