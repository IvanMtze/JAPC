//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_SET_EXPRESSION_H
#define JAPC_SET_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/addressable_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class SetExpression  : public AddressableExpression
{
  public:
    SetExpression(const Location &location, ExpressionType expressionType,
                  std::shared_ptr<TypeDeclaration> typeDeclaration,
                  const std::vector<std::shared_ptr<ExpressionAST>> &values);
    llvm::Value* getAddress() override;
    llvm::Value* makeConstantSet(TypeDeclaration *type);
    static bool isClassOf(const ExpressionAST *expressionAst);
    const std::vector<std::shared_ptr<ExpressionAST>> &getValues() const;

  private:
    std::vector<std::shared_ptr<ExpressionAST>> values;
};

}
#endif // JAPC_SET_EXPRESSION_H
