//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_FOR_EXPRESSION_H
#define JAPC_FOR_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/variable_expression.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal
{
class ForExpression : public ExpressionAST
{
  public:
    ForExpression(const Location &loc, std::shared_ptr<VariableExpression> var, std::shared_ptr<ExpressionAST> start,
                  std::shared_ptr<ExpressionAST> end, bool down, std::shared_ptr<ExpressionAST> body);
    ForExpression(const Location &loc, std::shared_ptr<VariableExpression> var, std::shared_ptr<ExpressionAST> start,
                  std::shared_ptr<ExpressionAST> body);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    static bool classof(const ExpressionAST *expressionAst);
    void accept(ExpressionVisitor &visitor) override;
    bool isStepDown() const;
    void setStepDown(bool stepDown);
    const std::shared_ptr<VariableExpression> &getVariable() const;
    void setVariable(const std::shared_ptr<VariableExpression> &variable);
    const std::shared_ptr<ExpressionAST> &getStart() const;
    void setStart(const std::shared_ptr<ExpressionAST> &start);
    const std::shared_ptr<ExpressionAST> &getEnd() const;
    void setEnd(const std::shared_ptr<ExpressionAST> &end);
    const std::shared_ptr<ExpressionAST> &getBody() const;
    void setBody(const std::shared_ptr<ExpressionAST> &body);

  private:
    llvm::Value *forInGen();
    bool stepDown;
    std::shared_ptr<VariableExpression> variable;
    std::shared_ptr<ExpressionAST> start;
    std::shared_ptr<ExpressionAST> end;
    std::shared_ptr<ExpressionAST> body;
};

} // namespace Pascal
#endif // JAPC_FOR_EXPRESSION_H
