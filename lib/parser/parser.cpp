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

Parser::Parser(std::unique_ptr<Scanner> scanner)
{
    this->scanner = std::move(scanner);
}
void Parser::parseFile()
{
    currentTokenPos = 0;
    std::vector<Token> vector;
    tokenList = std::make_unique<std::vector<Token>>(vector);
    do{
        scanner->scan();
        Token tok = scanner->getCurrentTokenObject();
        tokenList->push_back(tok);
    }while (scanner->getCurrentTokenObject().getTokenType() != TokenType::END_OF_FILE);
    parseProgram();
}
bool Parser::compareAhead(const int num, Pascal::TokenType tokenType)
{
}
std::unique_ptr<Token> Parser::advance()
{
    if (!isAtEnd())
        currentTokenPos++;
}
std::unique_ptr<Token> Parser::current()
{
    if (currentTokenPos < tokenList->size())
    {
        return std::make_unique<Token>(tokenList->at(currentTokenPos));
    }
    return nullptr;
}
bool Parser::isAtEnd()
{
    return current()->getTokenType() == TokenType::END_OF_FILE;
}
std::unique_ptr<Token> Parser::lookAhead(const int& num)
{
    if(tokenList->size() >= currentTokenPos + num){
        return nullptr;
    }
    return std::make_unique<Token>(tokenList->at(currentTokenPos + num));
}
bool Parser::match(Pascal::TokenType tokenType)
{
}
std::unique_ptr<Token> Parser::previous()
{
    if (currentTokenPos > 0)
    {
        return std::make_unique<Token>(tokenList->at(currentTokenPos - 1));
    }
}
void Parser::sync()
{
}
bool Parser::match(std::vector<TokenType> tokensToMatch)
{
}
std::unique_ptr<MainExpression> Parser::parseExpression()
{
    //  ISO 10206 -  6.8.1
    //  EXPRESSION = SIMPLE-EXPRESSION [ RELATIONAL-OPERATOR SIMPLE-EXPRESSION ]
    std::unique_ptr<MainExpression> mainExpression;
    mainExpression->leftSimpleExpression = parseSimpleExpression();
    if (isRelationalOperator(current()->getTokenType()))
    {
        mainExpression->relationalOperator = current()->getTokenType();
        advance();
        // TODO: THROW ERROR IF NOT SIMPLE-EXPRESSION
        mainExpression->rightSimpleExpression = parseSimpleExpression();
    }
    return mainExpression;
}
std::unique_ptr<Primary> Parser::parsePrimary()
{
    Primary primaryExpression;
}
std::unique_ptr<Factor> Parser::parseFactor()
{
    //  ISO 10206 -  6.8.1
    //  FACTOR = PRIMARY [ EXPONENTIATING-OPERATOR PRIMARY ]
    std::unique_ptr<Factor> factor;
    factor->leftPrimary = parsePrimary();
    if (isExponentiatingOperator(current()->getTokenType()))
    {
        factor->exponentiatingOperator = std::make_unique<TokenType>(current()->getTokenType());
        advance();
        //  TODO: THROW ERROR IF NOT PRIMARY AND SYNC
        factor->rightPrimary = parsePrimary();
    }
    return factor;
}
std::unique_ptr<SimpleExpression> Parser::parseSimpleExpression()
{
    //  ISO 10206 -  6.8.1
    // SIMPLE-EXPRESSION = [ SIGN ] TERM [ ADDING-OPERATOR TERM ]
    std::unique_ptr<SimpleExpression> simpleExpression;
    if (isSign(current()->getTokenType()))
    {
        simpleExpression->sign = current()->getTokenType();
        advance();
    } //  TODO: May assume positive sign?

    simpleExpression->leftTerm = parseTerm();
    // PARSE OPTINAL [ADDING-OPERATOR TERM]
    if (isAddingOperator(current()->getTokenType()))
    {
        // Consume addingOperator
        simpleExpression->addingOperator = current()->getTokenType();
        advance();
        // SEARCH FOR TERM
        std::unique_ptr<Term> rightTerm = parseTerm();
        //  TODO: Throw error if term is not found and sync
    }
    return simpleExpression;
}

