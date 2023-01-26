//
// Created by wuser on 18/01/23.
//

#ifndef JAPC_BINARY_EXPRESSION_H
#define JAPC_BINARY_EXPRESSION_H
#include "japc/AST/char_expression.h"
#include "japc/AST/expression.h"
#include "japc/AST/named_object.h"
#include "japc/AST/set_expression.h"
#include "japc/AST/string_expression.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/IR/Value.h>

namespace Pascal
{

llvm::Value *makeStrCompare(TokenType oper, llvm::Value *v);
static int stringishScore(ExpressionAST *expression);
static bool bothAreString(ExpressionAST *left, ExpressionAST *right);
static llvm::Value *shortCtAnd(ExpressionAST *left, ExpressionAST *right);
static llvm::Value *shortCtOr(ExpressionAST *left, ExpressionAST *right);
static llvm::Value* callStrCat(std::shared_ptr<ExpressionAST> lhs, std::shared_ptr<ExpressionAST> rhs);
class BinaryExpression : public ExpressionAST
{
  public:
    Token &getOper();
    std::shared_ptr<ExpressionAST> &getLhs();
    std::shared_ptr<ExpressionAST> &getRhs();
    BinaryExpression(Token tok, std::shared_ptr<ExpressionAST> lhs, std::shared_ptr<ExpressionAST> rhs);
    llvm::Value *codeGen() override;
    static bool isClassOf(const ExpressionAST *expressionAst);
    std::shared_ptr<TypeDeclaration> getTypeDeclaration() const override;
    void accept(ExpressionVisitor &visitor) override;
    static bool classof(const ExpressionAST *expressionAst);

  private:
    llvm::Value *setCodeGen();
    llvm::Value *inlineSetFunc(const std::string &name);
    llvm::Value *callSetFunc(const std::string &name, bool resultTypeIsSet);
    llvm::Value *callStrFunc(const std::string &name);
    llvm::Value *callArrFunc(const std::string &name, size_t size);
    Token oper;
    std::shared_ptr<ExpressionAST> lhs;
    std::shared_ptr<ExpressionAST> rhs;
};

} // namespace Pascal
#endif // JAPC_BINARY_EXPRESSION_H
