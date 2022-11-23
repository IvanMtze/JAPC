//
// Created by wuser on 15/10/22.
//
#include "japc/AST/expression.h"
using namespace Pascal;

void ExpressionAST::ensureSized() const
{
}

// GOTO EXPRESSION CLASS
std::shared_ptr<llvm::Value> GotoExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// CALL FUNCTION EXPRESSION
std::shared_ptr<llvm::Value> CallFunctExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void CallFunctExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// SIZE OF EXPRESSION
std::shared_ptr<llvm::Value> SizeOfExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// POINTER EXPRESSION
std::shared_ptr<llvm::Value> PointerExpression::getAddress()
{
}
void PointerExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// WITH EXPRESSION
std::shared_ptr<llvm::Value> WithExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// LABEL EXPRESSION
std::shared_ptr<llvm::Value> LabelExpression::codeGen()
{
    return ExpressionAST::codeGen();
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

// ADDRESSABLE EXPRESSION
std::shared_ptr<llvm::Value> AddressableExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// STRING EXPRESSION
std::shared_ptr<llvm::Value> StringExpression::codeGen()
{
    return AddressableExpression::codeGen();
}
std::shared_ptr<llvm::Value> StringExpression::getAddress()
{
    return AddressableExpression::getAddress();
}

// RANGE CHECK
std::shared_ptr<llvm::Value> RangeCheckExpression::codeGen()
{
    return RangeReduceExpression::codeGen();
}

// VARIANT FIELD EXPRESSION
std::shared_ptr<llvm::Value> VariantFieldExpression::getAddress()
{
    return VariableExpression::getAddress();
}

// CHAR EXPRESSION
std::shared_ptr<llvm::Value> CharExpression::codeGen()
{
    return IntegerExpression::codeGen();
}

// ARRAY EXPRESSION
ArrayExpression::ArrayExpression(const Location &loc, std::shared_ptr<ExpressionAST> v,
                                 const std::vector<std::shared_ptr<ExpressionAST>> inds,
                                 const std::vector<std::shared_ptr<RangeDeclaration>> rangeDeclaration,
                                 std::shared_ptr<TypeDeclaration> typeDeclaration)
    : AddressableExpression(loc, ExpressionType::TYPE_ARRAY_EXPRE, typeDeclaration)
{
}
std::shared_ptr<llvm::Value> ArrayExpression::getAddress()
{
}
void ArrayExpression::accept(ExpressionVisitor &v)
{
    ExpressionAST::accept(v);
}

// UNARY EXPRESSION
std::shared_ptr<llvm::Value> UnaryExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// ASSIGN EXPRESSION
std::shared_ptr<llvm::Value> AssignExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
std::shared_ptr<llvm::Value> AssignExpression::assignStr()
{
    return std::shared_ptr<llvm::Value>();
}
std::shared_ptr<llvm::Value> AssignExpression::assignSet()
{
    return std::shared_ptr<llvm::Value>();
}

// IF EXPRESSION
std::shared_ptr<llvm::Value> IfExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void IfExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// FILE POINTER EXPRESSION
std::shared_ptr<llvm::Value> FilePointerExpression::getAddress()
{
}
void FilePointerExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// BINARY EXPRESSION
std::shared_ptr<llvm::Value> BinaryExpression::codeGen()
{
    return ExpressionAST::codeGen();
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

// WRITE EXPRESSION
std::shared_ptr<llvm::Value> WriteExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void WriteExpression::accept(ExpressionVisitor &expressionVisitor)
{
    ExpressionAST::accept(expressionVisitor);
}

// RANGE EXPRESSION
std::shared_ptr<llvm::Value> RangeExpression::getLow()
{
    return std::shared_ptr<llvm::Value>();
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

// FIELD EXPRESSION
std::shared_ptr<llvm::Value> FieldExpression::getAddress()
{
    return VariableExpression::getAddress();
}
void FieldExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// VARIABLE DECLARATION EXPRESSION
std::shared_ptr<llvm::Value> VariableDeclarationExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// TYPE CAST EXPRESSION
std::shared_ptr<llvm::Value> TypeCastExpression::codeGen()
{
    return AddressableExpression::codeGen();
}
std::shared_ptr<llvm::Value> TypeCastExpression::getAddress()
{
    return AddressableExpression::getAddress();
}

// CASE EXPRESSION
std::shared_ptr<llvm::Value> CaseExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void CaseExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// READ EXPRESSION
std::shared_ptr<llvm::Value> ReadExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void ReadExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// FUNCTION
Function::Function(const Location &loc, std::shared_ptr<PrototypeExpression> prototype,
                   const std::vector<std::shared_ptr<VariableDeclarationExpression>> variablesDecl,
                   std::shared_ptr<BlockExpression> block)
    : ExpressionAST(loc, ExpressionType::TYPE_FUNCTION), prototype(prototype), variablesDecl(variablesDecl),
      body(block), parent(nullptr), returnType(nullptr)
{
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

// REAL EXPRESSION
std::shared_ptr<llvm::Value> RealExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// BUILTINT EXPRESSION
std::shared_ptr<llvm::Value> BuiltInExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

void BuiltInExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// VARIABLE EXPRESSION
std::shared_ptr<llvm::Value> VariableExpression::getAddress()
{
    return AddressableExpression::getAddress();
}

// SET EXPRESSION
std::shared_ptr<llvm::Value> SetExpression::getAddress()
{
    return AddressableExpression::getAddress();
}
std::shared_ptr<llvm::Value> SetExpression::makeConstantSet(TypeDeclaration *type)
{
    return std::shared_ptr<llvm::Value>();
}

// RANGE REDUCE EXPRESSION
std::shared_ptr<llvm::Value> RangeReduceExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// FOR EXPRESSION
std::shared_ptr<llvm::Value> ForExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void ForExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}
std::shared_ptr<llvm::Value> ForExpression::forInGen()
{
    return std::shared_ptr<llvm::Value>();
}

// FUNCTION EXPRESSION
std::shared_ptr<llvm::Value> FunctionExpression::codeGen()
{
    return AddressableExpression::codeGen();
}

// INTEGER EXPRESSION
std::shared_ptr<llvm::Value> IntegerExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

// BLOCK EXPRESSION
std::shared_ptr<llvm::Value> BlockExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void BlockExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// REPEAT EXPRESSION
std::shared_ptr<llvm::Value> RepeatExpression::codeGen()
{
    return ExpressionAST::codeGen();
}
void RepeatExpression::accept(ExpressionVisitor &visitor)
{
    ExpressionAST::accept(visitor);
}

// WHILE EXPRESSION
std::shared_ptr<llvm::Value> WhileExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

void WhileExpression::accept(ExpressionVisitor &expressionVisitor)
{
    ExpressionAST::accept(expressionVisitor);
}

// PROTOTYPE EXPRESSION
std::shared_ptr<llvm::Function> PrototypeExpression::create(const std::string &namePrefix)
{
    return std::shared_ptr<llvm::Function>();
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

// NULL EXPRESSION
std::shared_ptr<llvm::Value> NullExpression::codeGen()
{
    return ExpressionAST::codeGen();
}

std::shared_ptr<llvm::Value> ClosureExpression::codeGen()
{
}

// UNIT EXPRESSION
std::shared_ptr<llvm::Value> UnitExpression::codeGen()
{
}
void UnitExpression::accept(Pascal::ExpressionVisitor &v)
{
}