//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_TYPECAST_EXPRESSION_H
#define JAPC_TYPECAST_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/addressable_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal{
class TypeCastExpression : public AddressableExpression
{
  public:
    TypeCastExpression(const Location &loc, std::shared_ptr<ExpressionAST> expressionAst,
                       std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value* codeGen() override;
    llvm::Value* getAddress() override;
    std::shared_ptr<ExpressionAST> getExpressionn();
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &v) override;

  private:
    std::shared_ptr<ExpressionAST> expr;
};

}
#endif // JAPC_TYPECAST_EXPRESSION_H
