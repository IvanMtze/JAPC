//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_INTEGER_EXPRESSION_H
#define JAPC_INTEGER_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class IntegerExpression : public ExpressionAST
{
  public:
    IntegerExpression(const Location &loc, int64_t value, std::shared_ptr<TypeDeclaration> typeDec);
    IntegerExpression(const Location &loc, ExpressionType expressionType, int64_t value,
                      std::shared_ptr<TypeDeclaration> typeDec);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
    int64_t getValue();

  protected:
    int64_t value;
};
} // namespace Pascal
#endif // JAPC_INTEGER_EXPRESSION_H
