//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_READ_EXPRESSION_H
#define JAPC_READ_EXPRESSION_H
#include "japc/AST/addressable_expression.h"
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class ReadExpression : public ExpressionAST
{
  public:
    ReadExpression(const Location &w, AddressableExpression *fi, const std::vector<std::shared_ptr<ExpressionAST>> &a,
                   bool isLn);
    llvm::Value *codeGen() override;
    static bool classof(const ExpressionAST *e);
    void accept(ExpressionVisitor &v) override;

  private:
    std::shared_ptr<AddressableExpression> file;
    std::vector<std::shared_ptr<ExpressionAST>> args;
    bool isReadln;
};

} // namespace Pascal
#endif // JAPC_READ_EXPRESSION_H
