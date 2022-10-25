//
// Created by wuser on 15/10/22.
//
#include "japc/AST/expression.h"
using namespace Pascal;

void ExpressionAST::ensureSized() const
{
}
std::shared_ptr<llvm::Value> GotoExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

std::shared_ptr<llvm::Value> CallFunctExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> SizeOfExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> PointerExpression::getAddress()
{
    return VariableExpression::getAddress();
}
std::shared_ptr<llvm::Value> WithExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> LabelExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> AddressableExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> StringExpression::codeGen()
{
    return AddressableExpression::codeGen();
}
std::shared_ptr<llvm::Value> RangeCheckExpression::codeGen()
{
    return RangeReduceExpression::codeGen();
}
std::shared_ptr<llvm::Value> VariantFieldExpression::getAddress()
{
    return VariableExpression::getAddress();
}
std::shared_ptr<llvm::Value> CharExpression::codeGen()
{
    return IntegerExpression::codeGen();
}
std::shared_ptr<llvm::Value> ArrayExpression::getAddress()
{
    return VariableExpression::getAddress();
}
std::shared_ptr<llvm::Value> UnaryExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> AssignExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> IfExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> FilePointerExpression::getAddress()
{
    return VariableExpression::getAddress();
}
std::shared_ptr<llvm::Value> BinaryExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> WriteExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> RangeExpression::getLow()
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<llvm::Value> FieldExpression::getAddress()
{
    return VariableExpression::getAddress();
}
std::shared_ptr<llvm::Value> VariableDeclarationExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> TypeCastExpression::codeGen()
{
    return AddressableExpression::codeGen();
}
std::shared_ptr<llvm::Value> CaseExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> ReadExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
Function::Function(const Location &loc, std::shared_ptr<PrototypeExpression> prototype,
                   const std::vector<std::shared_ptr<VariableDeclarationExpression>> variablesDecl,
                   std::shared_ptr<BlockExpression> block)
{
}
std::shared_ptr<llvm::Function> Function::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> RealExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> BuiltInExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> VariableExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
std::shared_ptr<llvm::Value> SetExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
std::shared_ptr<llvm::Value> RangeReduceExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> ForExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> FunctionExpression::codeGen()
{
    return AddressableExpression::codeGen();
}
std::shared_ptr<llvm::Value> IntegerExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> BlockExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> RepeatExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> WhileExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Function> PrototypeExpression::create(const std::string &namePrefix)
{
    return std::shared_ptr<llvm::Function>();
}
std::shared_ptr<llvm::Value> NullExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void PrototypeExpression::createArgumentsAlloca()
{
}
void PrototypeExpression::addExtraArgsFirst(std::vector<std::shared_ptr<VariableDefinition>> vars)
{
}
bool PrototypeExpression::operator==(const PrototypeExpression &rhs) const
{
    return false;
}
bool PrototypeExpression::isMatchWithoutClosure(const PrototypeExpression *rhs) const
{
    return false;
}
void WhileExpression::accept(ExpressionVisitor &expressionVisitor)
{
    ExpressionAST::accept(expressionVisitor);
}
void RepeatExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
void BlockExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
void ForExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
std::shared_ptr<llvm::Value> ForExpression::forInGen()
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<llvm::Value> SetExpression::makeConstantSet(TypeDeclaration *type)
{
    return std::shared_ptr<llvm::Value>();
}
void BuiltInExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
std::shared_ptr<llvm::Function> Function::codeGen(const std::string &namePrefix)
{
    return std::shared_ptr<llvm::Function>();
}
std::shared_ptr<TypeDeclaration> Function::getReturnType()
{
    return std::shared_ptr<TypeDeclaration>();
}
void Function::accept(ExpressionVisitor &v)
{
    ExpressionAST::accept(v);
}
void ReadExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
void CaseExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
std::shared_ptr<llvm::Value> TypeCastExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
void FieldExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
std::shared_ptr<llvm::Value> RangeExpression::getHigh()
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<ExpressionAST> RangeExpression::getLowExpression()
{
    return std::shared_ptr<ExpressionAST>();
}
std::shared_ptr<ExpressionAST> RangeExpression::getHighExpression()
{
    return std::shared_ptr<ExpressionAST>();
}
void WriteExpression::accept(ExpressionVisitor &expressionVisitor)
{
    ExpressionAST::accept(expressionVisitor);
}
std::shared_ptr<TypeDeclaration> BinaryExpression::getTypeDeclaration() const
{
    return ExpressionAST::getTypeDeclaration();
}
std::shared_ptr<llvm::Value> BinaryExpression::setCodeGen()
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<llvm::Value> BinaryExpression::inlineSetFunc(const std::string &name)
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<llvm::Value> BinaryExpression::callSetFunc(const std::string &name, bool resultTypeIsSet)
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<llvm::Value> BinaryExpression::callStrFunc(const std::string &name)
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<llvm::Value> BinaryExpression::callArrFunc(const std::string &name, size_t size)
{
    return std::shared_ptr<llvm::Value>();
}
void FilePointerExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
void IfExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
std::shared_ptr<llvm::Value> AssignExpression::assignStr()
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<llvm::Value> AssignExpression::assignSet()
{
    return std::shared_ptr<llvm::Value>();
}
void ArrayExpression::accept(ExpressionVisitor &v)
{
    ExpressionAST::accept(v);
}
std::shared_ptr<llvm::Value> StringExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
std::shared_ptr<llvm::Value> LabelExpression::codeGen(std::shared_ptr<llvm::BasicBlock> casebb,
                                                      std::shared_ptr<llvm::BasicBlock> afterbb)
{
    return std::shared_ptr<llvm::Value>();
}
void LabelExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
void PointerExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
void CallFunctExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
