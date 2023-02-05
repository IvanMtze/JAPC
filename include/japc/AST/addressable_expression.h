//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_ADDRESSABLE_EXPRESSION_H
#define JAPC_ADDRESSABLE_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
llvm::Value *makeAddressable(ExpressionAST *e);

class AddressableExpression : public ExpressionAST
{
  public:
    AddressableExpression(const Location &location, ExpressionType expressionType,
                          std::shared_ptr<TypeDeclaration> typeDeclaration);
    virtual llvm::Value *getAddress();
    llvm::Value *codeGen() override;
    virtual const std::string getName() const;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
};

} // namespace Pascal
#endif // JAPC_ADDRESSABLE_EXPRESSION_H
