//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_REAL_EXPRESSION_H
#define JAPC_REAL_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal
{
class RealExpression : public ExpressionAST
{
  public:
    RealExpression(const Location &loc, double value, std::shared_ptr<TypeDeclaration> typeDec);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);

  private:
    double value;
};
} // namespace Pascal
#endif // JAPC_REAL_EXPRESSION_H
