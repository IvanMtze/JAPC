//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_POINTER_EXPRESSION_H
#define JAPC_POINTER_EXPRESSION_H
#include "japc/AST/addressable_expression.h"
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal
{
class PointerExpression : public AddressableExpression
{
  public:
    PointerExpression(const Location &location, std::shared_ptr<ExpressionAST> variableExpre,
                      std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value *getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> pointer;
};
} // namespace Pascal
#endif // JAPC_POINTER_EXPRESSION_H
