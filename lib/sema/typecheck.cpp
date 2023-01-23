#include "japc/sema/typecheck.h"

using namespace Pascal;

void TypeCheckVisitor::checkBinaryExpr(std::shared_ptr<BinaryExpression> binaryExpression)
{
    const std::shared_ptr<ExpressionAST> leftSideExpr = binaryExpression->getLhs();
    const std::shared_ptr<ExpressionAST> rightSideExpr = binaryExpression->getRhs();
    TokenType oper = binaryExpression->getOper().getTokenType();
    if (oper == TokenType::SYMBOL_IN)
    {
        if (!leftSideExpr->getTypeDeclaration()->isIntegral())
        {
            this->semanticAnalizer->diagnosticsEngine->japc_error_at(
                leftSideExpr->getLocation(), "Left side expression of 'in' expression must be an integral type");
        }

        if (SetExpression *s = llvm::dyn_cast<SetExpression>(binaryExpression->getRhs().get()))
        {
            if (s->getValues().empty())
            {
                llvm::dyn_cast<SetDeclaration>(rightSideExpr->getTypeDeclaration().get())
                    ->updateSubtype(rightSideExpr->getTypeDeclaration());
            }
        }
    }
}
void TypeCheckVisitor::checkAssignExpr(std::shared_ptr<AssignExpression> assignExpression)
{
    std::shared_ptr<TypeDeclaration> leftType = assignExpression->getLhs()->getTypeDeclaration();
    std::shared_ptr<TypeDeclaration> rightType = assignExpression->getRhs()->getTypeDeclaration();
    if (llvm::isa<SetDeclaration>(leftType.get()) && llvm::isa<SetDeclaration>(rightType.get()))
    {
        if (!rightType->getRange())
        {
            llvm::dyn_cast<SetDeclaration>(rightType.get())->updateSubtype(getRangeDeclaration(leftType));
        }
        if (!rightType->getSubtype())
        {
            llvm::dyn_cast<SetDeclaration>(rightType.get())->updateSubtype(leftType->getSubtype());
        }
    }
    if (llvm::isa<PointerDeclaration>(leftType.get()) && llvm::isa<IntegerExpression>(assignExpression->getRhs().get()))
    {
        assignExpression->setRhs(TypecheckUtils::recast(assignExpression->getRhs(), leftType));
        return;
    }
    if (llvm::isa<RangeDeclaration>(leftType.get()) && llvm::isa<IntegerExpression>(assignExpression->getRhs().get()))
    {
        std::shared_ptr<Range> r = leftType->getRange();
        int64_t v = llvm::dyn_cast<IntegerExpression>(assignExpression->getRhs().get())->getValue();
        if (r->getStart() > v || v > r->getEnd())
        {
            this->semanticAnalizer->diagnosticsEngine->japc_error_at(assignExpression->getLocation(),
                                                                     "Value must be in range");
        }
        return;
    }

    if (isCharArray(leftType.get()) && !llvm::isa<StringDeclaration>(leftType.get()) &&
        llvm::isa<StringExpression>(assignExpression->getRhs().get()))
    {
        StringExpression *s = llvm::dyn_cast<StringExpression>(assignExpression->getRhs().get());
        ArrayDeclaration *aty = llvm::dyn_cast<ArrayDeclaration>(leftType.get());
        if (aty->getRanges().size() == 1)
        {
            if (aty->getRanges()[0]->getRange()->getSize() >= s->getValue().size())
            {
                return;
            }
        }
        this->semanticAnalizer->diagnosticsEngine->japc_error_at(assignExpression->getLocation(),
                                                                 "String assignment invalid from a string constant");
        return;
    }

    const TypeDeclaration *ty = leftType->isAssignableType(rightType.get());
    if (!ty)
    {
        this->semanticAnalizer->diagnosticsEngine->japc_error_at(assignExpression->getLocation(),
                                                                 "Incompatible type in assignment");
        return;
    }
    assignExpression->setRhs(TypecheckUtils::recast(assignExpression->getRhs().get(), ty));
}
void TypeCheckVisitor::checkRangeExpr(std::shared_ptr<RangeExpression> rangeExpression)
{
    if (rangeExpression->getHighExpression()->getTypeDeclaration() !=
        rangeExpression->getHighExpression()->getTypeDeclaration())
    {
        this->semanticAnalizer->diagnosticsEngine->japc_error_at(
            rangeExpression->getLocation(), "Upper and lower type values in range expression are different");
    }
}
void TypeCheckVisitor::checkSetExpr(std::shared_ptr<SetExpression> setExpression)
{
    std::shared_ptr<Range> range;
    if (!(range = setExpression->getTypeDeclaration()->getRange()))
    {
        std::shared_ptr<RangeDeclaration> rangeDecl = getRangeDeclaration(setExpression->getTypeDeclaration());
        if (setExpression->getTypeDeclaration()->getSubtype())
        {
        }
    }
}
std::shared_ptr<RangeDeclaration> TypeCheckVisitor::getRangeDeclaration(
    std::shared_ptr<TypeDeclaration> typeDeclaration)
{
    std::shared_ptr<Range> range = typeDeclaration->getRange();
    std::shared_ptr<TypeDeclaration> base;
    if (!range)
    {
        if (!typeDeclaration->getSubtype())
        {
            return nullptr;
        }
        range = typeDeclaration->getSubtype()->getRange();
    }

    if (typeDeclaration->isIntegral())
    {
        base = typeDeclaration;
    }
    else
    {
        base = typeDeclaration->getSubtype();
    }

    if (range->getSize() > __MAX__SET__SIZE__)
    {
        range = std::make_shared<Range>(0, __MAX__SET__SIZE__ - 1);
    }

    return std::make_shared<RangeDeclaration>(range, base);
}
void TypeCheckVisitor::checkArrayExpr(std::shared_ptr<ArrayExpression> arrayExpression)
{
    for (size_t i = 0; i < arrayExpression->getIndices().size(); i++)
    {
        std::shared_ptr<ExpressionAST> indexExpression = arrayExpression->getIndices()[i];
        if (!indexExpression->getTypeDeclaration()->isIntegral())
        {
            this->semanticAnalizer->diagnosticsEngine->japc_error_at(indexExpression->getLocation(),
                                                                     "Array index must be integral type");
        }
        std::shared_ptr<RangeDeclaration> range = arrayExpression->getRanges()[i];
        if (!range->getTypeKind() != indexExpression->getTypeDeclaration()->getTypeKind() &&
            indexExpression->getTypeDeclaration()->isCompatibleType(range.get()))
        {
            this->semanticAnalizer->diagnosticsEngine->japc_error_at(indexExpression->getLocation(),
                                                                     "Index expression must be compatible type");
        }
    }
}
void TypeCheckVisitor::checkBuiltInExpr(std::shared_ptr<BuiltInExpression> builtInExpression)
{
}
void TypeCheckVisitor::checkCallExpr(std::shared_ptr<CallFunctExpression> callFunctExpression)
{
    const std::shared_ptr<PrototypeExpression> prototypeExpression = callFunctExpression->getPrototype();
    if (callFunctExpression->getArgs().size() != prototypeExpression->getArgs().size())
    {
        this->semanticAnalizer->diagnosticsEngine->japc_error_at(
            callFunctExpression->getLocation(), "Call to %s expected to have %d arguments, but %d were found",
            prototypeExpression->getName().c_str(), prototypeExpression->getArgs().size(),
            callFunctExpression->getArgs().size());
        return;
    }
    int idx = 0;
    const std::vector<std::shared_ptr<VariableDefinition>> &parg = prototypeExpression->getArgs();
    for (auto &a : callFunctExpression->getArgs())
    {
        bool bad = true;

        if (const TypeDeclaration *ty =
                parg[idx]->getTypeDeclaration()->isCompatibleType(a->getTypeDeclaration().get()))
        {
            if (parg[idx]->isReference() &&
                !(llvm::isa<AddressableExpression>(a.get()) || llvm::isa<ClosureExpression>(a.get())))
            {
                this->semanticAnalizer->diagnosticsEngine->japc_error_at(a->getLocation(),
                                                                         "Expect variable for 'var' parameter");
            }
            else
            {
                a = TypecheckUtils::recast(a.get(), ty);
                bad = false;
            }
        }
        else if (llvm::isa<PointerDeclaration>(parg[idx]->getTypeDeclaration().get()) &&
                 llvm::isa<NullExpression>(a.get()))
        {
            a = TypecheckUtils::recast(a, parg[idx]->getTypeDeclaration());
            bad = false;
        }
        else if (FunctionPointerDeclaration *argTy =
                     llvm::dyn_cast<FunctionPointerDeclaration>(parg[idx]->getTypeDeclaration().get()))
        {
            FunctionExpression *fnArg = llvm::dyn_cast<FunctionExpression>(a.get());
            assert(fnArg && "Expected argument to be FunctionExprAST");

            if (fnArg->getPrototype()->isMatchWithoutClosure(argTy->getPrototype().get()))
            {
                // Todo: Make this a function
                std::vector<std::shared_ptr<VariableExpression>> vf;
                std::shared_ptr<Function> fn = fnArg->getPrototype()->getFunction();
                std::shared_ptr<TypeDeclaration> closureTy = fn->getReturnType();
                for (auto u : fn->getUsedVars())
                {
                    vf.push_back(
                        std::make_shared<VariableExpression>(fn->getLocation(), u.getName(), u.getTypeDeclaration()));
                }
                std::shared_ptr<ClosureExpression> closure =
                    std::make_shared<ClosureExpression>(fn->getLocation(), closureTy, vf);
                a = std::make_shared<TrampolineExpression>(fnArg->getLocation(),
                                                           std::shared_ptr<FunctionExpression>(fnArg), closure,
                                                           std::shared_ptr<FunctionPointerDeclaration>(argTy));
                bad = false;
            }
            else
            {
                bad = !(*fnArg->getPrototype() == *argTy->getPrototype());
            }
        }
        if (bad)
        {
            this->semanticAnalizer->diagnosticsEngine->japc_error_at(a->getLocation(), "Incompatible argument type %s",
                                                                     std::to_string(idx).c_str());
        }
        idx++;
    }
}
void TypeCheckVisitor::checkReadExpr(std::shared_ptr<ReadExpression> readExpression)
{
}
void TypeCheckVisitor::checkWriteExpr(std::shared_ptr<WriteExpression> writeExpression)
{
}
void TypeCheckVisitor::checkForExpr(std::shared_ptr<ForExpression> forExpression)
{
    std::shared_ptr<TypeDeclaration> vty = forExpression->getVariable()->getTypeDeclaration();
    bool bad = !vty->isIntegral();
    if (bad)
    {
        this->semanticAnalizer->diagnosticsEngine->japc_error_at(forExpression->getVariable()->getLocation(),
                                                                 "Iteration variable must be numeric type");
        return;
    }
    if (forExpression->getEnd())
    {
        if (const TypeDeclaration *ty = forExpression->getStart()->getTypeDeclaration()->isCompatibleType(vty.get()))
        {
            forExpression->setStart(
                TypecheckUtils::recast(forExpression->getStart().get(), ty));
        }
        else
        {
            bad = true;
        }
        if (const TypeDeclaration *ty = forExpression->getEnd()->getTypeDeclaration()->isCompatibleType(vty.get()))
        {
            forExpression->setEnd(
                TypecheckUtils::recast(forExpression->getEnd().get(), ty));
        }
        else
        {
            bad = true;
        }
    }
    else
    {
        if (auto setDecl = llvm::dyn_cast<SetDeclaration>(forExpression->getStart()->getTypeDeclaration().get()))
        {
            if (!setDecl->getSubtype()->isCompatibleType(vty.get()))
            {
                this->semanticAnalizer->diagnosticsEngine->japc_error_at(forExpression->getVariable()->getLocation(),
                                                                         "Expected variable to be compatible with set");
            }
        }
        else
        {
            this->semanticAnalizer->diagnosticsEngine->japc_error_at(forExpression->getStart()->getLocation(),
                                                                     "Expected to be a set");
        }
    }
    if (bad)
    {
        this->semanticAnalizer->diagnosticsEngine->japc_error_at(forExpression->getLocation(), "Bad for loop");
    }
}
void TypeCheckVisitor::checkCaseExpr(std::shared_ptr<CaseExpression> caseExpression)
{
    if (!caseExpression->getExpre()->getTypeDeclaration()->isIntegral())
    {
        this->semanticAnalizer->diagnosticsEngine->japc_error_at(caseExpression->getLocation(),
                                                                 "Expression used un case expression must be integral");
    }
    std::vector<std::pair<int, int>> vals;
    for (auto label : caseExpression->getLabels())
        for (auto labelVal : label->getLabels())
        {
            if (std::find(vals.begin(), vals.end(), labelVal) != vals.end())
            {
                this->semanticAnalizer->diagnosticsEngine->japc_error_at(
                    caseExpression->getLocation(), "The label %d in case expression was already declared",
                    labelVal.first);
            }
            vals.push_back(labelVal);
        }
}
void TypeCheckVisitor::visit(ExpressionAST *elem)
{
    if (elem == nullptr){
        return;
    }
    if (BinaryExpression *b = llvm::dyn_cast<BinaryExpression>(elem))
    {
        checkBinaryExpr(std::shared_ptr<BinaryExpression>(b));
    }
    else if (AssignExpression *a = llvm::dyn_cast<AssignExpression>(elem))
    {
        checkAssignExpr(std::shared_ptr<AssignExpression>(a));
    }
    else if (RangeExpression *r = llvm::dyn_cast<RangeExpression>(elem))
    {
        checkRangeExpr(std::shared_ptr<RangeExpression>(r));
    }
    else if (SetExpression *s = llvm::dyn_cast<SetExpression>(elem))
    {
        checkSetExpr(std::shared_ptr<SetExpression>(s));
    }
    else if (ArrayExpression *a = llvm::dyn_cast<ArrayExpression>(elem))
    {
        checkArrayExpr(std::shared_ptr<ArrayExpression>(a));
    }
    else if (BuiltInExpression *b = llvm::dyn_cast<BuiltInExpression>(elem))
    {
        checkBuiltInExpr(std::shared_ptr<BuiltInExpression>(b));
    }
    else if (CallFunctExpression *c = llvm::dyn_cast<CallFunctExpression>(elem))
    {
        checkCallExpr(std::shared_ptr<CallFunctExpression>(c));
    }
    else if (ForExpression *f = llvm::dyn_cast<ForExpression>(elem))
    {
        checkForExpr(std::shared_ptr<ForExpression>(f));
    }
    else if (ReadExpression *r = llvm::dyn_cast<ReadExpression>(elem))
    {
        checkReadExpr(std::shared_ptr<ReadExpression>(r));
    }
    else if (WriteExpression *w = llvm::dyn_cast<WriteExpression>(elem))
    {
        checkWriteExpr(std::shared_ptr<WriteExpression>(w));
    }
    else if (CaseExpression *c = llvm::dyn_cast<CaseExpression>(elem))
    {
        checkCaseExpr(std::shared_ptr<CaseExpression>(c));
    }
}
