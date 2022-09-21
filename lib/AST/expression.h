//
// Created by wuser on 19/09/22.
//

#ifndef JAPC_STATEMENT_H
#define JAPC_STATEMENT_H

#include "scanner.h"
#include "visitor.h"
namespace Pascal
{
class Expression
{
  public:
    virtual void accept(Visitor visitor);
};

class Primary : public Expression
{
  public:
};

class Factor : public Expression
{
  public:
    Primary leftPrimary;
    TokenType exponentiatingOperator;
    Primary rightPrimary;
};

class Term : public Expression
{
  public:
    Factor leftFactor;
    TokenType multiplyingOperator;
    Factor rightFactor;
};

class SimpleExpression : public Expression
{
  public:
    TokenType sign;
    Term leftTerm;
    TokenType addingOperator;
    Term rightTerm;
};

class MainExpression: public Expression{
  public:
    SimpleExpression leftSimpleExpression;
    SimpleExpression rightSimpleExpression;
    TokenType relationalOperator;
};
} // namespace Pascal
#endif // JAPC_STATEMENT_H
