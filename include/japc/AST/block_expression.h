//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_BLOCK_EXPRESSION_H
#define JAPC_BLOCK_EXPRESSION_H

#include "japc/AST/expression.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class BlockExpression : public ExpressionAST
{
  public:
    BlockExpression(const Location &loc, std::vector<std::shared_ptr<ExpressionAST>> body);
    bool isEmpty();
    llvm::Value* codeGen() override;
    std::vector<std::shared_ptr<ExpressionAST>> &getBody();
    static bool isClassOf(ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::vector<std::shared_ptr<ExpressionAST>> content;
};

}
#endif // JAPC_BLOCK_EXPRESSION_H
