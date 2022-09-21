//
// Created by wuser on 17/09/22.
//
#include "parser.h"
using namespace Pascal;

// NOTE:
// ACCORDING TO ISO 10206 - 3.5 TABLE 1
// THE GRAMMAR DEFINITIONS FOLLOWS THE FOLLOWING SYMBOLOGY DEFINITION.
//
// =            SHALL BE DEFINED TO BE
// >            SHALL HAVE AS AN ALTERNATIVE DEFINITION
// |            ALTERNATIVELY
// .            END OF DEFINITION
// [ X ]        0 OR 1 INSTANCE OF X
// { X }        0 OR MORE INSTANCES OF X
// ( X | Y )    GROUPING: EITHER OF X OR Y
// 'XYZ'        THE TERMNAL SYMBOL XYZ
//

Parser::Parser(const Pascal::Scanner scanner)
{
}
void Parser::parseFile()
{
    currentTokenPos = 0;
    while (scanner.scan() != TokenType::END_OF_FILE)
    {
        tokenList.push_back(scanner.getCurrentTokenObject());
    }
}
bool Parser::compareAhead(const int num, Pascal::TokenType tokenType)
{
}
Token Parser::advance()
{
    if (!isAtEnd())
        currentTokenPos++;
}
Token Parser::current()
{
    if (!isAtEnd())
    {
        return tokenList[currentTokenPos];
    }
}
bool Parser::isAtEnd()
{
    return currentTokenPos >= tokenList.size();
}
Token Parser::lookAhead(const int num)
{
}
bool Parser::match(Pascal::TokenType tokenType)
{
}
Token Parser::previous()
{
    if(currentTokenPos > 0){
        return tokenList[currentTokenPos - 1];
    }
}
void Parser::sync()
{
}
bool Parser::match(std::vector<TokenType> tokensToMatch)
{
}
MainExpression Parser::parseExpression()
{
    //  ISO 10206 -  6.8.1
    //  EXPRESSION = SIMPLE-EXPRESSION [ RELATIONAL-OPERATOR SIMPLE-EXPRESSION ]
    MainExpression mainExpression;
    mainExpression.leftSimpleExpression = parseSimpleExpression();
    if (isRelationalOperator(current().getTokenType()))
    {
        mainExpression.relationalOperator = current().getTokenType();
        advance();
        // TODO: THROW ERROR IF NOT SIMPLE-EXPRESSION
        mainExpression.rightSimpleExpression = parseSimpleExpression();
    }
    return mainExpression;
}
Primary Parser::parsePrimary()
{
}
Factor Parser::parseFactor()
{
    //  ISO 10206 -  6.8.1
    //  FACTOR = PRIMARY [ EXPONENTIATING-OPERATOR PRIMARY ]
    Factor factor;
    factor.leftPrimary = parsePrimary();
    if (isExponentiatingOperator(current().getTokenType()))
    {
        factor.exponentiatingOperator = current().getTokenType();
        advance();
        //  TODO: THROW ERROR IF NOT PRIMARY AND SYNC
        factor.rightPrimary = parsePrimary();
    }
    return factor;
}
SimpleExpression Parser::parseSimpleExpression()
{
    //  ISO 10206 -  6.8.1
    // SIMPLE-EXPRESSION = [ SIGN ] TERM [ ADDING-OPERATOR TERM ]
    SimpleExpression simpleExpression;
    if (isSign(current().getTokenType()))
    {
        simpleExpression.sign = current().getTokenType();
        advance();
    } //  TODO: May assume positive sign?

    Term term = parseTerm();
    simpleExpression.leftTerm = term;
    // PARSE OPTINAL [ADDING-OPERATOR TERM]
    if (isAddingOperator(current().getTokenType()))
    {
        // Consume addingOperator
        simpleExpression.addingOperator = current().getTokenType();
        advance();
        // SEARCH FOR TERM
        Term rightTerm = parseTerm();
        //  TODO: Throw error if term is not found and sync
    }
    return simpleExpression;
}

Term Parser::parseTerm()
{
    //  ISO 10206 -  6.8.1
    // TERM = FACTOR { MULTIPLYING-OPERATOR FACTOR }
    Term term;
    term.leftFactor = parseFactor();
    while (isMultiplyingOperator(current().getTokenType()))
    {
        term.multiplyingOperator = current().getTokenType();
        advance();
        Factor rightFactor = parseFactor();
    }
    return term;
}
bool Parser::isSign(TokenType tk)
{
    return tk == TokenType::SYMBOL_MINUS || tk == TokenType::SYMBOL_PLUS;
}

bool Parser::isRelationalOperator(TokenType tk)
{
    return tk == TokenType::SYMBOL_EQUAL || tk == TokenType::SYMBOL_LESS_GREATER_THAN ||
           tk == TokenType::SYMBOL_LESS_THAN || tk == TokenType::SYMBOL_GREATER_THAN ||
           tk == TokenType::SYMBOL_LESS_EQUAL_THAN || tk == TokenType::SYMBOL_GREATER_EQUAL_THAN ||
           tk == TokenType::SYMBOL_IN;
}
bool Parser::isAddingOperator(TokenType tk)
{
    return tk == TokenType::SYMBOL_PLUS || tk == TokenType::SYMBOL_MINUS || tk == TokenType::SYMBOL_GREATER_LESS_THAN ||
           tk == TokenType::SYMBOL_OR || tk == TokenType::SYMBOL_OR_ELSE;
}
bool Parser::isExponentiatingOperator(TokenType tk)
{
    return tk == TokenType::SYMBOL_STAR_STAR || tk == TokenType::SYMBOL_POW;
}
bool Parser::isMultiplyingOperator(TokenType tk)
{
    return tk == TokenType::SYMBOL_STAR || tk == TokenType::SYMBOL_SLASH || tk == TokenType::SYMBOL_DIV ||
           tk == TokenType::SYMBOL_MOD || tk == TokenType::SYMBOL_AND || tk == TokenType::SYMBOL_AND_THEN;
}