//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_WRITE_EXPRESSION_H
#define JAPC_WRITE_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/variable_expression.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class WriteExpression : public ExpressionAST
{
  public:
    struct wargs
    {
        wargs() : expr(0), width(0), precision(0)
        {
        }
        std::shared_ptr<ExpressionAST> expr;
        std::shared_ptr<ExpressionAST> width;
        std::shared_ptr<ExpressionAST> precision;
    };

    WriteExpression(const Location &loc, std::shared_ptr<VariableExpression> out, const std::vector<wargs> &args,
                    bool isWriteToLin);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &expressionVisitor) override;

  private:
    std::shared_ptr<VariableExpression> output;
    std::vector<wargs> args;
    bool isWriteToLn;
};

} // namespace Pascal
#endif // JAPC_WRITE_EXPRESSION_H
