#include "japc/AST/binary_expression.h"
using namespace Pascal;

static llvm::Value *Pascal::callStrCat(std::shared_ptr<ExpressionAST> lhs, std::shared_ptr<ExpressionAST> rhs)
{
    // llvm::Value* rv = makeStringFromExpr(rhs, rhs->getTypeDeclaration());
    // llvm::Value* lv = makeStringFromExpr(lhs, lhs->getTypeDeclaration());
}
static int Pascal::stringishScore(ExpressionAST *expression)
{
    if (llvm::isa<CharExpression>(expression) || llvm::isa<CharDeclaration>(expression->getTypeDeclaration().get()))
    {
        return 1;
    }
    if (llvm::isa<StringExpression>(expression))
    {
        return 2;
    }
    if (llvm::isa<StringDeclaration>(expression->getTypeDeclaration().get()))
    {
        return 3;
    }
    return 0;
}
static bool Pascal::bothAreString(ExpressionAST *left, ExpressionAST *right)
{
    int lScore = stringishScore(left);
    int rScore = stringishScore(right);
    return lScore && rScore && (lScore + rScore) >= 2;
}

static llvm::Value *Pascal::shortCtAnd(ExpressionAST *left, ExpressionAST *right)
{
    llvm::Value *l = left->codeGen();
    llvm::BasicBlock *originBlock = builder.GetInsertBlock();
    llvm::Function *theFunction = originBlock->getParent();
    llvm::BasicBlock *trueBB = llvm::BasicBlock::Create(theContext, "true", theFunction);
    llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(theContext, "merge", theFunction);
    llvm::Value *bfalse = llvm::ConstantInt::get(getBooleanType()->getLtype(), 0);

    llvm::Value *conditionLeft = builder.CreateICmpNE(l, bfalse, "conditionLeft");
    builder.CreateCondBr(conditionLeft, trueBB, mergeBB);
    builder.SetInsertPoint(trueBB);
    llvm::Value *r = right->codeGen();
    llvm::Value *conditionRight = builder.CreateICmpNE(r, bfalse, "conditionRight");

    builder.CreateBr(mergeBB);
    builder.SetInsertPoint(mergeBB);
    llvm::PHINode *phi = builder.CreatePHI(getBooleanType()->getLtype(), 2, "phi");
    phi->addIncoming(conditionRight, originBlock);
    phi->addIncoming(conditionLeft, trueBB);
    return phi;
}
llvm::Value *Pascal::makeStrCompare(TokenType oper, llvm::Value *v)
{
    llvm::Constant *zero = llvm::ConstantInt::get(getIntegerType()->getLtype(), 0);
    switch (oper)
    {
    case TokenType::SYMBOL_EQUAL:
        return builder.CreateICmpEQ(v, zero, "eq");
    case TokenType::SYMBOL_GREATER_LESS_THAN:
        return builder.CreateICmpNE(v, zero, "ne");
    case TokenType::SYMBOL_GREATER_EQUAL_THAN:
        return builder.CreateICmpSGE(v, zero, "ge");
    case TokenType::SYMBOL_GREATER_THAN:
        return builder.CreateICmpSGT(v, zero, "gt");
    case TokenType::SYMBOL_LESS_THAN:
        return builder.CreateICmpSLT(v, zero, "lt");
    case TokenType::SYMBOL_LESS_EQUAL_THAN:
        return builder.CreateICmpSLE(v, zero, "lE");
    default:
        assert(0 && "Unexpected operator for char array");
    }
}
static llvm::Value *Pascal::shortCtOr(ExpressionAST *left, ExpressionAST *right)
{
    llvm::Value *l = left->codeGen();
    llvm::BasicBlock *originBlock = builder.GetInsertBlock();
    llvm::Function *theFunction = originBlock->getParent();
    llvm::BasicBlock *falseBB = llvm::BasicBlock::Create(theContext, "false", theFunction);
    llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(theContext, "merge", theFunction);
    llvm::Value *bfalse = llvm::ConstantInt::get(getBooleanType()->getLtype(), 0);
    llvm::Value *btrue = llvm::ConstantInt::get(getBooleanType()->getLtype(), 1);

    llvm::Value *condl = builder.CreateICmpEQ(l, bfalse, "condl");
    builder.CreateCondBr(condl, falseBB, mergeBB);
    builder.SetInsertPoint(falseBB);
    llvm::Value *r = right->codeGen();
    llvm::Value *condr = builder.CreateICmpNE(r, bfalse, "condr");

    builder.CreateBr(mergeBB);

    builder.SetInsertPoint(mergeBB);
    llvm::PHINode *phi = builder.CreatePHI(getBooleanType()->getLtype(), 2, "phi");
    phi->addIncoming(btrue, originBlock);
    phi->addIncoming(condr, falseBB);
    return nullptr;
}
llvm::Value *makeStrCompare(TokenType oper, llvm::Value *v)
{
    llvm::Constant *zero = llvm::ConstantInt::get(getIntegerType()->getLtype(), 0);
    switch (oper)
    {
    case TokenType::SYMBOL_EQUAL:
        return builder.CreateICmpEQ(v, zero, "eq");
    case TokenType::SYMBOL_LESS_GREATER_THAN:
        return builder.CreateICmpNE(v, zero, "ne");
    case TokenType::SYMBOL_GREATER_EQUAL_THAN:
        return builder.CreateICmpSGE(v, zero, "ge");
    case TokenType::SYMBOL_LESS_EQUAL_THAN:
        return builder.CreateICmpSLE(v, zero, "le");
    case TokenType::SYMBOL_GREATER_THAN:
        return builder.CreateICmpSGT(v, zero, "gt");
    case TokenType::SYMBOL_LESS_THAN:
        return builder.CreateICmpSLT(v, zero, "lt");
    default:
        assert(0 && "Invalid operand for char arrays");
    }
}

