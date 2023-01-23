//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_ARRAY_EXPRESSION_H
#define JAPC_ARRAY_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/variable_expression.h"
#include "japc/AST/addressable_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class ArrayExpression : public AddressableExpression
{
  public:
    ArrayExpression(const Location &loc, std::shared_ptr<ExpressionAST> v,
                    const std::vector<std::shared_ptr<ExpressionAST>> inds,
                    const std::vector<std::shared_ptr<RangeDeclaration>> rangeDeclaration,
                    std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value* getAddress() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &v) override;
    const std::shared_ptr<VariableExpression> &getExpression() const;
    const std::vector<std::shared_ptr<ExpressionAST>> &getIndices() const;
    const std::vector<std::shared_ptr<RangeDeclaration>> &getRanges() const;
    const std::vector<size_t> &getIndexmul() const;

  private:
    std::shared_ptr<VariableExpression> expression;
    std::vector<std::shared_ptr<ExpressionAST>> indices;
    std::vector<std::shared_ptr<RangeDeclaration>> ranges;
    std::vector<size_t> indexmul;
};

}
#endif // JAPC_ARRAY_EXPRESSION_H
