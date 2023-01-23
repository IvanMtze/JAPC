//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_GOTO_EXPRESSION_H
#define JAPC_GOTO_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class GotoExpression : public ExpressionAST
{
  public:
    GotoExpression(const Location &loc, int pos);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);

  private:
    int pos;
};
} // namespace Pascal
#endif // JAPC_GOTO_EXPRESSION_H
