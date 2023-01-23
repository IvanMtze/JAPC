//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_NULL_EXPRESSION_H
#define JAPC_NULL_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class NullExpression : public ExpressionAST
{
  public:
    NullExpression(const Location &loc); // TODO: Add ExpreType
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
};

} // namespace Pascal
#endif // JAPC_NULL_EXPRESSION_H
