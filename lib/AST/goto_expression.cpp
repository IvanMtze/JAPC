//
// Created by wuser on 18/01/23.
//

#include "japc/AST/goto_expression.h"
using namespace Pascal;
GotoExpression::GotoExpression(const Location &loc, int pos) : ExpressionAST(loc, ExpressionType::TYPE_GOTO), pos(pos)
{
}
llvm::Value *GotoExpression::codeGen()
{
    llvm::BasicBlock *labelBB = 0;//CreateGotoTarget(pos);
    llvm::Value *v = builder.CreateBr(labelBB);
    llvm::Function *fn = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *dead = llvm::BasicBlock::Create(theContext, "dead", fn);
    builder.SetInsertPoint(dead);
    return v;
}
bool GotoExpression::isClassOf(const ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_GOTO;
}