llvm::Value *BinaryExpression::codeGen()
{
    // At least one of the sides is a set expression
    if (llvm::isa<SetExpression>(rhs.get()) || llvm::isa<SetExpression>(lhs.get()) ||
        (rhs->getTypeDeclaration() && llvm::isa<SetDeclaration>(rhs->getTypeDeclaration().get())) ||
        (lhs->getTypeDeclaration() && llvm::isa<SetDeclaration>(lhs->getTypeDeclaration().get())))
    {
        return setCodeGen();
    }
    // Both of them are strings
    if (bothAreString(lhs.get(), rhs.get()))
    {
        if (oper.getTokenType() == TokenType::SYMBOL_PLUS)
        {
            return callStrCat(lhs, rhs);
        }
        if (!llvm::isa<CharDeclaration>(lhs->getTypeDeclaration().get()) ||
            !llvm::isa<CharDeclaration>(rhs->getTypeDeclaration().get()))
        {
            return Pascal::makeStrCompare(oper.getTokenType(), callStrFunc("Compare"));
        }
    }
    //  Both are arrays
    if (llvm::isa<ArrayDeclaration>(rhs->getTypeDeclaration().get()) &&
        llvm::isa<ArrayDeclaration>(lhs->getTypeDeclaration().get()))
    {
        if (llvm::isa<CharDeclaration>(lhs->getTypeDeclaration()->getSubtype().get()) &&
            llvm::isa<CharDeclaration>(rhs->getTypeDeclaration()->getSubtype().get()))
        {
            ArrayDeclaration *ar = llvm::dyn_cast<ArrayDeclaration>(rhs->getTypeDeclaration().get());
            ArrayDeclaration *al = llvm::dyn_cast<ArrayDeclaration>(lhs->getTypeDeclaration().get());
            std::vector<std::shared_ptr<RangeDeclaration>> rr = ar->getRanges();
            std::vector<std::shared_ptr<RangeDeclaration>> rl = al->getRanges();
            if (rr.size() == 1 && rl.size() == 1 && rr[0]->getSize() == rl[0]->getSize())
            {
                return makeStrCompare(oper.getTokenType(), callArrFunc("compare", rr[0]->getSize()));
            }
        }
    }
    switch (oper.getTokenType())
    {
    case TokenType::SYMBOL_AND_THEN:
        return shortCtAnd(lhs.get(), rhs.get());
    case TokenType::SYMBOL_OR_ELSE:
        return shortCtOr(lhs.get(), rhs.get());
    default:
        break;
    }

    llvm::Value *left = lhs->codeGen();
    llvm::Value *right = rhs->codeGen();

    llvm::Type *ltype = left->getType();
    llvm::Type *rtype = right->getType();

    assert(rtype == ltype && "Expected same types");
    if (rtype->isIntegerTy() || rtype->isPointerTy())
    {
        switch (oper.getTokenType())
        {
        case TokenType::SYMBOL_EQUAL:
            return builder.CreateICmpEQ(left, right, "eq");
        case TokenType::SYMBOL_LESS_GREATER_THAN:
            return builder.CreateICmpNE(left, right, "ne");
        default:
            break;
        }
    }
    if (rtype->isIntegerTy())
    {
        bool isUnsigned = rhs->getTypeDeclaration()->isUnsigned();
        if (auto v =
                integerBinaryExpression(left, right, oper.getTokenType(), this->getTypeDeclaration().get(), isUnsigned))
        {
            return v;
        }
        assert(0 && "Unkown token in bin op");
    }
    if (rtype->isDoubleTy())
    {
        if (auto v = doubleBinaryExpression(left, right, oper.getTokenType(), this->getTypeDeclaration().get()))
        {
            return v;
        }
    }
    return 0;
}
std::shared_ptr<TypeDeclaration> BinaryExpression::getTypeDeclaration() const
{
    return ExpressionAST::getTypeDeclaration();
}
llvm::Value *BinaryExpression::setCodeGen()
{
    if (lhs->getTypeDeclaration() && lhs->getTypeDeclaration()->isIntegral() &&
        oper.getTokenType() == TokenType::SYMBOL_IN)
    {
        llvm::Value *l = lhs->codeGen();
        llvm::Value *setV = 0;//makeAddressable(rhs.get());
        TypeDeclaration *type = rhs->getTypeDeclaration().get();
        int start = type->getRange()->getStart();
        l = builder.CreateZExt(l, getIntegerType()->getLtype(), "zext.l");
        l = builder.CreateSub(l, llvm::ConstantInt::get(getIntegerType()->getLtype(), start));
        llvm::Value *index;
        if (llvm::dyn_cast<SetDeclaration>(type)->SetWords() > 1)
        {
            index = builder.CreateLShr(l, llvm::ConstantInt::get(getIntegerType()->getLtype(), __SET_POW_TO_BITS__));
        }
        else
        {
            index = llvm::ConstantInt::get(getIntegerType()->getLtype(), 0);
        }
        llvm::Value *offset = builder.CreateAnd(l, llvm::ConstantInt::get(getIntegerType()->getLtype(), __SET_MASK__));
        std::vector<llvm::Value *> ind{llvm::ConstantInt::get(getIntegerType()->getLtype(), 0), index};
        llvm::Value *bitsetAddr = builder.CreateGEP(setV, ind, "valueindex");

        llvm::Value *bitset = builder.CreateLoad(bitsetAddr, "bitsetaddr");
        llvm::Value *bit = builder.CreateLShr(bitset, offset);
        return builder.CreateTrunc(bit, getBooleanType()->getLtype());
    }

    if (llvm::isa<SetExpression>(lhs.get()) ||
        (lhs->getTypeDeclaration() && llvm::isa<SetDeclaration>(lhs->getTypeDeclaration().get())))
    {
        switch (oper.getTokenType())
        {
        case TokenType::SYMBOL_MINUS:
            return callSetFunc("Diff", true);

        case TokenType::SYMBOL_PLUS:
            return callSetFunc("Union", true);

        case TokenType::SYMBOL_STAR:
            return callSetFunc("Intersect", true);

        case TokenType::SYMBOL_EQUAL:
            return callSetFunc("Equal", false);

        case TokenType::SYMBOL_LESS_GREATER_THAN:
            return builder.CreateNot(callSetFunc("Equal", false), "notEqual");

        case TokenType::SYMBOL_LESS_EQUAL_THAN:
            return callSetFunc("Contains", false);

        case TokenType::SYMBOL_GREATER_LESS_THAN:
            return callSetFunc("SymDiff", true);

        case TokenType::SYMBOL_GREATER_EQUAL_THAN: {
            // Swap left<->right sides
            std::shared_ptr<ExpressionAST> tmp = rhs;
            rhs = lhs;
            lhs = tmp;
            llvm::Value *v = 0; // CallSetFunc("Contains", false);
            // Set it back
            lhs = rhs;
            rhs = tmp;
            return v;
        }

        case TokenType::SYMBOL_GREATER_THAN:
            return builder.CreateNot(callSetFunc("Contains", false));

        case TokenType::SYMBOL_LESS_THAN: {
            // Swap left<->right sides
            std::shared_ptr<ExpressionAST> tmp = rhs;
            rhs = lhs;
            lhs = tmp;
            llvm::Value *v = builder.CreateNot(callSetFunc("Contains", false));
            // Set it back
            lhs = rhs;
            rhs = tmp;
            return v;
        }
        }
        return nullptr;
    }
}
BinaryExpression::BinaryExpression(Token tok, std::shared_ptr<ExpressionAST> lhs, std::shared_ptr<ExpressionAST> rhs)
    : ExpressionAST(tok.getTokenPos(), ExpressionType::TYPE_BINARY_EXPRE), oper(tok), lhs(lhs), rhs(rhs)
{
}
llvm::Value *BinaryExpression::inlineSetFunc(const std::string &name)
{
    return ExpressionAST::codeGen();
}
llvm::Value *BinaryExpression::callSetFunc(const std::string &name, bool resultTypeIsSet)
{
    return ExpressionAST::codeGen();
}
llvm::Value *BinaryExpression::callStrFunc(const std::string &name)
{
    return ExpressionAST::codeGen();
}
llvm::Value *BinaryExpression::callArrFunc(const std::string &name, size_t size)
{
    return ExpressionAST::codeGen();
}
Token &BinaryExpression::getOper()
{
    return oper;
}
std::shared_ptr<ExpressionAST> &BinaryExpression::getLhs()
{
    return lhs;
}
std::shared_ptr<ExpressionAST> &BinaryExpression::getRhs()
{
    return rhs;
}
bool BinaryExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_BINARY_EXPRE;
}
void BinaryExpression::accept(ExpressionVisitor &visitor)
{
    rhs->accept(visitor);
    lhs->accept(visitor);
    visitor.visit(this);
}
bool BinaryExpression::classof(const ExpressionAST *expressionAst)
{
    return isClassOf(expressionAst);
}