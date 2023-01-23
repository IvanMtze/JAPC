//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_VARIABLE_DECLARATION_EXPRESSION_H
#define JAPC_VARIABLE_DECLARATION_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/function.h"
#include "japc/AST/type.h"
#include "japc/AST/variable_definition.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class Function;
class VariableDeclarationExpression : public ExpressionAST
{
  public:
    VariableDeclarationExpression(const Location &loc, std::vector<std::shared_ptr<VariableDefinition>> vars);
    llvm::Value *codeGen() override;
    void setFunction(std::shared_ptr<Function> function);
    static bool isClassOf(const ExpressionAST *expressionAst);
    const std::vector<std::shared_ptr<VariableDefinition>> getVars();

  private:
    std::vector<std::shared_ptr<VariableDefinition>> vars;
    std::shared_ptr<Function> function;
};
} // namespace Pascal
#endif // JAPC_VARIABLE_DECLARATION_EXPRESSION_H
