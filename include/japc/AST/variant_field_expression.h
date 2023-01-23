//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_VARIANT_FIELD_EXPRESSION_H
#define JAPC_VARIANT_FIELD_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/variable_expression.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class VariantFieldExpression : public VariableExpression
{
  public:
    VariantFieldExpression(const Location &loc, std::shared_ptr<VariableExpression> variableExpression, int element,
                           std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value *getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst);

  private:
    std::shared_ptr<VariableExpression> expre;
    int element;
};

} // namespace Pascal
#endif // JAPC_VARIANT_FIELD_EXPRESSION_H
