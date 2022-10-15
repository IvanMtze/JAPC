//
// Created by wuser on 17/09/22.
//
#include "japc/parser/parser.h"
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

Parser::Parser(std::unique_ptr<Scanner> scanner, std::unique_ptr<JAPCDiagnostics> diagnosticsEngine)
{
    this->scanner = std::move(scanner);
    this->diagnosticsEngine = std::move(diagnosticsEngine);
}
void Parser::parseFile()
{
    currentTokenPos = 0;
    std::vector<Token> vector;
    tokenList = std::make_unique<std::vector<Token>>(vector);
    do
    {
        TokenType t = scanner->scan();
        Token tok = scanner->getCurrentTokenObject();
        tokenList->push_back(tok);
    } while (scanner->getCurrentTokenObject().getTokenType() != TokenType::END_OF_FILE);
    parseProgram();
}
bool Parser::compareAhead(const int num, TokenType tokenType)
{
}
std::unique_ptr<Token> Parser::advance()
{
    std::unique_ptr<Token> old = current();
    if (!isAtEnd())
        currentTokenPos++;
    else
    {
        //  TODO: Error unexpected EOF while parsing
    }
    return old;
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
std::unique_ptr<Token> Parser::lookAhead(const int num)
{
    if (tokenList->size() <= currentTokenPos + num)
    {
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
void Parser::parseExpression()
{
    //  ISO 10206 -  6.8.1
    //  EXPRESSION = SIMPLE-EXPRESSION [ RELATIONAL-OPERATOR SIMPLE-EXPRESSION ]
    parseSimpleExpression();
    if (isRelationalOperator(current()->getTokenType()))
    {
        current()->getTokenType();
        advance();
        // TODO: THROW ERROR IF NOT SIMPLE-EXPRESSION
        parseSimpleExpression();
    }
}
void Parser::parsePrimary()
{
}
void Parser::parseFactor()
{
    //  ISO 10206 -  6.8.1
    //  FACTOR = PRIMARY [ EXPONENTIATING-OPERATOR PRIMARY ]
    parsePrimary();
    if (isExponentiatingOperator(current()->getTokenType()))
    {
        std::make_unique<TokenType>(current()->getTokenType());
        advance();
        //  TODO: THROW ERROR IF NOT PRIMARY AND SYNC
        parsePrimary();
    }
}
void Parser::parseSimpleExpression()
{
    //  ISO 10206 -  6.8.1
    // SIMPLE-EXPRESSION = [ SIGN ] TERM [ ADDING-OPERATOR TERM ]

    if (isSign(current()->getTokenType()))
    {
        current()->getTokenType();
        advance();
    } //  TODO: May assume positive sign?

    parseTerm();
    // PARSE OPTINAL [ADDING-OPERATOR TERM]
    if (isAddingOperator(current()->getTokenType()))
    {
        // Consume addingOperator
        current()->getTokenType();
        advance();
        // SEARCH FOR TERM
        parseTerm();
        //  TODO: Throw error if term is not found and sync
    }
}

void Parser::parseTerm()
{
    //  ISO 10206 -  6.8.1
    // TERM = FACTOR { MULTIPLYING-OPERATOR FACTOR }
    parseFactor();
    while (isMultiplyingOperator(current()->getTokenType()))
    {
        current()->getTokenType();
        advance();
        // TODO: THROW error if no factor is found and sync
        parseFactor();
    }
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
    parseProgramHeading();
    if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
    {
        //  TODO: Sync error missing semicolon after program header
    }
    else
    {
        advance();
    }
    if (isAtEnd())
    {
        // TODO: Error unexpected end of file
    }
    parseImportPart();
    if (isAtEnd())
    {
        // TODO: Error unexpected end of file
    }
    parseMainProgramBlock();
}
void Parser::parseProgramHeading()
{
    if (current()->getTokenType() != TokenType::SYMBOL_PROGRAM)
    {
        //  TODO: Missing program declaration error, do not sync
        return;
    }
    else
    {
        advance(); // eat PROGRAM keyword
        if (current()->getTokenType() != TokenType::IDENTIFIER)
        {
            //  TODO: Missing program identifier and sync
            sync();
        }
        else
        {
            current()->getValue();
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
        }
    }
}
void Parser::parseMainProgramBlock()
{
    while (!isAtEnd() && current()->getTokenType() != TokenType::SYMBOL_BEGIN)
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
            break;
        default:
            //  TODO: ERROR ??
            sync();
            break;
        }
    }
    if (isAtEnd())
    {
        //  TODO: THROW ERROR NO MAIN PROGRAM BLOCK DECLARATION FOUND, END OF FILE REACHED INSTEAD.
    }
}
void Parser::parseImportPart()
{
    while (current()->getTokenType() == TokenType::SYMBOL_IMPORT)
    {
        this->parseImportStatement();
    }
}
void Parser::parseConstantDefinitionPart()
{
}
void Parser::parseTypeDefinitionPart()
{
}
void Parser::parseVariableDeclarationPart()
{
    advance(); // eat VAR keyword
    while (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        parseIdentifierList();
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_COLON)
        {
            advance();
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                advance();
                if (current()->getTokenType() == TokenType::SYMBOL_SEMICOLON)
                {
                    advance();
                }
                else
                {
                    // TODO: Error expected semicolon after variable declaration;
                    break;
                }
            }
            else
            {
                // TODO: Error expected type after colon
                break;
            }
        }
        else
        {
            // TODO: Error expected colon after identier list
            break;
        }
    }
    advance();
}
void Parser::parseProcedureAndDefinitionPart()
{
}
void Parser::parseProcedureDeclaration()
{
}
void Parser::parseFunctionDeclaration()
{
    advance(); // Eat FUNCTION keyword
    if (current()->getTokenType() != TokenType::IDENTIFIER)
    {
        // TODO: throw error function identifier expected,
        //      as it is safe to continue parsing we can
        //      continue with the declaration parsing.
    }
    else
    {
        // funct->identifier = current()->getValue();
    }
    advance();
    // funct->identifier = current()->getValue();
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
void Parser::parseIdentifier()
{
    if (current()->getTokenType() == TokenType::END_OF_FILE)
    {
        //  TODO: Error unexpected end of file
    }
    current()->getValue();
    // Eat current TokenType is Identifier
    advance();
    switch (current()->getTokenType())
    {
    case TokenType::SYMBOL_DOT:
        // Is trying to access a member

        break;
    case TokenType::SYMBOL_SQUARE_BRACKET_OPEN:
        // Array access
        advance();
        parseExpression();
        if (current()->getTokenType() != TokenType::SYMBOL_SQUARE_BRACKET_CLOSE)
        {
            // TODO: Throw error missing square bracket close
            parseExpression();
        }
        break;
    }
}
void Parser::parseProgramParameterList()
{
    if (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        advance(); // eat first identifier
        while (current()->getTokenType() == TokenType::SYMBOL_COMMA)
        {
            advance(); // eat comma;
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                advance();
            }
            else
            {
                //  TODO: Error Expected identifier after comma in programm parameter list
            }
        }
    }
    else
    {
        //  TODO: Error Expected identifier
    }
}
void Parser::parseFunction()
{
    if (current()->getTokenType() != TokenType::SYMBOL_FUNCTION)
    {
        //  TODO: Throw error expected FUNCTION symbol
    }
    advance();
    if (current()->getTokenType() != TokenType::IDENTIFIER)
    {
        //  TODO: Throw error expected FUNCTION IDENTIFIER
    }
    else
    {
        // function->identifier = current()->getValue();
        advance(); // eat identifier tokens
        // function->paramList = parseFunctionParams();
    }
    if (current()->getTokenType() == TokenType::SYMBOL_BEGIN)
    {
        if (current()->getTokenType() == TokenType::SYMBOL_END)
        {
            advance();
        }
        else
        {
            //  TODO: Error expected end symbol END
        }
    }
}
void Parser::parseProcedure()
{
}
void Parser::parseFunctionParams()
{
    if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
    {
        advance();
        parseFunctionParameter();
    }
}
void Parser::parseFunctionParameter()
{
    //  ISO 10206 -  6.8.1
    //  FORMAL-PARAMETER-LIST           =   "(" FORMAL-PARAMETER-SECTION { ";" FORMAL-PARAMETER-SECTION } ")"
    //  FORMAL-PARAMETER-SECTION        >   VALUE-PARAMETER-SPECIFICATION       |
    //                                      VARIABLE-PARAMETER-SPECIFICATION    |
    //                                      PROCEDURAL-PARAMETER-SPECIFICATION  |
    //                                      FUNCTIONAL-PARAMETER-SPECIFICATION  |
    //                                      CONFORMANT-ARRAY-PARAMETER-SPECIFICATION
    //  VALUE-PARAMETER-SPECIFICATION       =   [ 'PROTECTED' ] IDENTIFIER-LIST ':' PARAMETER-FORM
    //  VARIABLE-PARAMETER-SPECIFICATION    =   [ 'PROTECTED' ] 'VAR' IDENTIFIER-LIST ':' PARAMETER-FORM
    //  PARAMETER-FORM                      =   TYPE-NAME   |   SCHEMA-NAME |   TYPE-INQUIRY
    //  TYPE-NAME                           =   [ IMPORTED-INTERFACE-IDENTIFIER '.' ] TYPE-IDENTIFIER
    //  SCHEMA-NAME                         =   [ IMPORTED-INTERFACE-IDENTIFIER '.' ] SCHEMA-IDENTIFIER
    //  TYPE-INQUIRY                        =   'TYPE' 'OF' TYPE-INQUIRY-OBJECT
    //  TYPE-INQUIRY-OBJECT                 =   VARIABLE-NAME   | PARAMETER-IDENTIFIER

    ;
    parseFunctionAccessModifier();
    if (current()->getTokenType() == TokenType::SYMBOL_VAR)
    {
        //  TODO:
        advance();
    }
    parseIdentifierList();
    if (current()->getTokenType() == TokenType::SYMBOL_COLON)
    {
        advance();
        switch (current()->getTokenType())
        {
        case TokenType::SYMBOL_TYPE:
            advance();
            if (current()->getTokenType() == TokenType::SYMBOL_OF)
            {
                advance();
                if (current()->getTokenType() == TokenType::IDENTIFIER)
                {
                    advance(); //  TODO: Parse identifier as type of var or param
                }
                else
                {
                    //  TODO: Expected identifier as variable type
                }
            }
            else
            {
                //  TODO: Expected keyword "OF"
            }
            break;
        case TokenType::IDENTIFIER:
            advance(); // eat identifier
            if (current()->getTokenType() == TokenType::SYMBOL_DOT)
            {
                advance(); // eat dot
                if (current()->getTokenType() == TokenType::IDENTIFIER)
                {
                    advance(); // eat identifier
                    //  TODO: Parse identifier as type
                }
                else
                {
                    //  TODO: Error expected identifier after dot.
                }
            }
            else
            {
                //  TODO: Parse identifier as type
            }
            break;
        default:
            //  TODO: Throw error expected param type after
            break;
        }
    }
    else
    {
        //  TODO: Expected colon error
    }
}
void Parser::parseFunctionAccessModifier()
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
void Parser::parseImportStatement()
{
    advance(); // eat IMPORT keyword
    do
    {
        if (current()->getTokenType() != TokenType::IDENTIFIER)
        {
            //  TODO: expected identifier error
            return;
        }
        std::string importName = current()->getValue();
        advance();
        if (current()->getTokenType() == TokenType::SYMBOL_QUALIFIED)
        {
            //  TODO: Qualified statement
            advance(); // eat qualified;
        }
        if (current()->getTokenType() == TokenType::SYMBOL_ONLY)
        {
            advance();
            if (current()->getTokenType() == TokenType::SYMBOL_PAREN_OPEN)
            {
                advance();
                parseImportList();
                if (current()->getTokenType() == TokenType::SYMBOL_PAREN_CLOSE)
                {
                    advance();
                }
                else
                {
                    //  TODO: Error expected open paren
                    return;
                }
            }
            else
            {
                //  TODO: expected open parentesis after only
                return;
            }
        }
        if (current()->getTokenType() != TokenType::SYMBOL_SEMICOLON)
        {
            // TODO: Expected semicolon after import identifier
            return;
        }
        else
        {
            advance();
        }
    } while (current()->getTokenType() == TokenType::IDENTIFIER);
}
void Parser::parseImportList()
{
    while (true)
    {
        if (current()->getTokenType() == TokenType::IDENTIFIER)
        {
            advance();
        }
        else
        {
            //  TODO: Error set missing identifier expected
            return;
        }
        if (current()->getTokenType() != TokenType::SYMBOL_COMMA)
        {
            return;
        }
        advance();
    }
}
void Parser::parseIdentifierList()
{
    //  ISO 10206
    //  IDENTIFIER-LIST = IDENTIFIER { ',' IDENTIFIER }
    if (current()->getTokenType() == TokenType::IDENTIFIER)
    {
        advance(); // eat first ocurrence of identifier
        while (current()->getTokenType() == TokenType::SYMBOL_COMMA)
        {
            advance(); // eat comma
            if (current()->getTokenType() == TokenType::IDENTIFIER)
            {
                current()->getValue();
                advance();
            }
            else
            {
                //  TODO: ERROR expected identifier after comma
                diagnosticsEngine->japc_error_at((*current().get()),
                                                 "Expected identifier after comma while parsing identifier list");
            }
        }
    }
    else
    {
        //  TODO: ERROR expected identifier
        diagnosticsEngine->japc_error_at((*current().get()), "Expected identifier in identifier list");
    }
}
