//
// Created by wuser on 18/01/23.
//

#include "japc/AST/unit_expression.h"

using namespace Pascal;
UnitExpression::UnitExpression(const Location &loc, const std::vector<std::shared_ptr<ExpressionAST>> &c,
                               std::shared_ptr<Function> init)
    : ExpressionAST(loc, ExpressionType::TYPE_UNIT), initFunc(init), code(c)
{
}
llvm::Value *UnitExpression::codeGen()
{
    for (auto i : code)
    {
        if (!i->codeGen())
            return 0;
    }
    if (initFunc)
    {
        initFunc->codeGen();
        if (initFunc->getPrototype()->getName() != "_MAIN_")
        {
            unitInit.push_back(initFunc);
        }
    }
}
void UnitExpression::accept(Pascal::ExpressionVisitor &v)
{
    for (auto i : code)
    {
        i->accept(v);
    }
    if (initFunc)
    {
        initFunc->accept(v);
    }
}
bool UnitExpression::classof(const ExpressionAST *e)
{
    return e->getExpressionType() == ExpressionType::TYPE_UNIT;
}
void Pascal::builtUnitInitList()
{
    std::vector<llvm::Constant *> unitList;
    llvm::Type *base = llvm::IntegerType::getInt8Ty(theContext);
    llvm::Type *vp = llvm::PointerType::getUnqual(base);
    int index = 0;
    for (auto v : unitInit)
    {
        llvm::Function *fn = theModule->getFunction("P." + v->getPrototype()->getName());
        if (!fn)
        {
            printf("Unable to find function!!");
            continue;
        }
    }
    unitList[unitInit.size()] = llvm::Constant::getNullValue(vp);
    llvm::ArrayType *arr = llvm::ArrayType::get(vp, unitInit.size() + 1);
    llvm::Constant *init = llvm::ConstantArray::get(arr, unitList);
    llvm::Value *unitInitList =
        new llvm::GlobalVariable(*theModule, arr, true, llvm::GlobalValue::ExternalLinkage, init, "UnitIniList");
    (void)unitInitList;
}

void backPatch()
{
    Pascal::builtUnitInitList();
}
