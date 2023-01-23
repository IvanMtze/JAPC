//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_CLOSURE_EXPRESSION_H
#define JAPC_CLOSURE_EXPRESSION_H

#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/variable_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class ClosureExpression : public ExpressionAST
{
  public:
    ClosureExpression(const Location &loc, std::shared_ptr<TypeDeclaration> ty,
                      std::vector<std::shared_ptr<VariableExpression>> &vf);
    llvm::Value* codeGen() override;
    static bool classof(const ExpressionAST *e);

  private:
    const std::vector<std::shared_ptr<VariableExpression>> content;
};

}
#endif // JAPC_CLOSURE_EXPRESSION_H
