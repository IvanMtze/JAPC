//
// Created by wuser on 18/01/23.
//

#include "japc/AST/label_expression.h"
using namespace Pascal;
LabelExpression::LabelExpression(const Location &loc, const std::vector<std::pair<int, int>> &lab,
                std::shared_ptr<ExpressionAST> stmt)
    : ExpressionAST(loc, ExpressionType::TYPE_LABEL_EXPRE), labelsValues(lab), statement(stmt)
{
}
llvm::Value *LabelExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
llvm::Value *LabelExpression::codeGen(std::shared_ptr<llvm::BasicBlock> casebb,
                                      std::shared_ptr<llvm::BasicBlock> afterbb)
{
}

void LabelExpression::accept(ExpressionVisitor &visitor)
{
    if (statement)
    {
        statement->accept(visitor);
    }
    visitor.visit(this);
}
std::vector<std::pair<int, int>> &LabelExpression::getLabels()
{
    return this->labelsValues;
};
bool LabelExpression::isClassOf(ExpressionAST *expressionAst)
{
    return expressionAst->getExpressionType() == ExpressionType::TYPE_LABEL_EXPRE;
}