//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_ASSIGN_EXPRESSION_H
#define JAPC_ASSIGN_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class AssignExpression : public ExpressionAST
{
  public:
    AssignExpression(const Location &location, std::shared_ptr<ExpressionAST> &lhs,
                     std::shared_ptr<ExpressionAST> &rhs);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;
    const std::shared_ptr<ExpressionAST> &getLhs() const;
    const std::shared_ptr<ExpressionAST> &getRhs() const;
    void setLhs(const std::shared_ptr<ExpressionAST> &lhs);
    void setRhs(const std::shared_ptr<ExpressionAST> &rhs);

  private:
    llvm::Value *assignStr();
    llvm::Value *assignSet();
    std::shared_ptr<ExpressionAST> lhs;
    std::shared_ptr<ExpressionAST> rhs;
};

} // namespace Pascal
#endif // JAPC_ASSIGN_EXPRESSION_H
