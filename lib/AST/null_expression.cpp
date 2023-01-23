//
// Created by wuser on 18/01/23.
//

#include "japc/AST/null_expression.h"

using namespace Pascal;

NullExpression::NullExpression(const Location &loc) : ExpressionAST(loc, ExpressionType::TYPE_NIL_EXPRE)
{
}
llvm::Value *NullExpression::codeGen()
{
    llvm::Type *base = llvm::IntegerType::getInt8Ty(theContext);
    llvm::Type *vp = llvm::PointerType::getUnqual(base);
    return llvm::ConstantPointerNull::get(llvm::dyn_cast<llvm::PointerType>(vp));
}
bool NullExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_NIL_EXPRE;
}
bool NullExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}