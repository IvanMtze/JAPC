//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_RANGE_EXPRESSION_H
#define JAPC_RANGE_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class RangeExpression : public ExpressionAST
{
  public:
    RangeExpression(const Location &loc, std::shared_ptr<ExpressionAST> lhs, std::shared_ptr<ExpressionAST> rhs);
    llvm::Value *getLow();
    llvm::Value *getHigh();
    std::shared_ptr<ExpressionAST> getLowExpression();
    std::shared_ptr<ExpressionAST> getHighExpression();
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override;

  private:
    std::shared_ptr<ExpressionAST> low;
    std::shared_ptr<ExpressionAST> high;
};

} // namespace Pascal
#endif // JAPC_RANGE_EXPRESSION_H
