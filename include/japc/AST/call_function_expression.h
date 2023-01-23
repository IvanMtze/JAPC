//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_CALL_FUNCTION_EXPRESSION_H
#define JAPC_CALL_FUNCTION_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/prototype_expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal{
class CallFunctExpression : public ExpressionAST
{
  public:
    CallFunctExpression(const Location &w, std::shared_ptr<ExpressionAST> calle,
                        std::vector<std::shared_ptr<ExpressionAST>> &args,
                        std::shared_ptr<PrototypeExpression> prototype);
    llvm::Value* codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
    const std::shared_ptr<PrototypeExpression> getPrototype();
    std::shared_ptr<ExpressionAST> getCallee() const;
    std::vector<std::shared_ptr<ExpressionAST>> getArgs();
    void accept(ExpressionVisitor &visitor) override;

  private:
    const std::shared_ptr<PrototypeExpression> prototypeExpression;
    std::shared_ptr<ExpressionAST> callee;
    std::vector<std::shared_ptr<ExpressionAST>> args;
};

}
#endif // JAPC_CALL_FUNCTION_EXPRESSION_H
