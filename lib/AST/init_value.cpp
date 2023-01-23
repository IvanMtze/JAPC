//
// Created by wuser on 18/01/23.
//

#include "japc/AST/init_value.h"
using namespace Pascal;

InitValue::InitValue(const Location &loc, const std::vector<std::shared_ptr<ExpressionAST>> &v)
    : values(v), ExpressionAST(loc, ExpressionType::TYPE_INIT_VALUE)
{
}
llvm::Value* InitValue::codeGen()
{
    return ExpressionAST::codeGen();
}

std::shared_ptr<TypeDeclaration> InitValue::getTypeDeclaration() const
{
    return values[0]->getTypeDeclaration();
}