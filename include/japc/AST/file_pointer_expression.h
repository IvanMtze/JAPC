//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_FILE_POINTER_EXPRESSION_H
#define JAPC_FILE_POINTER_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/addressable_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal{
class FilePointerExpression : public AddressableExpression
{
  public:
    FilePointerExpression(const Location &loc, std::shared_ptr<ExpressionAST> varExpre,
                          std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value* getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;

  private:
    std::shared_ptr<ExpressionAST> pointer;
};
}
#endif // JAPC_FILE_POINTER_EXPRESSION_H
