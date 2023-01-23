//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_LABEL_EXPRESSION_H
#define JAPC_LABEL_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class LabelExpression : public ExpressionAST
{
  public:
    LabelExpression(const Location &loc, const std::vector<std::pair<int, int>> &lab,
                    std::shared_ptr<ExpressionAST> stmt);
    llvm::Value* codeGen() override;
    llvm::Value* codeGen(std::shared_ptr<llvm::BasicBlock> casebb,
                                         std::shared_ptr<llvm::BasicBlock> afterbb);
    void accept(ExpressionVisitor &visitor) override;
    std::vector<std::pair<int, int>> &getLabels();
    static bool isClassOf(ExpressionAST *expressionAst);
  private:
    std::vector<std::pair<int, int>> labelsValues;
    std::shared_ptr<ExpressionAST> statement;
};
}
#endif // JAPC_LABEL_EXPRESSION_H
