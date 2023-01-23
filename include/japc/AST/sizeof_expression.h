//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_SIZEOF_EXPRESSION_H
#define JAPC_SIZEOF_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal{
class SizeOfExpression : public ExpressionAST
{
  public:
    SizeOfExpression(const Location &loc, std::shared_ptr<TypeDeclaration> typeDeclaration);
    llvm::Value* codeGen() override;
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override;
    static bool isClassOf(const ExpressionAST *expressionAst)
    {
        return expressionAst->getExpressionType() == ExpressionType::TYPE_SIZE_OF_EXPRE;
    }
};

}
#endif // JAPC_SIZEOF_EXPRESSION_H
