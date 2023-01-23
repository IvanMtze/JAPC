//
// Created by wuser on 12/10/22.
//

#ifndef JAPC_EXPRESSION_H
#define JAPC_EXPRESSION_H
#include "japc/AST/builtin.h"
#include "japc/AST/named_object.h"
#include "japc/AST/stack.h"
#include "japc/AST/type.h"
#include "japc/AST/visitor.h"
#include "japc/scanner/scanner.h"
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>

#include <memory>
#include <set>
extern llvm::Module *theModule;

namespace Pascal
{
class Label
{
  public:
    Label(llvm::BasicBlock *b, int lab) : bb(b), label(lab)
    {
    }
    llvm::BasicBlock *getBasicBlock()
    {
        return bb;
    }

  private:
    llvm::BasicBlock *bb;
    int label;
};

typedef Stack<Label *> LabelStack;
typedef StackWrapper<Label *> LabelWrapper;
typedef Stack<llvm::Value *> VarStack;
typedef StackWrapper<llvm::Value *> VarStackWrapper;

static VarStack variables;
static LabelStack labels;
static llvm::LLVMContext theContext;
static llvm::IRBuilder<> builder(theContext);

// Utils
llvm::AllocaInst* createTempAlloca(TypeDeclaration* ty);
static llvm::AllocaInst* createNamedAlloca(llvm::Function* fn, TypeDeclaration* ty, const std::string& name);
llvm::Value *integerBinaryExpression(llvm::Value *leftValue, llvm::Value *rightValue, TokenType &tokenType,
                                            TypeDeclaration *type, bool isUnsigned);
llvm::Value *powerInt(llvm::Value *base, llvm::Value *exp, TypeDeclaration *ty);
//
class ExpressionAST : public Visitable<ExpressionAST>
{
  public:
    enum ExpressionType
    {
        TYPE_EXPRE,
        TYPE_REAL_EXPRE,
        TYPE_INTEGER_EXPRE,
        TYPE_CHAR_EXPRE,
        TYPE_NIL_EXPRE,

        TYPE_ADDRESSABLE_EXPRE,
        TYPE_STRING_EXPRE,
        TYPE_SET_EXPRE,
        TYPE_VARIABLE_EXPRE,
        TYPE_ARRAY_EXPRE,
        TYPE_POINTER_EXPRE,
        TYPE_FILE_POINTER_EXPRE,
        TYPE_FIELD_EXPRE,
        TYPE_VARIANT_EXPRE,
        TYPE_FUNCTION_EXPRE,
        TYPE_TYPE_CAST_EXPRE,
        TYPE_LAST_ADDRESSABLE,

        TYPE_BINARY_EXPRE,
        TYPE_UNARY_EXPRE,
        TYPE_RANGE_EXPRE,
        TYPE_BLOCK_EXPRE,
        TYPE_ASSIGN_EXPRE,
        TYPE_VAR_DECL_EXPRE,
        TYPE_FUNCTION,
        TYPE_PROTOTYPE,
        TYPE_CALL_EXPRE,
        TYPE_BUILTIN_EXPRE,
        TYPE_IF_EXPRE,
        TYPE_FOR_EXPRE,
        TYPE_WHILE_EXPRE,
        TYPE_REPEAT_EXPRE,
        TYPE_WRITE,
        TYPE_READ,
        TYPE_LABEL_EXPRE,
        TYPE_CASE_EXPRE,
        TYPE_WITH_EXPRE,
        TYPE_RANGE_REDUCE_EXPRE,
        TYPE_RANGE_CHECK_EXPRE,
        TYPE_SIZE_OF_EXPRE,
        TYPE_V_TABLE_EXPRE,
        TYPE_VIRT_FUNCTION,
        TYPE_INIT_VALUE,
        TYPE_GOTO,
        TYPE_UNIT,
        TYPE_CLOSURE,
        TYPE_TRAMPOLINE,
    };

  public:
    ExpressionAST(const Location &location, ExpressionType expressionType);
    ExpressionAST(const Location &location, ExpressionType expressionType,
                  std::shared_ptr<TypeDeclaration> typeDeclaration);
    void accept(ExpressionVisitor &visitor) override;
    virtual llvm::Value *codeGen();
    ExpressionType getExpressionType() const;
    void setTypeDeclaration(std::shared_ptr<TypeDeclaration> typeDeclaration);
    virtual std::shared_ptr<TypeDeclaration> getTypeDeclaration() const;
    const Location getLocation() const;
    void ensureSized() const;

  private:
    const Location location;
    const ExpressionType expressionType;

  protected:
    std::shared_ptr<TypeDeclaration> typeDeclaration;
};

} // namespace Pascal
#endif // JAPC_EXPRESSION_H
