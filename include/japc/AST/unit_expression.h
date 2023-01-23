//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_UNIT_EXPRESSION_H
#define JAPC_UNIT_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/function.h"
#include "japc/AST/prototype_expression.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"

namespace Pascal{
static std::vector<std::shared_ptr<Function>> unitInit;
static void builtUnitInitList();
class UnitExpression : public ExpressionAST
{
  public:
    UnitExpression(const Location &loc, const std::vector<std::shared_ptr<ExpressionAST>> &c,
                   std::shared_ptr<Function> init);
    llvm::Value* codeGen() override;
    static bool classof(const ExpressionAST *e);
    void accept(ExpressionVisitor &v) override;

  private:
    std::shared_ptr<Function> initFunc;
    std::vector<std::shared_ptr<ExpressionAST>> code;
};

}
#endif // JAPC_UNIT_EXPRESSION_H
