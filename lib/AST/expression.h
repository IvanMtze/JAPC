//
// Created by wuser on 19/09/22.
//

#ifndef JAPC_STATEMENT_H
#define JAPC_STATEMENT_H

#include "scanner.h"
#include "visitor.h"
namespace Pascal
{
enum class PrimaryType : int
{
    VariableAccess = 1 << 0,
    UnsignedConstant = 1 << 2,
    SetConstructor = 1 << 3,
    FunctionAccess = 1 << 4,
    ParenthesizedExpression = 1 << 5,
    NegatingPrimary = 1 << 6,
    ConstantAccess = 1 << 7,
    SchemaDiscriminant = 1 << 8,
    StructuredValueConstructor = 1 << 9,
    DiscriminantIdentifier = 1 << 10,
};
enum class VariableType : int
{
    ArrayAccess = 1 << 0,
    SimpleVarible = 1 << 1,
    MemberAccess = 1 << 2,
};

class Expression
{
  public:
    void accept(Visitor visitor);
};

class Primary : public Expression
{
  public:
    PrimaryType primaryType;
};

class Factor : public Expression
{
  public:
    std::unique_ptr<Primary> leftPrimary;
    std::unique_ptr<TokenType> exponentiatingOperator;
    std::unique_ptr<Primary> rightPrimary;
};

class Term : public Expression
{
  public:
    std::unique_ptr<Factor> leftFactor;
    TokenType multiplyingOperator;
    std::unique_ptr<Factor> rightFactor;
};

class SimpleExpression : public Expression
{
  public:
    TokenType sign;
    std::unique_ptr<Term> leftTerm;
    TokenType addingOperator;
    std::unique_ptr<Term> rightTerm;
};

class MainExpression : public Expression
{
  public:
    std::unique_ptr<SimpleExpression> leftSimpleExpression;
    std::unique_ptr<SimpleExpression> rightSimpleExpression;
    TokenType relationalOperator;
};

class VariableAccess : public Expression
{
  public:
    std::string name;
    std::unique_ptr<Expression> index;
    VariableType variableType;
    std::unique_ptr<VariableAccess> memberAccess;
};
} // namespace Pascal
#endif // JAPC_STATEMENT_H
