//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_CHAR_EXPRESSION_H
#define JAPC_CHAR_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include "japc/AST/integer_expression.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class CharExpression : public IntegerExpression
{
  public:
    CharExpression(const Location &loc, char values, std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
};
} // namespace Pascal
#endif // JAPC_CHAR_EXPRESSION_H
