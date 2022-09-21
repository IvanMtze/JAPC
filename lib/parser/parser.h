//
// Created by wuser on 17/09/22.
//

#ifndef JAPC_PARSER_H
#define JAPC_PARSER_H
#include "expression.h"
#include "scanner.h"
#include <list>

namespace Pascal
{

class Parser
{
  public:
    Parser(const Scanner scanner);
    void parseFile();
  private:
    Scanner scanner;
    int currentTokenPos;
    std::vector<Token> tokenList;
    Token lookAhead(const int num);
    bool compareAhead(const int num, TokenType tokenType);
    void sync();
    Token advance();
    Token current();
    Token previous();
    bool isAtEnd();
    bool match(TokenType tokenType);
    bool match(std::vector<TokenType> tokensToMatch);
    MainExpression parseExpression();
    Factor parseFactor();
    SimpleExpression parseSimpleExpression();
    Term parseTerm();
    Primary parsePrimary();
    bool isSign(TokenType tk);
    bool isRelationalOperator(TokenType tk);
    bool isAddingOperator(TokenType tk);
    bool isExponentiatingOperator(TokenType tk);
    bool isMultiplyingOperator(TokenType tk);
};
} // namespace Pascal
#endif // JAPC_PARSER_H
