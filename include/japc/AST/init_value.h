//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_INIT_VALUE_H
#define JAPC_INIT_VALUE_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class InitValue : public ExpressionAST
{
  public:
    InitValue(const Location &loc, const std::vector<std::shared_ptr<ExpressionAST>> &v);
    llvm::Value *codeGen() override;
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override;

  private:
    std::vector<std::shared_ptr<ExpressionAST>> values;
};

} // namespace Pascal
#endif // JAPC_INIT_VALUE_H
