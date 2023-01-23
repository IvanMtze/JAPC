//
// Created by wuser on 15/10/22.
//
#include "japc/AST/expression.h"
using namespace Pascal;
const size_t _MIN_ALIGN_ = 4;
llvm::Value *Pascal::powerInt(llvm::Value *base, llvm::Value *exp, TypeDeclaration *ty)
{
    llvm::BasicBlock *originBlock = builder.GetInsertBlock();
    llvm::Function *theFunction = originBlock->getParent();
    llvm::BasicBlock *expNegBB = llvm::BasicBlock::Create(theContext, "expNeg", theFunction);
    llvm::BasicBlock *oddEvenBB = llvm::BasicBlock::Create(theContext, "oddeven", theFunction);
    llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(theContext, "loop", theFunction);
    llvm::BasicBlock *moreBB = llvm::BasicBlock::Create(theContext, "more", theFunction);
    llvm::BasicBlock *moreOddBB = llvm::BasicBlock::Create(theContext, "moreOdd", theFunction);
    llvm::BasicBlock *moreEvenBB = llvm::BasicBlock::Create(theContext, "moreEven", theFunction);
    llvm::BasicBlock *moreMergeBB = llvm::BasicBlock::Create(theContext, "moreMerge", theFunction);
    llvm::BasicBlock *doneBB = llvm::BasicBlock::Create(theContext, "powdone", theFunction);

    llvm::Constant *one = llvm::ConstantInt::get(getIntegerType()->getLtype(), 1);
    llvm::Constant *zero = llvm::ConstantInt::get(getIntegerType()->getLtype(), 0);
    llvm::Value *res = createTempAlloca(ty);
    builder.CreateStore(one, res);

    llvm::Value *expneg = builder.CreateICmpSLT(exp, zero, "expneg");
    builder.CreateCondBr(expneg, expNegBB, loopBB);
    // e < 0 case:
    builder.SetInsertPoint(expNegBB);
    llvm::Value *bIsMinus1 =
        builder.CreateICmpEQ(base, llvm::ConstantInt::get(getIntegerType()->getLtype(), -1), "isNeg1");
    builder.CreateCondBr(bIsMinus1, oddEvenBB, doneBB);

    builder.SetInsertPoint(oddEvenBB);
    llvm::Value *isOdd = builder.CreateICmpEQ(builder.CreateAnd(exp, one, "odd"), one, "isOdd");
    // b = -1 && e & 1 => return b;
    builder.CreateCondBr(isOdd, doneBB, loopBB);

    builder.SetInsertPoint(loopBB);
    llvm::PHINode *phiLoop = builder.CreatePHI(ty->getLtype(), 2, "phi");
    phiLoop->addIncoming(exp, originBlock);
    phiLoop->addIncoming(exp, oddEvenBB);
    llvm::Value *expgt0 = builder.CreateICmpSLE(phiLoop, zero, "expgt0");
    llvm::Value *curVal = builder.CreateLoad(res);
    builder.CreateCondBr(expgt0, doneBB, moreBB);

    builder.SetInsertPoint(moreBB);
    llvm::Value *isOdd1 = builder.CreateICmpEQ(builder.CreateAnd(exp, one, "odd1"), one, "isOdd1");
    builder.CreateCondBr(isOdd1, moreOddBB, moreEvenBB);

    builder.SetInsertPoint(moreOddBB);
    llvm::Value *valOdd = builder.CreateMul(curVal, base);
    builder.CreateBr(moreMergeBB);

    builder.SetInsertPoint(moreEvenBB);
    llvm::Value *valEven = builder.CreateMul(curVal, builder.CreateMul(base, base));
    builder.CreateBr(moreMergeBB);

    builder.SetInsertPoint(moreMergeBB);
    llvm::PHINode *phiMerge = builder.CreatePHI(ty->getLtype(), 2, "phi");
    phiMerge->addIncoming(valEven, moreEvenBB);
    phiMerge->addIncoming(valOdd, moreOddBB);
    llvm::PHINode *phiMerge2 = builder.CreatePHI(ty->getLtype(), 2, "phi");
    phiMerge2->addIncoming(llvm::ConstantInt::get(getIntegerType()->getLtype(),2), moreEvenBB);
    phiMerge2->addIncoming(one, moreOddBB);
    builder.CreateStore(phiMerge, res);
    phiLoop->addIncoming(builder.CreateSub(phiLoop, phiMerge2), moreMergeBB);
    builder.CreateBr(loopBB);

    builder.SetInsertPoint(doneBB);
    llvm::PHINode *phi = builder.CreatePHI(ty->getLtype(), 3, "phi");
    // e < 0, b != -1
    phi->addIncoming(zero, expNegBB);
    // e < 0, e is odd, b = -1
    phi->addIncoming(base, oddEvenBB);
    // all other cases.
    phi->addIncoming(curVal, loopBB);
    return phi;
}

