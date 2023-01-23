//
// Created by wuser on 18/01/23.
//
#include "japc/AST/sizeof_expression.h"
using namespace Pascal;
SizeOfExpression::SizeOfExpression(const Location &loc, std::shared_ptr<TypeDeclaration> typeDeclaration)
    : ExpressionAST(loc, ExpressionType::TYPE_SIZE_OF_EXPRE, typeDeclaration)
{
}
llvm::Value *SizeOfExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<TypeDeclaration> SizeOfExpression::getTypeDeclaration() const
{
    return getIntegerType();
}