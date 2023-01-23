//
// Created by wuser on 18/01/23.
//
#include "japc/AST/file_pointer_expression.h"
using namespace Pascal;
FilePointerExpression::FilePointerExpression(const Location &loc, std::shared_ptr<ExpressionAST> varExpre,
                      std::shared_ptr<TypeDeclaration> typeDeclaration)
    : pointer(varExpre), AddressableExpression(loc, ExpressionType::TYPE_FILE_POINTER_EXPRE, typeDeclaration)
{
}
llvm::Value* FilePointerExpression::getAddress()
{
}
void FilePointerExpression::accept(ExpressionVisitor &visitor)
{
    pointer->accept(visitor);
    visitor.visit(this);
}
bool FilePointerExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_FILE_POINTER_EXPRE;
}