//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_FUNCTION_H
#define JAPC_FUNCTION_H


#include "japc/AST/expression.h"
#include "japc/AST/block_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/variable_declaration_expression.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <set>
#include <llvm/IR/Value.h>

namespace Pascal{
class VariableDefinition;
class VariableDeclarationExpression;
class ExpressionAST;
class Function : public ExpressionAST
{
  public:
    Function(const Location &loc, std::shared_ptr<PrototypeExpression> prototype,
             const std::vector<std::shared_ptr<VariableDeclarationExpression>> variablesDecl,
             std::shared_ptr<BlockExpression> block);
    llvm::Function* codeGen() const;
    llvm::Function* codeGen(const std::string &namePrefix);
    const std::shared_ptr<PrototypeExpression> getPrototype() const;
    void addSubFunctions(const std::vector<std::shared_ptr<Function>> subs);
    void setParent(std::shared_ptr<Function> parent);
    const std::shared_ptr<Function> getParent();
    const std::vector<std::shared_ptr<Function>> getSubFunctions();
    void setUsedVars(const std::set<VariableDefinition> &usedVariables);
    const std::set<VariableDefinition> &getUsedVars();
    std::shared_ptr<TypeDeclaration> getReturnType();
    const std::string closureName();
    static bool isClassOf(const ExpressionAST *expressionAst);
    void setEndLoc(Location loc);
    void accept(ExpressionVisitor &v) override;

  private:
    std::shared_ptr<PrototypeExpression> prototype;
    std::vector<std::shared_ptr<VariableDeclarationExpression>> variablesDecl;
    std::shared_ptr<BlockExpression> body;
    std::vector<std::shared_ptr<Function>> subFunctions;
    std::set<VariableDefinition> usedVariables;
    std::shared_ptr<Function> parent;
    std::shared_ptr<TypeDeclaration> returnType;
    Location endLoc;
};
}
#endif // JAPC_FUNCTION_H
