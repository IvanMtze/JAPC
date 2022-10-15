//
// Created by wuser on 13/10/22.
//

#ifndef JAPC_BUILTIN_H
#define JAPC_BUILTIN_H

#include "japc/AST/expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

class ExpressionAST;
namespace Pascal
{
class FunctionBaseBuiltin
{
  public:
    FunctionBaseBuiltin(const std::vector<std::shared_ptr<ExpressionAST>> &args) : args(args)
    {
    }
    virtual std::shared_ptr<llvm::Value> codeGen(llvm::IRBuilder<> &builder) = 0;
    virtual std::shared_ptr<TypeDeclaration> getType() const = 0;
    virtual bool semantics() = 0;
    virtual void accept(ExpressionVisitor &expressionVisitor);

  protected:
    std::vector<std::shared_ptr<ExpressionAST>> args;
};

} // namespace Pascal
#endif // JAPC_BUILTIN_H
