//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_UNARY_EXPRESSION_H
#define JAPC_UNARY_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class UnaryExpression : public ExpressionAST
{
  public:
    UnaryExpression(const Location &loc, Token tk, std::shared_ptr<ExpressionAST> expressionAst);
    llvm::Value* codeGen() override;
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;

  private:
    Token op;
    std::shared_ptr<ExpressionAST> rhs;
};
}
#endif // JAPC_UNARY_EXPRESSION_H
