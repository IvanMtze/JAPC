//
// Created by wuser on 18/01/23.
//

#include "japc/AST/addressable_expression.h"

using namespace Pascal;
llvm::Value *Pascal::makeAddressable(ExpressionAST *e)
{
    if(auto ea = llvm::dyn_cast<AddressableExpression>(e)){
        llvm::Value* v = ea->getAddress();
        assert(v && "Expect addressable object to have address");
        return v;
    }
    llvm::Value* store = e->codeGen();
    if (store->getType()->isPointerTy())
    {
        return store;
    }

    llvm::Value* v = createTempAlloca(e->getTypeDeclaration().get());
    assert(v && "Expect address to be non-zero");
    builder.CreateStore(store, v);
    return v;
}
AddressableExpression::AddressableExpression(const Location &location, ExpressionType expressionType,
                      std::shared_ptr<TypeDeclaration> typeDeclaration)
    : ExpressionAST(location, expressionType, typeDeclaration)
{
}
llvm::Value *AddressableExpression::codeGen()
{
    llvm::Value* value = getAddress();
    assert(value && "Expected to get an address");
    return builder.CreateLoad(value, getName());
}

llvm::Value *AddressableExpression::getAddress()
{
    assert(0 && "What?!");
    return 0;
}
const std::string AddressableExpression::getName() const
{
    return "";
}

bool AddressableExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() >= ExpressionType::TYPE_ADDRESSABLE_EXPRE &&
           expressionAst->getExpressionType() <= ExpressionType::TYPE_LAST_ADDRESSABLE;
}
bool AddressableExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}