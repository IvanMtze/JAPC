//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_REPEAT_EXPRESSION_H
#define JAPC_REPEAT_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class RepeatExpression : public ExpressionAST
{
  public:
    RepeatExpression(const Location &loc, std::shared_ptr<ExpressionAST> cond, std::shared_ptr<ExpressionAST> body);
    llvm::Value* codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> cond;
    std::shared_ptr<ExpressionAST> body;
};
}
#endif // JAPC_REPEAT_EXPRESSION_H
