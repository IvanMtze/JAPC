//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_PROTOTYPE_EXPRESSION_H
#define JAPC_PROTOTYPE_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/function.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{
class PrototypeExpression : public ExpressionAST
{
  public:
    PrototypeExpression(const Location &loc, const std::string &name,
                        const std::vector<std::shared_ptr<VariableDefinition>> args,
                        std::shared_ptr<TypeDeclaration> resultType);
    llvm::Function *create(const std::string &namePrefix);
    void createArgumentsAlloca();
    void addExtraArgsFirst(std::vector<std::shared_ptr<VariableDefinition>> vars);
    bool operator==(const PrototypeExpression &rhs) const;
    bool isMatchWithoutClosure(const PrototypeExpression *rhs) const;
    static bool isClassOf(ExpressionAST *expressionAst);
    std::shared_ptr<llvm::Function> getLlvmFunction() const;
    std::string getName() const;
    const std::vector<std::shared_ptr<VariableDefinition>> getArgs();
    bool isForwarded() const;
    void setIsForward(bool forward);
    void setFunction(std::shared_ptr<Function> function);
    std::shared_ptr<Function> getFunction() const;

  private:
    std::string name;
    std::vector<std::shared_ptr<VariableDefinition>> args;
    std::shared_ptr<Function> function;
    bool isForward;
    std::shared_ptr<llvm::Function> llvmFunction;
};

} // namespace Pascal
#endif // JAPC_PROTOTYPE_EXPRESSION_H