llvm::Value *Pascal::integerBinaryExpression(llvm::Value *leftValue, llvm::Value *rightValue, TokenType &tokenType,
                                             TypeDeclaration *type, bool isUnsigned)
{
    switch (tokenType)
    {
    case TokenType::SYMBOL_PLUS:
        return builder.CreateAdd(leftValue, rightValue, "addtmp");
    case TokenType::SYMBOL_MINUS:
        return builder.CreateSub(leftValue, rightValue, "subtmp");
    case TokenType::SYMBOL_STAR:
        return builder.CreateMul(leftValue, rightValue, "multmp");
    case TokenType::SYMBOL_DIV:
        return builder.CreateSDiv(leftValue, rightValue, "divtmp");
    case TokenType::SYMBOL_MOD:
        return builder.CreateSRem(leftValue, rightValue, "modtmp");
    // TODO: add support for shl, shr and xor
    case TokenType::SYMBOL_AND:
        return builder.CreateAnd(leftValue, rightValue, "andtmp");
    case TokenType::SYMBOL_OR:
        return builder.CreateOr(leftValue, rightValue, "ortmp");
    case TokenType::SYMBOL_POW:
        return powerInt(leftValue, rightValue, type);
    case TokenType::SYMBOL_LESS_THAN:
        if (isUnsigned)
        {
            return builder.CreateICmpULT(leftValue, rightValue, "lt");
        }
        return builder.CreateICmpSLT(leftValue, rightValue, "lt");
    case TokenType::SYMBOL_LESS_EQUAL_THAN:
        if (isUnsigned)
        {
            return builder.CreateICmpULE(leftValue, rightValue, "le");
        }
        return builder.CreateICmpULE(leftValue, rightValue, "le");
    case TokenType::SYMBOL_GREATER_THAN:
        if (isUnsigned)
        {
            return builder.CreateICmpUGT(leftValue, rightValue, "gt");
        }
        return builder.CreateICmpUGT(leftValue, rightValue, "gt");
    case TokenType::SYMBOL_GREATER_EQUAL_THAN:
        if (isUnsigned)
        {
            return builder.CreateICmpUGE(leftValue, rightValue, "ge");
        }
        return builder.CreateICmpSGE(leftValue, rightValue, "ge");
    }
}
llvm::AllocaInst *Pascal::createTempAlloca(TypeDeclaration *ty)
{
    llvm::Function *fn = builder.GetInsertBlock()->getParent();
    return createNamedAlloca(fn, ty, "tmp");
}
llvm::AllocaInst *Pascal::createNamedAlloca(llvm::Function *fn, TypeDeclaration *ty, const std::string &name)
{
    llvm::BasicBlock *bb = builder.GetInsertBlock();
    llvm::BasicBlock::iterator ip = builder.GetInsertPoint();

    llvm::IRBuilder<> bld(&fn->getEntryBlock(), fn->getEntryBlock().begin());

    assert(ty && "Must have type passed in");
    llvm::Type *type = ty->getLtype();

    llvm::AllocaInst *a = bld.CreateAlloca(type, 0, name);
    size_t align = std::max(ty->getAllignSize(), _MIN_ALIGN_);
    if (a->getAlignment() < align)
    {
        a->setAlignment(llvm::Align(align));
    }

    // Now go back to where we were...
    builder.SetInsertPoint(bb, ip);
    return a;
    return nullptr;
}

// Expression Class
ExpressionAST::ExpressionAST(const Location &location, ExpressionType expressionType)
    : location(location), expressionType(expressionType), typeDeclaration(0)
{
}
ExpressionAST::ExpressionAST(const Location &location, ExpressionType expressionType,
              std::shared_ptr<TypeDeclaration> typeDeclaration)
    : location(location), expressionType(expressionType), typeDeclaration(typeDeclaration)
{
}
void ExpressionAST::ensureSized() const
{
}

void ExpressionAST::accept(ExpressionVisitor &visitor)
{
    visitor.visit(this);
}
llvm::Value *ExpressionAST::codeGen()
{
    assert(0 && "WHAT?!");
    return 0;
};
ExpressionAST::ExpressionType ExpressionAST::getExpressionType() const
{
    return expressionType;
}
void ExpressionAST::setTypeDeclaration(std::shared_ptr<TypeDeclaration> typeDeclaration)
{
    this->typeDeclaration = typeDeclaration;
}
std::shared_ptr<TypeDeclaration> ExpressionAST::getTypeDeclaration() const
{
    return typeDeclaration;
}
const Location ExpressionAST::getLocation() const
{
    return location;
}

// UTILS

static llvm::BasicBlock *CreateGotoTarget(int label)
{
    std::string name = std::to_string(label);
    if (Label *lab = labels.find(name))
    {
        return lab->getBasicBlock();
    }
    llvm::Function *theFunction = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(theContext, name, theFunction);
    labels.insert(name, new Label(bb, label));
    return bb;
}

// other
