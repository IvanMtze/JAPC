//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_RANGE_CHECK_EXPRESSION_H
#define JAPC_RANGE_CHECK_EXPRESSION_H
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/range_reduce_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>
namespace Pascal{

class RangeCheckExpression : public RangeReduceExpression
{
    RangeCheckExpression(ExpressionAST *expressionAst, RangeDeclaration *rangeDecl);
    llvm::Value* codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
};
}
#endif // JAPC_RANGE_CHECK_EXPRESSION_H
