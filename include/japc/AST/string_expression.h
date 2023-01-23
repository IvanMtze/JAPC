//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_STRING_EXPRESSION_H
#define JAPC_STRING_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/addressable_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class StringExpression : public AddressableExpression
{
  public:
    StringExpression(const Location &location, const std::string &value,
                     std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value* codeGen() override;
    llvm::Value* getAddress() override;
    const std::string &getValue() const;
    static bool isClassOf(const ExpressionAST *expressionAst);

  private:
    std::string value;
};
}
#endif // JAPC_STRING_EXPRESSION_H