std::unique_ptr<Term> Parser::parseTerm()
{
    //  ISO 10206 -  6.8.1
    // TERM = FACTOR { MULTIPLYING-OPERATOR FACTOR }
    std::unique_ptr<Term> term;
    term->leftFactor = parseFactor();
    while (isMultiplyingOperator(current()->getTokenType()))
    {
        term->multiplyingOperator = current()->getTokenType();
        advance();
        // TODO: THROW error if no factor is found and sync
        std::unique_ptr<Factor> rightFactor = parseFactor();
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
void Parser::parseProgram()
{
    //  ISO 10206 -  6.12
    parseMainProgramDeclaration();
}
void Parser::parseProgramComponent()
{
}
void Parser::parseMainProgramDeclaration()
{
    Program mainProgram;
    parseProgramHeading(mainProgram);
    if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
    {
        //  TODO: Sync error missing semicolon after program header
    }
    parseMainProgramBlock();
}
void Parser::parseProgramHeading(Program &program)
{
    if (current()->getTokenType() != TokenType::SYMBOL_PROGRAM)
    {
        //  TODO: Missing program declaration error, sync to semicolon
        sync();
        return;
    }
    advance(); // eat PROGRAM keyword
    if (current()->getTokenType() != TokenType::IDENTIFIER)
    {
        //  TODO: Missing program identifier
        sync();
    }
    program.name = current()->getValue();
    advance();
    if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
    {
        parseProgramParameterList();
        if (current()->getTokenType() != TokenType::SYMBOL_PAREN_CLOSE)
        {
            //  TODO: Error missing paren close of program param list
            sync();
        }
        advance();
    }
    return;
}
void Parser::parseMainProgramBlock()
{
    if (current()->getTokenType() == TokenType::SYMBOL_IMPORT)
    {
        parseImportPart();
    }
    while (!isAtEnd() || current()->getTokenType() != TokenType::SYMBOL_BEGIN)
    {
        switch (current()->getTokenType())
        {
        case TokenType::SYMBOL_LABEL:
            break;
        case TokenType::SYMBOL_TYPE:
            break;
        case TokenType::SYMBOL_VAR:
            break;
        case TokenType::SYMBOL_PROCEDURE:
            parseProcedure();
            break;
        case TokenType::SYMBOL_FUNCTION:
            parseFunction();
        default:
            //  TODO: ERROR ??
            sync();
        }
    }
    if (isAtEnd())
    {
        //  TODO: THROW ERROR NO MAIN PROGRAM BLOCK DECLARATION FOUND, END OF FILE REACHED INSTEAD.
    }
    parseMainProgramBlock();
}
void Parser::parseImportPart()
{
}
void Parser::parseConstantDefinitionPart()
{
}
void Parser::parseTypeDefinitionPart()
{
}
void Parser::parseVariableDeclarationPart()
{
}
void Parser::parseProcedureAndDefinitionPart()
{
}
void Parser::parseProcedureDeclaration()
{
}
std::unique_ptr<Function> Parser::parseFunctionDeclaration()
{
    advance(); // Eat FUNCTION keyword
    std::unique_ptr<Function> funct;
    if (current()->getTokenType() != TokenType::IDENTIFIER)
    {
        // TODO: throw error function identifier expected,
        //      as it is safe to continue parsing we can
        //      continue with the declaration parsing.
    }
    else
    {
        funct->identifier = current()->getValue();
    }
    advance();
    funct->identifier = current()->getValue();
    if (current()->getTokenType() != TokenType::SYMBOL_PAREN_OPEN)
    {
        //  TODO: throw error function expected symbol paren open
    }
    //  TODO: parse arg list.
    advance();
    if (current()->getTokenType() != TokenType::SYMBOL_PAREN_CLOSE)
    {
        //  TODO: throw error function expected symbol paren close
    }
    advance();
}
void Parser::parseVariableAccess()
{
    //  ISO 10206 -  6.8.1
    //  VARIABLE-ACCESS                 = ENTIRE-VARIABLE | COMPONENT-VARIABLE |
    //                                    IDENTIFIED-VARIABLE | BUFFER-VARIABLE |
    //                                    SUBSTRING-VARIABLE | FUNCTION-IDENTIFIED-VARIABLE
    //  ENTIRE-VARIABLE                 = VARIABLE-NAME
    //  VARIABLE-NAME                   = [ IMPORTED-INTERFACE-IDENTIFIER '.' ] VARIABLE-IDENTIFIER
    //  VARIABLE-IDENTIFIER             = IDENTIFIER
    //  IMPORTED-INTERFACE-IDENTIFIER   = IDENTIFIER
    //  According to this definition ENTIRE-VARIABLE must be a VARIABLE-NAME. Thus ENTIRE-VARIABLE maps
    //  to the following [ IDENTIFIER '.' ] IDENTIFIER
    //  COMPONENT-VARIABLE              = INDEXED-VARIABLE FIELD-DESIGNATOR
    //  INDEXED-VARIABLE                = ARRAY-VARIABLE '[' INDEX-EXPRESSION { ',' INDEX-EXPRESSION } ']'
    //                                    STRING-VARIABLE '[' INDEX-EXPRESSION ']'
    //  ARRAY-VARIABLE                  = VARIABLE-ACCESS
    //  STRING-VARIABLE                 = VARIABLE-ACCESS
    std::deque<char> stack;
    switch (current()->getTokenType())
    {
    case TokenType::IDENTIFIER:
        parseIdentifier();
    case TokenType::END_OF_FILE:
        // TODO: THROW UNEXPECTED END OF FILE
        break;
    default:
        // TODO: THROW UNKNOWN ERROR
        break;
    }
}
void Parser::parseUnsignedConstant()
{
}
void Parser::parseSetConstructor()
{
}
void Parser::parseFunctionAccess()
{
}
void Parser::parseParenthesizedExpression()
{
}
void Parser::parseNegatedPrimary()
{
}
void Parser::parseConstantAccess()
{
}
void Parser::parseSchemaDiscriminant()
{
}
void Parser::parseStructuredValueConstructor()
{
}
void Parser::parseDiscriminantIdentifier()
{
}
std::unique_ptr<VariableAccess> Parser::parseIdentifier()
{
    std::unique_ptr<VariableAccess> var;
    if (current()->getTokenType() == TokenType::END_OF_FILE)
    {
        //  TODO: Error unexpected end of file
    }
    var->name = current()->getValue();
    // Eat current TokenType is Identifier
    advance();
    switch (current()->getTokenType())
    {
    case TokenType::SYMBOL_DOT:
        // Is trying to access a member
        var->variableType = VariableType::MemberAccess;
        break;
    case TokenType::SYMBOL_SQUARE_BRACKET_OPEN:
        // Array access
        advance();
        var->index = parseExpression();
        if (current()->getTokenType() != TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
        {
            // TODO: Throw error missing square bracket close
            var->index = parseExpression();
        }
        break;
    }
    return var;
}
void Parser::parseProgramParameterList()
{
}
std::unique_ptr<Function> Parser::parseFunction()
{
    if (current()->getTokenType() != TokenType::SYMBOL_FUNCTION)
    {
        //  TODO: Throw error expected FUNCTION symbol
    }
    std::unique_ptr<Function> function;
    advance();
    if (current()->getTokenType() != TokenType::IDENTIFIER)
    {
        //  TODO: Throw error expected FUNCTION IDENTIFIER
    }
    function->identifier = current()->getValue();
    function->paramList = parseFunctionParams();
}
void Parser::parseProcedure()
{
}
std::unique_ptr<std::vector<std::unique_ptr<FunctionParameter>>> Parser::parseFunctionParams()
{
    std::unique_ptr<std::vector<std::unique_ptr<FunctionParameter>>> functionParameterList;
    if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
    {
        advance();
    }
    return functionParameterList;
}
std::unique_ptr<FunctionParameter> Parser::parseFunctionParameter()
{
    std::unique_ptr<FunctionParameter> param;
    param->accessModifier = std::make_unique<AccessModifier>(parseFunctionAccessModifier());
    if (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        param->identifier = current()->getValue();
    }
    else
    {
        // TODO:: Throw expected identifier as param
    }
    return param;
}
AccessModifier Parser::parseFunctionAccessModifier()
{
    switch (current()->getTokenType())
    {
    case TokenType::SYMBOL_PROTECTED:
        advance();
        return AccessModifier::PROTECTED;
    default:
        return AccessModifier::DEFAULT;
    }
}
