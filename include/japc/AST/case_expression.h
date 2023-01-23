//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_CASE_EXPRESSION_H
#define JAPC_CASE_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/label_expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class CaseExpression : public ExpressionAST
{
  public:
    CaseExpression(const Location &loc, std::shared_ptr<ExpressionAST> expr,
                   const std::vector<std::shared_ptr<LabelExpression>> &labels, std::shared_ptr<ExpressionAST> other);
    llvm::Value *codeGen();

    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;
    const std::shared_ptr<ExpressionAST> &getExpre() const;
    const std::vector<std::shared_ptr<LabelExpression>> &getLabels() const;
    const std::shared_ptr<ExpressionAST> &getOtherwise() const;

  private:
    std::shared_ptr<ExpressionAST> expre;
    std::vector<std::shared_ptr<LabelExpression>> labels;
    std::shared_ptr<ExpressionAST> otherwise;
};
} // namespace Pascal
#endif // JAPC_CASE_EXPRESSION_H
