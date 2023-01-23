//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_TRAMPOLINE_EXPRESSION_H
#define JAPC_TRAMPOLINE_EXPRESSION_H
#include "japc/AST/closure_expression.h"
#include "japc/AST/expression.h"
#include "japc/AST/function_expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class TrampolineExpression : public FunctionExpression
{
  public:
    TrampolineExpression(const Location &w, std::shared_ptr<FunctionExpression> fn,
                         std::shared_ptr<ClosureExpression> c, std::shared_ptr<FunctionPointerDeclaration> fnPtrTy);
    llvm::Value *codeGen() override;
    static bool classof(const ExpressionAST *e);
    void accept(ExpressionVisitor &v) override;

  private:
    std::shared_ptr<FunctionExpression> func;
    std::shared_ptr<ClosureExpression> closure;
    std::shared_ptr<FunctionPointerDeclaration> funcPtrTy;
};
} // namespace Pascal
#endif // JAPC_TRAMPOLINE_EXPRESSION_H
